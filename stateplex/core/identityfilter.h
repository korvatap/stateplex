/*
 * Stateplex - A server-side actor model library.
 *
 * core/identityfilter.h
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

#ifndef INCLUDED_STATEPLEX_IDENTITY_FILTER_H
#define INCLUDED_STATEPLEX_IDENTITY_FILTER_H

#include "filter.h"

namespace Stateplex {

class IdentityFilter : public Object, public Filter {
protected:
	virtual void receiveEnd();
	virtual bool receive(const String *string);
	virtual bool receive(Buffer *buffer);

public:
	IdentityFilter(Actor *actor, Receiver *receiver);
	virtual ~IdentityFilter();

};

}

/*** Inline implementations ***/

namespace Stateplex {

/*
 * Constructor that initialize a new instances of IdentityFilter.
 *
 * @param *actor	pointer to Actor.
 * @param *receiver	pointer to Receiver.
 */

inline IdentityFilter::IdentityFilter(Actor *actor, Receiver *receiver)
	: Object(actor), Filter(receiver)
{ }

/*
 * Default destructor.
 */

inline IdentityFilter::~IdentityFilter()
{ }

}

#endif
