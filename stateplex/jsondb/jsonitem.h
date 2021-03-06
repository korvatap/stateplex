/*
 * Stateplex - A server-side actor model library.
 *
 * jsondb/jsonitem.h
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

#ifndef INCLUDED_STATEPLEX_JSON_ITEM_H
#define INCLUDED_STATEPLEX_JSON_ITEM_H

#include "../core/object.h"
#include "../core/list.h"
#include "../core/string.h"

namespace Stateplex {

class JsonObject;
class JsonArray;
class Receiver;

class JsonItem : public Object {
	friend class JsonArray;
public:
	enum Type {
		JSON_ITEM_TYPE_INVALID = -1,
		JSON_ITEM_TYPE_NULL = 0,
		JSON_ITEM_TYPE_OBJECT,
		JSON_ITEM_TYPE_ARRAY,
		JSON_ITEM_TYPE_INTEGER,
		JSON_ITEM_TYPE_DECIMAL,
		JSON_ITEM_TYPE_STRING,
		JSON_ITEM_TYPE_BOOLEAN
	};

	struct Observer : public ListItem {
		virtual ~Observer();
		virtual void itemDeleted(JsonItem *item) = 0;
		virtual void elementInserted(JsonArray *array, int index) = 0;
		virtual void elementRemoved(JsonArray *array, int index) = 0;
		virtual void memberSet(JsonObject *object, const String *name) = 0;
		virtual void memberUnset(JsonObject *object, const String *name) = 0;
	};

private:
	JsonItem *mParent;
	List<Observer> mObservers;

protected:
	JsonItem(Actor *actor);
	JsonItem(JsonItem *parent);

	void notifyElementInserted(JsonArray *array, int index) const;
	void notifyElementRemoved(JsonArray *array, int index) const;
	void notifyMemberSet(JsonObject *object, const String *name) const;
	void notifyMemberUnset(JsonObject *object, const String *name) const;
	String *escape(long int integer) const;
	String *escape(double decimal) const;
	String *escape(const String *string) const;

public:
	virtual ~JsonItem();

	JsonItem *parent() const;
	void send(Receiver *receiver) const;
	virtual void send(Receiver *receiver, Size depth) const = 0;
	virtual Type type() const = 0;
	bool isObject() const;
	bool isArray() const;
	void addObserver(Observer *observer);
};

}

/*** Inline implementations ***/

#include "../core/source.h"

namespace Stateplex {

inline JsonItem::JsonItem(Actor *actor)
	: Object(actor), mParent(0)
{ }

inline JsonItem::JsonItem(JsonItem *parent)
	: Object(parent->actor()), mParent(parent)
{ }

inline JsonItem *JsonItem::parent() const
{
	return mParent;
}

inline void JsonItem::addObserver(Observer *observer)
{
	mObservers.addTail(observer);
}

inline void JsonItem::send(Receiver *receiver) const
{
	send(receiver, 0xffffffff); // TODO: size_max
}

inline bool JsonItem::isObject() const
{
	return type() == JSON_ITEM_TYPE_OBJECT;
}

inline bool JsonItem::isArray() const
{
	return type() == JSON_ITEM_TYPE_ARRAY;
}

}

#endif
