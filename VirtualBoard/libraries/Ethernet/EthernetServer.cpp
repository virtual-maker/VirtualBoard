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

#include "EthernetServer.h"
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <utils/log.h>
#include "EthernetClient.h"

EthernetServer::EthernetServer(uint16_t port, uint16_t max_clients) : port(port),
	max_clients(max_clients), sockfd(-1)
{
	clients.reserve(max_clients);
}

void EthernetServer::begin()
{
	begin(IPAddress(0,0,0,0));
}

void EthernetServer::begin(IPAddress address)
{
	int result = ethernetWrapper.serverBegin(address.toString().c_str(), port, &sockfd);
	if (result == -1) {
		logError("Failed to bind server!\n");
	} else {
		logDebug("Listening for connections on %s:%u\n", address.toString().c_str(), port);
	}

	//struct addrinfo hints, *servinfo, *p;
	//int yes=1;
	//int rv;
	//char ipstr[INET_ADDRSTRLEN];
	//char portstr[6];

	//if (sockfd != -1) {
	//	close(sockfd);
	//	sockfd = -1;
	//}

	//memset(&hints, 0, sizeof hints);
	//hints.ai_family = AF_UNSPEC;
	//hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;

	//sprintf(portstr, "%d", port);
	//if ((rv = getaddrinfo(address.toString().c_str(), portstr, &hints, &servinfo)) != 0) {
	//	logError("getaddrinfo: %s\n", gai_strerror(rv));
	//	return;
	//}

	//// loop through all the results and bind to the first we can
	//for (p = servinfo; p != NULL; p = p->ai_next) {
	//	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	//		logError("socket: %s\n", strerror(errno));
	//		continue;
	//	}

	//	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	//		logError("setsockopt: %s\n", strerror(errno));
	//		freeaddrinfo(servinfo);
	//		return;
	//	}

	//	if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	//		close(sockfd);
	//		logError("bind: %s\n", strerror(errno));
	//		continue;
	//	}

	//	break;
	//}

	//if (p == NULL)  {
	//	logError("Failed to bind!\n");
	//	freeaddrinfo(servinfo);
	//	return;
	//}

	//if (listen(sockfd, ETHERNETSERVER_BACKLOG) == -1) {
	//	logError("listen: %s\n", strerror(errno));
	//	freeaddrinfo(servinfo);
	//	return;
	//}

	//freeaddrinfo(servinfo);

	//fcntl(sockfd, F_SETFL, O_NONBLOCK);

	//struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
	//void *addr = &(ipv4->sin_addr);
	//inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
	//logDebug("Listening for connections on %s:%s\n", ipstr, portstr);
}

bool EthernetServer::hasClient()
{
	// Check if any client has disconnected
	for (size_t i = 0; i < clients.size(); ++i) {
		EthernetClient client(clients[i]);
		if (!client.connected()) {
			// Checks if this disconnected client is also on the new clients list
			for (std::list<int>::iterator it = new_clients.begin(); it != new_clients.end(); ++it) {
				if (*it == clients[i]) {
					new_clients.erase(it);
					break;
				}
			}
			client.stop();
			clients[i] = clients.back();
			clients.pop_back();
			logDebug("Ethernet client disconnected.\n");
		}
	}

	_accept();

	return !new_clients.empty();
}

EthernetClient EthernetServer::available()
{
//	if (new_clients.empty()) {
	if (!hasClient()) {
		return EthernetClient();
	} else {
		int sock = new_clients.front();
		new_clients.pop_front();
		return EthernetClient(sock);
	}
}

size_t EthernetServer::write(uint8_t b)
{
	return write(&b, 1);
}

size_t EthernetServer::write(const uint8_t *buffer, size_t size)
{
	size_t n = 0;
	size_t i = 0;

	while (i < clients.size()) {
		EthernetClient client(clients[i]);
		if (client.status() == ETHERNETCLIENT_W5100_ESTABLISHED) {
			n += client.write(buffer, size);
			i++;
		}
	}

	return n;
}

size_t EthernetServer::write(const char *str)
{
	if (str == NULL) {
		return 0;
	}
	return write((const uint8_t *)str, strlen(str));
}

size_t EthernetServer::write(const char *buffer, size_t size)
{
	return write((const uint8_t *)buffer, size);
}

void EthernetServer::_accept()
{
	//int new_fd;
	//socklen_t sin_size;
	//struct sockaddr_storage client_addr;
	//char ipstr[INET_ADDRSTRLEN];

	//sin_size = sizeof client_addr;
	//new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

	int new_fd = ethernetWrapper.serverAccept(sockfd);
	if (new_fd == -1) {
		// ToDo:
		//if (errno != EAGAIN && errno != EWOULDBLOCK) {
		//	logError("accept: %s\n", strerror(errno));
		//}
		return;
	}

	if (clients.size() == max_clients) {
		// no free slots, search for a dead client
		ethernetWrapper.clientClose(new_fd);
		logDebug("Max number of ethernet clients reached.\n");
		return;
	}

	new_clients.push_back(new_fd);
	clients.push_back(new_fd);

	//void *addr = &(((struct sockaddr_in*)&client_addr)->sin_addr);
	//inet_ntop(client_addr.ss_family, addr, ipstr, sizeof ipstr);
	logDebug("New connection from %s:%u\n", ethernetWrapper.clientRemoteIpAddress(new_fd), ethernetWrapper.clientRemotePort(new_fd));
}
