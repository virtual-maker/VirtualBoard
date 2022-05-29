/*
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * for the MySensors Arduino library
 * Copyright (C) 2013-2017 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
 * Copyright (c) 2022 Immo Wache
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef EthernetServer_h
#define EthernetServer_h

#include <list>
#include <vector>
#include <Server.h>
#include <IPAddress.h>
#include "EthernetClient.h"

#include "EthernetWrapper.h"

#ifdef ETHERNETSERVER_MAX_CLIENTS
#define ETHERNETSERVER_BACKLOG ETHERNETSERVER_MAX_CLIENTS //!< Maximum length to which the queue of pending connections may grow.
#else
#define ETHERNETSERVER_MAX_CLIENTS 10 //!< Default value for max_clients.
#define ETHERNETSERVER_BACKLOG 10 //!< Maximum length to which the queue of pending connections may grow.
#endif

class EthernetClient;

/**
 * @brief EthernetServer class
 */
class EthernetServer : public Server
{

public:
	/**
	 * @brief EthernetServer constructor.
	 *
	 * @param port number for the socket addresses.
	 * @param max_clients The maximum number allowed for connected clients.
	 */
	EthernetServer(uint16_t port, uint16_t max_clients = ETHERNETSERVER_MAX_CLIENTS);
	/**
	 * @brief Listen for inbound connection request.
	 *
	 */
	virtual void begin();
	/**
	 * @brief Listen on the specified ip for inbound connection request.
	 *
	 * @param addr IP address to bind to.
	 */
	void begin(IPAddress addr);
	/**
	 * @brief Verifies if a new client has connected.
	 *
	 * @return @c true if a new client has connected, else @c false.
	 */
	bool hasClient();
	/**
	 * @brief Get the new connected client.
	 *
	 * @return a EthernetClient object; if no new client has connected, this object will evaluate to false.
	 */
	EthernetClient available();
	/**
	 * @brief Write a byte to all clients.
	 *
	 * @param b byte to send.
	 * @return 0 if FAILURE or 1 if SUCCESS.
	 */
	virtual size_t write(uint8_t b);
	/**
	 * @brief Write at most 'size' bytes to all clients.
	 *
	 * @param buffer to read from.
	 * @param size of the buffer.
	 * @return 0 if FAILURE else number of bytes sent.
	 */
	virtual size_t write(const uint8_t *buffer, size_t size);
	/**
	 * @brief Write a null-terminated string to all clients.
	 *
	 * @param str String to write.
	 * @return 0 if FAILURE else number of characters sent.
	 */
	size_t write(const char *str);
	/**
	 * @brief Write at most 'size' characters to all clients.
	 *
	 * @param buffer to read from.
	 * @param size of the buffer.
	 * @return 0 if FAILURE else the number of characters sent.
	 */
	size_t write(const char *buffer, size_t size);

	using ClientType = EthernetClient;

private:
	uint16_t port; //!< @brief Port number for the network socket.
	std::list<int> new_clients; //!< Socket list of new connected clients.
	std::vector<int> clients; //!< @brief Socket list of connected clients.
	uint16_t max_clients; //!< @brief The maximum number of allowed clients.
	int sockfd; //!< @brief Network socket used to accept connections.

	/**
	 * @brief Accept new clients if the total of connected clients is below max_clients.
	 *
	 */
	void _accept();
};

#include "EthernetServer.cpp"

#endif
