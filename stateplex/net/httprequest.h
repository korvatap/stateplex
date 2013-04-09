/*
 * Stateplex - A server-side actor model library.
 *
 * net/httprequest.h
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

#ifndef INCLUDED_STATEPLEX_HTTP_REQUEST_H
#define INCLUDED_STATEPLEX_HTTP_REQUEST_H

#include <sys/types.h>
#include <sys/socket.h>

#include "../core/buffer.h"

namespace Stateplex {

class HttpServer;
class TcpConnection;
class IoSource;
class HttpConnection;

/**
 * HTTP request handler.
 * One HttpConnection may carry through multiple requests.
 */

class HttpRequest {
	friend class HttpConnection;

	HttpConnection *mHttpConnection;

protected:
	/**
	 * Receives an HTTP header belonging to the request.
	 */
	virtual bool receiveHeader(const Buffer<> *name, const Buffer<> *value) = 0;

	/**
	 * Receives an HTTP message body content belonging to the request.
	 */
	virtual Size receiveData(const Buffer<> *data) = 0;

	/**
	 * Indicates that the full HTTP request is now received.
	 *
	 * The HttpServer does not hold pointer nor call any of the functions of the HttpRequest after this call.
	 * The HttpRequest must take care of freeing all of its resources by its own.
	 */
	virtual void receiveEnd() = 0;

	/**
	 * Indicates that the HTTP request is aborted, because the other end has closed the connection.
	 *
	 * The HttpServer does not hold pointer nor call any of the functions of the HttpRequest after this call.
	 * The HttpRequest must take care of freeing all of its resources by its own.
	 */
	virtual void receiveAbort() = 0;

public:
	class Embryo {
	public:
		HttpConnection *httpConnection;
		const Buffer<> *method;
		const Buffer<> *uri;

		Embryo(HttpConnection *httpConnection, const Buffer<> *method, const Buffer<> *uri);
	};

	HttpRequest(HttpConnection *httpConnection);
	virtual ~HttpRequest();

	HttpConnection *httpConnection() const;

	void sendStatus(const char *status, Size statusLength);
	void sendStatus(const Buffer<> *status);
	void sendHeader(const char *name, Size nameLength, const char *value, Size valueLength);
	void sendHeader(const Buffer<> *name, const Buffer<> *value);
	void sendData(const char *data, Size dataLength);
	void sendData(const Buffer<> *data);
	void sendEnd();
};

}

/*** Inline implementations ***/

#include "tcpconnection.h"

namespace Stateplex {

/**
 * Constructs a new request and associates it to the given HttpConnection.
 */

inline HttpRequest::HttpRequest(HttpConnection *httpConnection)
	: mHttpConnection(httpConnection)
{ }

/**
 * Destroys the request.
 */

inline HttpRequest::~HttpRequest()
{
	/* TODO: mTcpConnection->unsetHandler(); */
}

/**
 * Returns the HttpConnection associated with the request.
 */

inline HttpConnection *HttpRequest::httpConnection() const
{
	return mHttpConnection;
}

inline HttpRequest::Embryo::Embryo(HttpConnection *httpConnection, const Buffer<> *method, const Buffer<> *uri)
	: httpConnection(httpConnection), method(method), uri(uri)
{ }

}

#endif