#include "jsonobject.h"
#include <iostream>

JsonObject::JsonObject(const char *key)
{
        mKey = key;
	mItems = new Stateplex::List<JsonItem>;
}

JsonObject::~JsonObject()
{
        freeItems();
	delete mItems;
}

void JsonObject::add(JsonItem *item)
{
	mItems->addTail(item);
}

void JsonObject::traverse()
{
	std::cout << '"' <<  key() << '"' << ":" << std::endl;
	for (Stateplex::ListIterator<JsonItem> iterator(mItems); iterator.hasCurrent(); iterator.subsequent()) {
		JsonObject *item = reinterpret_cast<JsonObject *>(iterator.current());
		item->traverse();
	}

}

/*JsonNumber *JsonObject::nextJson()
{
	return mIterator.subsequent();
}*/

Stateplex::List<JsonItem> *JsonObject::getList()
{
	return mItems;
}

const char *JsonObject::key()
{
	return mKey;
}

void JsonObject::freeItems()
{
	for (Stateplex::ListIterator<JsonItem> iterator(mItems); iterator.hasCurrent(); iterator.subsequent()) {
		JsonObject *item = reinterpret_cast<JsonObject *>(iterator.current());
		delete item;
	}

}

JsonItem *JsonObject::find(const char *target)
{
	JsonItem *item;
	for (Stateplex::ListIterator<JsonItem> iterator(mItems); iterator.hasCurrent(); iterator.subsequent()) {
		if(strcmp(iterator.current()->key(), target)) {
			item = iterator.current();
		}
	}

	return item;
}

JsonObject *JsonObject::findObject(const char *target)
{
	JsonObject *item;
	for (Stateplex::ListIterator<JsonItem> iterator(mItems); iterator.hasCurrent(); iterator.subsequent()) {
		if(strcmp(iterator.current()->key(), target) == 0) {
			item = reinterpret_cast<JsonObject *> (iterator.current());
		}
	}

	return item;
}
