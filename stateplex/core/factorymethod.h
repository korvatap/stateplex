/*
 * Stateplex - A server-side actor model library.
 *
 * core/factorymethod.h
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

#ifndef INCLUDED_STATEPLEX_FACTORY_METHOD_H
#define INCLUDED_STATEPLEX_FACTORY_METHOD_H

class Unknown;

namespace Stateplex {

template<typename Return>
class FactoryMethod {
	Unknown *mObject;
	Return *(Unknown::*mFunction)(Unknown *argument);

	Return *warnUninitialisedMethod(Unknown *argument);

public:
	FactoryMethod();
	template<typename Type, typename Argument> FactoryMethod(Type *object, Return *(Type::*function)(Argument *argument));

	template<typename Type, typename Argument> void set(Type *object, Return *(Type::*function)(Argument *argument));
	template<typename Argument> Return *invoke(Argument *argument) const;
};

}

/*** Template implementations ***/

#include "method.h"

namespace Stateplex {

template<typename Return>
FactoryMethod<Return>::FactoryMethod()
	: mObject(reinterpret_cast<Unknown *>(this)), mFunction(reinterpret_cast<Return *(Unknown::*)(Unknown *)>(&Method::warnUninitialisedMethod))
{ }

template<typename Return> template<typename Type, typename Argument>
FactoryMethod<Return>::FactoryMethod(Type *object, Return *(Type::*function)(Argument *argument))
	: mObject(reinterpret_cast<Unknown *>(object)), mFunction(reinterpret_cast<Return *(Unknown::*)(Unknown *)>(function))
{ }

template<typename Return> template<typename Type, typename Argument>
inline void FactoryMethod<Return>::set(Type *object, Return *(Type::*function)(Argument *argument))
{
	mObject = reinterpret_cast<Unknown *>(object);
	mFunction = reinterpret_cast<Return *(Unknown::*)(Unknown *)>(function);
}

template<typename Return> template<typename Argument>
inline Return *FactoryMethod<Return>::invoke(Argument *argument) const
{
	return (mObject->*mFunction)(reinterpret_cast<Unknown *>(argument));
}

}

#endif