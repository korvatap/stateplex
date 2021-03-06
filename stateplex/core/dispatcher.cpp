/*
 * Stateplex - A server-side actor model library.
 *
 * core/dispatcher.cpp
 *
 * (c) 2013 Henrik Hedberg <henrik.hedberg@innologies.fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Authors: Henrik Hedberg
 */

#include <sys/time.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "dispatcher.h"
#include "actor.h"
#include "allocator.h"

/* TODO: Parametrise this */
#define MAX_EVENTS 10

namespace Stateplex {

Spinlock Dispatcher::sDispatchLock;
Allocator *Dispatcher::sRecycledAllocator;
__thread Dispatcher *Dispatcher::sCurrentDispatcher;

/**
 * Default constructor for dispatcher.
 * Also creates the epoll file discriptor.
 */

Dispatcher::Dispatcher()
	: mRunning(true), mMilliseconds(0)
{
	if (sCurrentDispatcher)
		abort();
	sCurrentDispatcher = this;

	mEpollFd = epoll_create(1024);
	mAllocator = new Allocator();

	sDispatchLock.lock();
	if (!sRecycledAllocator)
		sRecycledAllocator = new Allocator();
	sDispatchLock.unlock();
}

void Dispatcher::activateActor(Actor *actor)
{
	if (!actor->mActive) {
		mActiveActors.addTail(actor);
		actor->mActive = 1;
	}
}

/**
 * Function that dispatch incoming and outgoing messages with the lock hold,handles time out for active actors. 
 * Dispatch outgoing messages with the lock hold
 * Dispatch incoming messages with the lock hold
 * Release the lock if it was acquired in dispatching
 * Handle timeouts for waiting actors
 * Handle active actors i.e. actors that have incoming messages
 */

void Dispatcher::run()
{
	bool locked;
	struct epoll_event events[MAX_EVENTS];
	int n_events;
	struct timeval tv;
	List<Actor> actors;
	int timeout;
	
	while (mRunning) {
		locked = false;

		/* Dispatch outgoing messages with the lock hold */
		if (!mOutgoingMessages.isEmpty()) {
			sDispatchLock.lock();
			locked = true;
			for (ListIterator<Message<Actor> > iterator(&mOutgoingMessages); iterator.hasCurrent(); iterator.subsequent()) {
				Message<Actor> *message = iterator.current();
				Actor *receiver = message->mReceiver;
				receiver->mQueuedMessages.addTail(message);
				activateActor(receiver);
			}
		}

		/* Dispatch incoming messages with the lock hold */
		actors.spliceTail(&mActiveActors);
		if (!actors.isEmpty()) {
			if (!locked) {
				sDispatchLock.lock();
				locked = true;
			}
			for (ListIterator<Actor> iterator(&actors); iterator.hasCurrent(); iterator.subsequent()) {
				Actor *actor = iterator.current();
				actor->mIncomingMessages.spliceTail(&actor->mQueuedMessages);
			}
		}

		/* Recycle allocators and release the lock if the lock was acquired in dispatching */
		if (locked) {
			Allocator *allocator = sRecycledAllocator;
			sRecycledAllocator = mAllocator;
			mAllocator = allocator;

			sDispatchLock.unlock();
		}

		/* Start a new cycle */
		gettimeofday(&tv, 0);
		mMilliseconds = tv.tv_sec * 1000 + tv.tv_usec / 1000;

		/* Do not block, if there are active actors, check when the next waiting actor timeouts, or wait indefinitely */
		if (!actors.isEmpty())
			timeout = 0;
		else if(!mWaitingActors.isEmpty()) {
			timeout = mWaitingActors.first()->nextTimeoutMilliseconds() - mMilliseconds;
			if (timeout < 0)
				timeout = 0;
		} else
			timeout = -1;

		/* Receive external events i.e. poll sources */
		n_events = epoll_wait(mEpollFd, events, MAX_EVENTS, timeout);
		if (n_events == -1) {
			if (errno != EINTR) {
				perror("epoll_wait");
				abort();
			}
		} else {
			for (int i = 0; i < n_events; i++) {
				Source *source = reinterpret_cast<Source *>(events[i].data.ptr);
				source->handleReady(events[i].events & EPOLLIN, events[i].events & EPOLLOUT);
			}
		}

		/* Handle timeouts for waiting actors */
		for (ListIterator<Actor> iterator(&mWaitingActors); iterator.hasCurrent(); iterator.subsequent()) {
			Actor *actor = iterator.current();

			if (actor->nextTimeoutMilliseconds() > mMilliseconds) {
				break;
			}

			bool alive = actor->handleTimeouts(mMilliseconds);
			if (alive) {
				if (!actor->nextTimeoutMilliseconds())
					mPassiveActors.addTail(actor);
				else
					waitTimeout(actor);
			} else
				delete actor;

		}

		/* Handle active actors i.e. actors that have incoming messages */
		for (ListIterator<Actor> iterator(&actors); iterator.hasCurrent(); iterator.subsequent()) {
			Actor *actor = iterator.current();

			bool alive = actor->handleMessages(mMilliseconds);
			if (alive) {
				if (!actor->mIncomingMessages.isEmpty())
					mActiveActors.addTail(actor);
				else if (!actor->nextTimeoutMilliseconds()) {
					actor->mActive = 0;
					mPassiveActors.addTail(actor);
				} else {
					actor->mActive = 0;
					waitTimeout(actor);
				}
			} else
				delete actor;
		}
	}
}

/**
 * Function that adds or updates source.
 *
 * @param *source        target source to add or update.
 * @param epollOperation command for epoll.
 */

void Dispatcher::addOrUpdateSource(Source *source, int epollOperation)
{
	struct ::epoll_event event;

	event.events = (source->mReadable ? EPOLLIN : 0 ) | (source->mWritable ? EPOLLOUT : 0) | EPOLLET;
	event.data.ptr = source;
	epoll_ctl(mEpollFd, epollOperation, source->mFd, &event);
}

/**
 * Function that removes the source.
 *
 * @param *source        target source to remove.
 */

void Dispatcher::removeSource(Source *source)
{
	epoll_ctl(mEpollFd, EPOLL_CTL_DEL, source->mFd, 0);
}

/**
 * Function that handles timeout for waiting actors.
 *
 * @return		void if existing timeout is larger that the specified actor's timeout.
 */

void Dispatcher::waitTimeout(Actor *actor)
{
	for (ListIterator<Actor> iterator(&mWaitingActors); iterator.hasCurrent(); iterator.subsequent()) {
		Actor *existing = iterator.current();
		if (actor->nextTimeoutMilliseconds() < existing->nextTimeoutMilliseconds()) {
			actor->addBefore(existing);
			return;
		}
	}
	mWaitingActors.addTail(actor);
}

}
