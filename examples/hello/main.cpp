/*
 * Stateplex - A server-side actor model library.
 *
 * examples/hello/main.cpp
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

#include <stateplex/core/dispatcher.h>
#include "greeteractor.h"

int main(void)
{
	Stateplex::Dispatcher dispatcher;

	GreeterActor greeter(&dispatcher);
	greeter.greet(0, "Hello, actors!\n");

	dispatcher.run();

	return 0;
}
