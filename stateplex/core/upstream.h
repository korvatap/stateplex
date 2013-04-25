/*
 * Stateplex - A server-side actor model library.
 *
 * core/upstream.h
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

#ifndef INCLUDED_STATEPLEX_UPSTREAM_H
#define INCLUDED_STATEPLEX_UPSTREAM_H

#include "object.h"
#include "types.h"
#include "buffer.h"

namespace Stateplex {

class Downstream;
class String;

class Upstream : public virtual Object {
	friend class Downstream;

	Downstream *mDownstream;

protected:
	Upstream(Actor *actor);

	virtual void receiveDrainedFromDownstream() = 0;
	virtual void receiveFromDownstream(const char *data, Size length) = 0;
	virtual void receiveFromDownstream(Buffer<> *buffer) = 0;

public:
	virtual ~Upstream();

	Downstream *downstream() const;
	void setDownstream(Downstream *downstream);
	void sendDrainedToDownstream() const;
	void sendToDownstream(Buffer<> *buffer) const;
	void sendToDownstream(const char *data, Size length) const;
	void sendToDownstream(const String *string) const;
};

}

/*** Inline implementations ***/

#include "downstream.h"
#include "string.h"

namespace Stateplex {

inline Upstream::Upstream(Actor *actor)
	: Object(actor), mDownstream(0)
{ }

inline Upstream::~Upstream()
{ }

inline void Upstream::setDownstream(Downstream *downstream)
{
	if (mDownstream == downstream)
		return;

	if (mDownstream)
		mDownstream->setUpstream(0);

	mDownstream = downstream;

	if (mDownstream)
		mDownstream->setUpstream(this);

}

inline void Upstream::sendToDownstream(Buffer<> *buffer) const
{
	mDownstream->receiveFromUpstream(buffer);
}

inline void Upstream::sendToDownstream(const char *data, Size length) const
{
	mDownstream->receiveFromUpstream(data, length);
}

inline void Upstream::sendToDownstream(const String *string) const
{
	mDownstream->receiveFromUpstream(string->chars(), string->length());
}

inline void Upstream::sendDrainedToDownstream() const
{
	mDownstream->receiveDrainedFromUpstream();
}

}

#endif