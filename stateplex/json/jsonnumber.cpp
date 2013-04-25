#include "jsonnumber.h"
#include <iostream>

JsonNumber::JsonNumber(Stateplex::Actor *owner, const char *key, int number)
	: JsonItem(owner)
{
	mKey = const_cast<char*>(key);
	mValue = number;
	mType = JSON_NUMBER;
}

void JsonNumber::traverse()
{
	std::cout << "\t" << '"' << mKey << '"' << " : " << mValue << std::endl;
}

int JsonNumber::value()
{
	return mValue;
}

const char *JsonNumber::key()
{
	return const_cast<const char*>(mKey);
}

JsonType JsonNumber::type()
{
	return mType;
}

void JsonNumber::setValue(int value)
{
	mValue = value;
}

JsonNumber::~JsonNumber()
{
        //dtor
}
