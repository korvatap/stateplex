/*
 * Stateplex - A server-side actor model library.
 *
 * core/list.h
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

#ifndef INCLUDED_STATEPLEX_LIST_H
#define INCLUDED_STATEPLEX_LIST_H

#include <cstddef>

namespace Stateplex {

template<typename T>
class List;

template<typename T>
struct ListItem {
	ListItem<T> *mNext;
	ListItem<T> *mPrevious;

	ListItem();

	void addBetween(ListItem<T> *previous, ListItem<T> *next);
	void addBefore(ListItem<T> *existing_item);
	void addAfter(ListItem<T> *existing_item);
	void initialise();
	void remove();
	T *container();
};

template<typename T>
class List {
	ListItem<T> mItems;

public:
	List();
	
	ListItem<T> *first();
	ListItem<T> *last();
	ListItem<T> *next(ListItem<T> *item);
	ListItem<T> *previous(ListItem<T> *item);
	
	void addBetween(ListItem<T> *previous, ListItem<T> *next);
	void addHead(ListItem<T> *item);
	void addTail(ListItem<T> *item);
	void spliceHead(List<T> *list);
	void spliceTail(List<T> *list);

	bool isEmpty();
};

}

/*** Inline implementations ***/

namespace Stateplex {

template<typename T>
inline ListItem<T>::ListItem()
{
	initialise();
}

template<typename T>
inline void ListItem<T>::initialise()
{
	mNext = mPrevious = this;
}

template<typename T>
inline void ListItem<T>::addBetween(ListItem<T> *previous, ListItem<T> *next)
{
	previous->mNext = this;
	mPrevious = previous;
	mNext = next;
	next->mPrevious = this;
}

template<typename T>
inline void ListItem<T>::addBefore(ListItem<T> *existing_item)
{
	mPrevious->mNext = mNext;
	mNext->mPrevious = mPrevious;
	addBetween(existing_item->mPrevious, existing_item);
}

template<typename T>
inline void ListItem<T>::addAfter(ListItem<T> *existing_item)
{
	mNext->mPrevious = mPrevious;
	mPrevious->mNext = mNext;
	addBetween(existing_item, existing_item->mNext);
}

template<typename T>
inline void ListItem<T>::remove()
{
	mPrevious->mNext = mNext;
	mNext->mPrevious = mPrevious;
	mNext = this;
	mPrevious = this;
}

template<typename T>
inline T *ListItem<T>::container()
{
	return reinterpret_cast<T *>(this);
}

template<typename T>
inline List<T>::List()
{
	mItems.initialise();
}

template<typename T>
inline ListItem<T> *List<T>::first()
{
	return (mItems.mNext != &mItems ? mItems.mNext : 0);
}

template<typename T>
inline ListItem<T> *List<T>::last()
{
	return (mItems.mPrevious != &mItems ? mItems.mPrevious : 0);
}

template<typename T>
ListItem<T> *List<T>::next(ListItem<T> *item)
{
	return (item->mNext != &mItems ? item->mNext : 0);
}

template<typename T>
ListItem<T> *List<T>::previous(ListItem<T> *item)
{
	return (item->mPrevious != &mItems ? item->mPrevious : 0);
}

template<typename T>
inline void List<T>::addBetween(ListItem<T> *previous, ListItem<T> *next)
{
	previous->mNext = mItems.mNext;
	mItems.mNext->mPrevious = previous;
	mItems.mPrevious->mNext = next;
	next->mPrevious = mItems.mPrevious;

	mItems.initialise();
}

template<typename T>
inline void List<T>::addHead(ListItem<T> *item)
{
	item->addAfter(&mItems);
}

template<typename T>
inline void List<T>::addTail(ListItem<T> *item)
{
	item->addBefore(&mItems);
}

template<typename T>
inline void List<T>::spliceHead(List<T> *list)
{
	addBetween(&mItems, mItems.mNext);
}

template<typename T>
inline void List<T>::spliceTail(List<T> *list)
{
	addBetween(mItems.mPrevious, &mItems);
}

template<typename T>
inline bool List<T>::isEmpty()
{
	return mItems.mNext == &mItems;
}

}

#endif
