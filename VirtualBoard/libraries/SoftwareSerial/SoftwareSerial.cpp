/*
  SoftwareSerial.cpp - Based on hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman

  Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
  Copyright (c) 2022 Immo Wache
*/

#include "SoftwareSerial.h"

//
// Statics
//
SoftwareSerial *SoftwareSerial::active_object = 0;

//
// Public Methods
//
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic)
{
  (void)receivePin;
  (void)transmitPin;
  (void)inverse_logic;

  _deviceName = VM_SWSERIAL_PORTNAME;
  _written = false;
}

void SoftwareSerial::begin(unsigned long baud)
{
  _written = false;

  _serialInternal.setPort(_deviceName);
  _serialInternal.setBaudrate(baud);
  _serialInternal.open();
  listen();
}

void SoftwareSerial::end()
{
  // wait for transmission of outgoing data
  flush();
  _serialInternal.close();
}

int SoftwareSerial::available(void)
{
  return _serialInternal.available();
}

int SoftwareSerial::peek(void)
{
  // ToDo: return _serialInternal.peek();
  return -1;
}

int SoftwareSerial::read(void)
{
  uint8_t b;
  if (read((unsigned char*)&b, 1) >= 0) {
    // read worked
    return b;
  } else {
    // No data available
    return -1;
  }
}

int SoftwareSerial::read(uint8_t *buf, size_t bytes)
{
  return _serialInternal.read(buf, bytes);
}

int SoftwareSerial::availableForWrite(void)
{
  // ToDo: add function to wrapper
  return 64;
}

void SoftwareSerial::flush()
{
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!_written) {
    return;
  }

  _serialInternal.flush();
}

size_t SoftwareSerial::write(uint8_t b)
{
  return write(&b, 1);
}

size_t SoftwareSerial::write(const uint8_t *buf, size_t size)
{
  _written = true;

  size_t bytes = 0;
  while (size > 0) {
    int rc = _serialInternal.write(buf + bytes, size);
    bytes += rc;
    size -= rc;
  }
  return bytes;
}

size_t SoftwareSerial::write(const char *str)
{
  if (str == NULL) {
    return 0;
  }
  return write((const uint8_t *)str, strlen(str));
}

size_t SoftwareSerial::write(const char *buffer, size_t size)
{
  if (size == 0) {
    return 0;
  }
  return write((const uint8_t *)buffer, size);
}

