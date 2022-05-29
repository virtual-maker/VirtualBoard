/* Copyright 2018 Paul Stoffregen
 *
 * Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
 * Copyright (c) 2022 Immo Wache
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>

#include "EthernetClient.h"
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <utils/log.h>

EthernetClient::EthernetClient() : _sock(-1)
{
}

EthernetClient::EthernetClient(int sock) : _sock(sock)
{
}

EthernetClient::~EthernetClient()
{
	// ToDo: do we really need this
}

int EthernetClient::connect(const char* host, uint16_t port)
{
	close();

	int result = ethernetWrapper.clientConnect(host, port, &_sock);
	if (result != 1) {
		logError("connect: %d %s - %s\n", ethernetWrapper.clientErrorCode(_sock), 
			ethernetWrapper.clientSocketErrorCode(_sock), ethernetWrapper.clientErrorMessage(_sock));
	}
	return result;
}

int EthernetClient::connect(IPAddress ip, uint16_t port)
{
	return connect(ip.toString().c_str(), port);
}

size_t EthernetClient::write(uint8_t b)
{
	return write(&b, 1);
}

size_t EthernetClient::write(const uint8_t *buf, size_t size)
{
	if (_sock == -1) {
		return 0;
	}
	size_t bytes = 0;
	while (size > 0) {
		int rc = ethernetWrapper.clientWrite(_sock, buf + bytes, size);
		if (rc == -1) {
			logError("send: %s\n", strerror(errno));
			close();
			break;
		}
		bytes += rc;
		size -= rc;
	}
	return bytes;
}

size_t EthernetClient::write(const char *str)
{
	if (str == NULL) {
		return 0;
	}
	return write((const uint8_t *)str, strlen(str));
}

size_t EthernetClient::write(const char *buffer, size_t size)
{
	if (size == 0) {
		return 0;
	}
	return write((const uint8_t *)buffer, size);
}

size_t EthernetClient::write_P(PGM_P buf, size_t size)
{
	return write(buf, size);
}

int EthernetClient::available()
{
	return ethernetWrapper.clientAvailable(_sock);
}

int EthernetClient::read()
{
	uint8_t b;
	if (read((unsigned char*)&b, 1) >= 0 ) {
		// read worked
		return b;
	} else {
		// No data available
		return -1;
	}
}

int EthernetClient::read(uint8_t *buf, size_t bytes)
{
	return ethernetWrapper.clientRead(_sock, buf, bytes);
}

int EthernetClient::peek()
{
	return ethernetWrapper.clientPeek(_sock);
}

void EthernetClient::flush()
{
	ethernetWrapper.clientFlush(_sock);
}

void EthernetClient::stop()
{
	if (_sock != -1) {
		ethernetWrapper.clientClose(_sock);
		_sock = -1;
	}
	return;

	if (_sock == -1) {
		return;
	}
	ethernetWrapper.clientStop(_sock);
	_sock = -1;
}

uint8_t EthernetClient::status()
{
	if (_sock == -1) {
		return ETHERNETCLIENT_W5100_CLOSED;
	} else {
		return 	ethernetWrapper.clientStatus(_sock);
	}
}

uint8_t EthernetClient::connected()
{
	return ethernetWrapper.clientConnected(_sock);
}

void EthernetClient::close()
{
	if (_sock != -1) {
		ethernetWrapper.clientClose(_sock);
		_sock = -1;
	}
}

void EthernetClient::bind(IPAddress ip)
{
	// Do nothing
}

int EthernetClient::getSocketNumber()
{
	return _sock;
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

EthernetClient::operator bool()
{
	return _sock != -1;
}

bool EthernetClient::operator==(const EthernetClient& rhs)
{
	return _sock == rhs._sock && _sock != -1 && rhs._sock != -1;
}

EthernetWrapper ethernetWrapper;