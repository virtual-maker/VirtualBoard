/*
 *  Udp.cpp: Library to send/receive UDP packets with the Arduino ethernet shield.
 *  This version only offers minimal wrapping of socket.cpp
 *  Drop Udp.h/.cpp into the Ethernet library directory at hardware/libraries/Ethernet/
 *
 * MIT License:
 * Copyright (c) 2008 Bjoern Hartmann
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * bjoern@cs.stanford.edu 12/30/2008
 *
 * Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
 * Copyright (c) 2022 Immo Wache
 */

#include "EthernetUdp.h"

#define MAX_SOCK_NUM 4

/* Constructor */
EthernetUDP::EthernetUDP() : _sock(MAX_SOCK_NUM)
{
  _offset = 0;
  _port = 0;
  _remaining = 0;
  _remotePort = 0;
}

/* Start EthernetUDP socket, listening at local port PORT */
uint8_t EthernetUDP::begin(uint16_t port)
{
  if (_sock < MAX_SOCK_NUM) {
    stop();
  }

  _port = port;
  _remaining = 0;

  int result = ethernetWrapper.udpBegin(port, &_sock);
  if (result != 1) {
    logError("UDP begin failed.");
    _sock = MAX_SOCK_NUM;
    return 0;
  }
  return 1;
}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */
int EthernetUDP::available()
{
  return _remaining;
}

/* Release any resources being used by this EthernetUDP instance */
void EthernetUDP::stop()
{
  if (_sock == MAX_SOCK_NUM) {
    return;
  }

  ethernetWrapper.udpClose(_sock);
  _sock = MAX_SOCK_NUM;
}

int EthernetUDP::beginPacket(const char *host, uint16_t port)
{
  _offset = 0;
  int result = ethernetWrapper.udpBeginPacket(_sock, host, port);

  if (result != 1) {
    logError("UDP beginPacket failed.");
    return 0;
  }
  return 1;
}

int EthernetUDP::beginPacket(IPAddress ip, uint16_t port)
{
  return beginPacket(ip.toString().c_str(), port);
}

int EthernetUDP::endPacket()
{
  return ethernetWrapper.udpEndPacket(_sock);
}

size_t EthernetUDP::write(uint8_t byte)
{
  return write(&byte, 1);
}

size_t EthernetUDP::write(const uint8_t *buffer, size_t size)
{
  uint16_t bytes_written = ethernetWrapper.udpWrite(_sock, buffer, size);
  _offset += bytes_written;
  return bytes_written;
}

int EthernetUDP::parsePacket()
{
  unsigned int port;
  unsigned int remoteAddress;
  int size = ethernetWrapper.udpParsePacket(_sock, &remoteAddress, &port);
  if (size > 0) {
    _remoteIP = remoteAddress;
    _remotePort = port;
    _remaining = size;
    return _remaining;
  }
  return 0;
}


int EthernetUDP::read()
{
  uint8_t byte;

  if ((_remaining > 0) && read(&byte, 1) > 0) {
    // We read things without any problems
    _remaining--;
    return byte;
  }

  // If we get here, there's no data available
  return -1;
}

int EthernetUDP::read(unsigned char* buffer, size_t len)
{
  if (_remaining > 0) {
    int got;
    if (_remaining <= len) {
      // data should fit in the buffer
      got = ethernetWrapper.udpRead(_sock, buffer, _remaining);
    }
    else {
      // too much data for the buffer,
      // grab as much as will fit
      got = ethernetWrapper.udpRead(_sock, buffer, len);
    }
    if (got > 0) {
      _remaining -= got;
      return got;
    }
  }
  // If we get here, there's no data available or recv failed
  return -1;
}

int EthernetUDP::peek()
{
  // Unlike recv, peek doesn't check to see if there's any data available, so we must.
  // If the user hasn't called parsePacket yet then return nothing otherwise they
  // may get the UDP header
  if (!_remaining) {
    return -1;
  }

  return ethernetWrapper.udpPeek(_sock);
}

void EthernetUDP::flush()
{
  // TODO: we should wait for TX buffer to be emptied
}

/* Start EthernetUDP socket, listening at local port PORT */
uint8_t EthernetUDP::beginMulticast(IPAddress ip, uint16_t port)
{
  if (_sock < MAX_SOCK_NUM) {
    stop();
  }

  _port = port;
  _remaining = 0;

  int result = ethernetWrapper.udpBeginMulticast(ip, port, &_sock);
  if (result != 1) {
    logError("UDP beginMulticast failed.");
    _sock = MAX_SOCK_NUM;
    return 0;
  }
  return 1;
}


