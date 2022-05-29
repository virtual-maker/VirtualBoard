/*
  HardwareSerial.h - Hardware serial library for Wiring
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

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman

  Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
  Copyright (c) 2022 Immo Wache
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Stream.h"
#include <SerialPortWrapper.h>

// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class HardwareSerial : public Stream
{
  private:
    SerialPortWrapper _serialWrapper;

  protected:
    const char *_deviceName;
    // Has any byte been written to the UART since begin()
    bool _written;

  public:
    HardwareSerial(const char *win32Device);
    void begin(unsigned long baud) {
      begin(baud, SERIAL_8N1);
    }
    void begin(unsigned long, uint8_t);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual int read(uint8_t *buf, size_t bytes);
    virtual int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) {
      return write((uint8_t)n);
    }
    inline size_t write(long n) {
      return write((uint8_t)n);
    }
    inline size_t write(unsigned int n) {
      return write((uint8_t)n);
    }
    inline size_t write(int n) {
      return write((uint8_t)n);
    }

    virtual size_t write(const uint8_t *buf, size_t size);
    size_t write(const char *str);
    size_t write(const char *buffer, size_t size);

    //using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() {
      return true;
    }
};

#define HAVE_HWSERIAL1
extern HardwareSerial Serial1;

#endif
