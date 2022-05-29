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

#ifndef EthernetClient_h
#define EthernetClient_h

#include <Client.h>
#include <IPAddress.h>
#include <EthernetWrapper.h>

// State codes from W5100 library
#define ETHERNETCLIENT_W5100_CLOSED 0x00
#define ETHERNETCLIENT_W5100_LISTEN 0x14
#define ETHERNETCLIENT_W5100_SYNSENT 0x15
#define ETHERNETCLIENT_W5100_SYNRECV 0x16
#define ETHERNETCLIENT_W5100_ESTABLISHED 0x17
#define ETHERNETCLIENT_W5100_FIN_WAIT 0x18
#define ETHERNETCLIENT_W5100_CLOSING 0x1A
#define ETHERNETCLIENT_W5100_TIME_WAIT 0x1B
#define ETHERNETCLIENT_W5100_CLOSE_WAIT 0x1C
#define ETHERNETCLIENT_W5100_LAST_ACK 0x1D

/**
 * EthernetClient class
 */
class EthernetClient : public Client
{

public:
	/**
	 * @brief EthernetClient constructor.
	 */
	EthernetClient();
	/**
	* @brief EthernetClient destructor.
	*/
	~EthernetClient();
	/**
	 * @brief EthernetClient constructor.
	 *
	 * @param sock Network socket.
	 */
	explicit EthernetClient(int sock);
	/**
	 * @brief Initiate a connection with host:port.
	 *
	 * @param host name to resolve or a stringified dotted IP address.
	 * @param port to connect to.
	 * @return 1 if SUCCESS or 0 if FAILURE.
	 */
	virtual int connect(const char *host, uint16_t port);
	/**
	 * @brief Initiate a connection with ip:port.
	 *
	 * @param ip to connect to.
	 * @param port to connect to.
	 * @return 1 if SUCCESS or 0 if FAILURE.
	 */
	virtual int connect(IPAddress ip, uint16_t port);
	/**
	 * @brief Write a byte.
	 *
	 * @param b byte to write.
	 * @return 0 if FAILURE or 1 if SUCCESS.
	 */
	virtual size_t write(uint8_t b);
	/**
	 * @brief Write at most 'size' bytes.
	 *
	 * @param buf Buffer to read from.
	 * @param size of the buffer.
	 * @return 0 if FAILURE or the number of bytes sent.
	 */
	virtual size_t write(const uint8_t *buf, size_t size);
	/**
	 * @brief Write a null-terminated string.
	 *
	 * @param str String to write.
	 * @return 0 if FAILURE or number of characters sent.
	 */
	size_t write(const char *str);
	/**
	 * @brief Write at most 'size' characters.
	 *
	 * @param buffer to read from.
	 * @param size of the buffer.
	 * @return 0 if FAILURE or the number of characters sent.
	 */
	size_t write(const char *buffer, size_t size);

	virtual size_t write_P(PGM_P buf, size_t size);

	/**
	 * @brief Returns the number of bytes available for reading.
	 *
	 * @return number of bytes available.
	 */
	virtual int available();
	/**
	 * @brief Read a byte.
	 *
	 * @return -1 if no data, else the first byte available.
	 */
	virtual int read();
	/**
	 * @brief Read a number of bytes and store in a buffer.
	 *
	 * @param buf buffer to write to.
	 * @param bytes number of bytes to read.
	 * @return -1 if no data or number of read bytes.
	 */
	virtual int read(uint8_t *buf, size_t bytes);
	/**
	 * @brief Returns the next byte of the read queue without removing it from the queue.
	 *
	 * @return -1 if no data, else the first byte of incoming data available.
	 */
	virtual int peek();
	/**
	 * @brief Waits until all outgoing bytes in buffer have been sent.
	 */
	virtual void flush();
	/**
	 * @brief Close the connection gracefully.
	 *
	 * Send a FIN and wait 1s for a response. If no response close it forcefully.
	 */
	virtual void stop();
	/**
	 * @brief Connection status.
	 *
	 * @return state according to W5100 library codes.
	 */
	uint8_t status();
	/**
	 * @brief Whether or not the client is connected.
	 *
	 * Note that a client is considered connected if the connection has been closed but
	 * there is still unread data.
	 *
	 * @return 1 if the client is connected, 0 if not.
	 */
	virtual uint8_t connected();
	/**
	 * @brief Close the connection.
	 */
	void close();
	/**
	 * @brief Bind the conection to the specified local ip.
	 */
	void bind(IPAddress ip);
	/**
	 * @brief Get the internal socket file descriptor.
	 *
	 * @return an integer, that is the socket number.
	 */
	int getSocketNumber();
	/**
	 * @brief Overloaded cast operators.
	 *
	 * Allow EthernetClient objects to be used where a bool is expected.
	 */
	virtual operator bool();
	/**
	 * @brief Overloaded cast operators.
	 *
	 */
	virtual bool operator==(const bool value)
	{
		return bool() == value;
	}
	/**
	 * @brief Overloaded cast operators.
	 *
	 */
	virtual bool operator!=(const bool value)
	{
		return bool() != value;
	}
	/**
	 * @brief Overloaded cast operators.
	 *
	 */
	virtual bool operator==(const EthernetClient& rhs);
	/**
	 * @brief Overloaded cast operators.
	 *
	 */
	virtual bool operator!=(const EthernetClient& rhs)
	{
		return !this->operator==(rhs);
	};

	friend class EthernetServer;

private:
	int _sock; //!< @brief Network socket file descriptor.
};

extern EthernetWrapper ethernetWrapper;

#include "EthernetClient.cpp"

#endif
