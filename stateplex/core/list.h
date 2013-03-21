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
class ListIterator;

/**
 * Class ListItem are stored in to lists and
 * they behave like normal listitems in a list.
 */

class ListItem {
	template<typename T> friend class List;

	ListItem *mNext;
	ListItem *mPrevious;

	void addBetween(ListItem *previous, ListItem *next);

public:
	ListItem();
	virtual ~ListItem();

	void addBefore(ListItem *existing_item);
	void addAfter(ListItem *existing_item);
	void remove();
};

/**
 * Class List has list items stored inside of it and it can be
 * iterated with class ListIterator.
 */

template<typename T>
class List {
	template<typename S> friend class ListIterator;

	ListItem mItems;

	void addBetween(ListItem *previous, ListItem *next);
	ListItem *next(ListItem *item);
	ListItem *previous(ListItem *item);

public:
	List();
	
	T *first();
	T *last();
	
	void addHead(T *item);
	void addTail(T *item);
	void spliceHead(List<T> *list);
	void spliceTail(List<T> *list);

	bool isEmpty();
};

/**
 * Class ListIterator can be assigned to iterate a list.
 * Iterator goes through the list one list item at time
 * backwards or forwards.
 */

template<typename T>
class ListIterator {
	List<T> *mList;
	ListItem *mCurrent;
	ListItem *mSubsequent;
	bool mBackwards;
	
public:
	ListIterator(List<T> *list, bool backwards = false);

	List<T> *list();
	T *current();
	T *subsequent();
	bool hasCurrent();
	bool hasSubsequent();
	bool backwards();
};

}


/*** Inline implementations ***/
namespace Stateplex {

/** 
 * A default constructor that initializes a new instance of class ListItem.
 */

inline ListItem::ListItem()
	: mNext(this), mPrevious(this)
{ }

/** 
 * Destructor for class ListItem.
 */

inline ListItem::~ListItem()
{ }

/**
 * Adds a ListItem to the list between two list items.
 *
 * @param (*previous) is a pointer to the previous list item.
 * @param (*next) is a pointer to the next list item.
 */
 
inline void ListItem::addBetween(ListItem *previous, ListItem *next)
{
	previous->mNext = this;
	mPrevious = previous;
	mNext = next;
	next->mPrevious = this;
}

/**
 * Adds a list item to the list before an existing list item.
 *
 * @param (*existing _item) is a pointer to the existing list item.
 */

inline void ListItem::addBefore(ListItem *existing_item)
{
	ListItem *previous = existing_item->mPrevious;

	mPrevious->mNext = mNext;
	mNext->mPrevious = mPrevious;
	addBetween(previous, existing_item);
}

/**
 * Adds a list item after an existing list item.
 *
 * @param (existing _item) is a pointer to the existing list item.
 */

inline void ListItem::addAfter(ListItem *existing_item)
{
	ListItem *next = existing_item->mNext;

	mNext->mPrevious = mPrevious;
	mPrevious->mNext = mNext;
	addBetween(existing_item, next);
}

/**
 * Removes a list item address from a list.
 */

inline void ListItem::remove()
{
	mPrevious->mNext = mNext;
	mNext->mPrevious = mPrevious;
	mNext = this;
	mPrevious = this;
}

/**
 * Default constructor of List class.
 */

template<typename T>
List<T>::List()
{ }

/**
 * Returns the next list item address in a list.
 *
 * @return T* list item address, else 0.
 */
 
template<typename T>
T *List<T>::first()
{
	return static_cast<T *>(mItems.mNext != &mItems ? mItems.mNext : 0);
}

/**
 * Returns the previous list item from a List.
 *
 * @return T* list item address, else 0.
 *
 */

template<typename T>
T *List<T>::last()
{
	return static_cast<T *>(mItems.mPrevious != &mItems ? mItems.mPrevious : 0);
}


/**
 * Returns next list items address related to a specified list item.
 *
 * @param *item list items, whose next item will be returned.
 * @return *item or zero.
 */

template<typename T>
ListItem *List<T>::next(ListItem *item)
{
	return (item->mNext != &mItems ? item->mNext : 0);
}

/**
 * Returns previous ListItem from a List.
 *
 * @param *item 	is a ListItem, whose previous item will be returned.
 * @return mNext	next list item or zero.
 */

template<typename T>
ListItem *List<T>::previous(ListItem *item)
{
	return (item->mPrevious != &mItems ? item->mPrevious : 0);
}

/**
 * Function that adds list item between two specified items.
 * @param *previous and *next 		pointer adresses to the specified items. 
 *
 */

template<typename T>
void List<T>::addBetween(ListItem *previous, ListItem *next)
{
	previous->mNext = mItems.mNext;
	mItems.mNext->mPrevious = previous;
	mItems.mPrevious->mNext = next;
	next->mPrevious = mItems.mPrevious;

	mItems.mNext = mItems.mPrevious = &mItems;
}

/**
 * Function that adds list's listitem to ahead of a specific listitem.
 * 
 * @param *item	ahead of this pointer address is placed another listitem.
 */

template<typename T>
void List<T>::addHead(T *item)
{
	item->addAfter(&mItems);
}

/**
 * Function that adds list's listitem to behind of a specific listitem.
 * 
 * @param *item	behind of this pointer address is placed another listitem.
 */

template<typename T>
void List<T>::addTail(T *item)
{
	item->addBefore(&mItems);
}

/**
 * Function that adds specified list's listitem between
 * objects listitem and the next listitem.
 *  
 * @see List::addBetween.
 */

template<typename T>
void List<T>::spliceHead(List<T> *list)
{
	list->addBetween(&mItems, mItems.mNext);
}

/**
 * Function that adds specified list's listitem beteween
 * objects previous listitem and the objects listitem.
 *
 * @see List::addBetween.
 */

template<typename T>
void List<T>::spliceTail(List<T> *list)
{
	list->addBetween(mItems.mPrevious, &mItems);
}
/**
 * Function that checks it there aren't any listitems to go through.
 *
 * @return true if there isn't a next list item, otherwise false.
 */
template<typename T>
bool List<T>::isEmpty()
{
	return mItems.mNext == &mItems;
}

/**
 * Constructor for class ListIterator.
 * The constructor initializes a new instance of class ListIterator.
 * Also sets is the list iterated backwards or not.
 */

template<typename T>
ListIterator<T>::ListIterator(List<T> *list, bool backwards)
	: mList(list), mBackwards(backwards)
{
	if (!backwards) {
		mCurrent = mList->first();
		if (mCurrent)
			mSubsequent = mList->next(mCurrent);
	} else {
		mCurrent = list->last();
		if (mCurrent)
			mSubsequent = mList->previous(mCurrent);
	}
}

/**
 * Function that returns the list pointer address that is being iterated.
 *
 * @return mList	pointer to the list being iterated.
 */

template<typename T>
List<T> *ListIterator<T>::list()
{
	return mList;
}

/**
 * Function that returns the list item that is currently being iterated.
 *
 * @return mCurrent	current list item iterated.
 */

template<typename T>
T *ListIterator<T>::current()
{
	return static_cast<T *>(mCurrent);
}

/**
 * Function that returns next item depending 
 * is the list iterator going through the list backward or forwards.
 *
 * @return mCurrent	next item to iterate.
 */

template<typename T>
T *ListIterator<T>::subsequent()
{
	mCurrent = mSubsequent;
	if (mCurrent)
		mSubsequent = (!mBackwards ? mList->next(mCurrent) : mList->previous(mCurrent));

	return static_cast<T *>(mCurrent);
}

/**
 * Function that checks if the iterator has a listitem selected.
 *
 * @return mCurrent	true if iterator has listitem selected, else false.
 */

template<typename T>
bool ListIterator<T>::hasCurrent()
{
	return mCurrent != 0;
}

/**
 * Function that checks if the iterator has a next item.
 *
 * @return mSubsequent	true if iterator has next item, else false.
 */

template<typename T>
bool ListIterator<T>::hasSubsequent()
{
	return mSubsequent != 0;
}

/**
 * Function that checks if the iterator is going through the list backwards.
 *
 * @return mBackwards	true if iterator is going through the list backwards, else false.
 */

template<typename T>
bool ListIterator<T>::backwards()
{
	return mBackwards;
}

}

#endif
