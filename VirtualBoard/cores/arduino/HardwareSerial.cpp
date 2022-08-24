/*
  HardwareSerial.cpp - Hardware serial library for Wiring
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#include "HardwareSerial.h"

// this next line disables the entire HardwareSerial.cpp,
// this is so I can support Attiny series and any other chip without a uart
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3)

#if !defined(VM_HWSERIAL1_PORTNAME)
#define VM_HWSERIAL1_PORTNAME "NULL"
#endif
HardwareSerial Serial1(VM_HWSERIAL1_PORTNAME);

// Public Methods //////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial(const char *deviceName)
{
  _deviceName = deviceName;
  _written = false;
}

void HardwareSerial::begin(unsigned long baud, byte config)
{
  _written = false;

  _serialInternal.setPort(_deviceName);
  _serialInternal.setBaudrate(baud);
  _serialInternal.open();
}

void HardwareSerial::end()
{
  // wait for transmission of outgoing data
  _serialInternal.flush();
  _serialInternal.close();
}

int HardwareSerial::available(void)
{
  return _serialInternal.available();
}

int HardwareSerial::peek(void)
{
  // ToDo: return _serialInternal.peek();
  return -1;
}

int HardwareSerial::read(void)
{
  uint8_t b = 0;
  if (read((unsigned char*)&b, 1) >= 0) {
    // read worked
    return b;
  } else {
    // No data available
    return -1;
  }
}

int HardwareSerial::read(uint8_t *buf, size_t bytes)
{
    return _serialInternal.read(buf, bytes);
}

int HardwareSerial::availableForWrite(void)
{
  // ToDo: add function to _serialInternal
  return 64;
}

void HardwareSerial::flush()
{
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!_written) {
    return;
  }
  _serialInternal.flushOutput();
}

size_t HardwareSerial::write(uint8_t b)
{
  return write(&b, 1);
}

size_t HardwareSerial::write(const uint8_t *buf, size_t size)
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

size_t HardwareSerial::write(const char *str)
{
  if (str == NULL) {
    return 0;
  }
  return write((const uint8_t *)str, strlen(str));
}

size_t HardwareSerial::write(const char *buffer, size_t size)
{
  if (size == 0) {
    return 0;
  }
  return write((const uint8_t *)buffer, size);
}

#endif // whole file
