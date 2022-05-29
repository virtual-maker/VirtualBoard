/*
  SoftwareSerial.h - Based on hardware serial library for Wiring
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

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>

#include <Stream.h>
#include <SerialPortWrapper.h>

class SoftwareSerial : public Stream
{
  private:
    SerialPortWrapper _serialWrapper;
    static SoftwareSerial *active_object;

  protected:
    const char *_deviceName;
    // Has any byte been written to the UART since begin()
    bool _written;

  public:
    SoftwareSerial(const char *win32Device);
    void begin(unsigned long baud);
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

    void listen(void)
    {
      active_object = this;
    }
    bool isListening(void)
    {
      return this == active_object;
    }

    operator bool() {
      return true;
    }
};

#include "SoftwareSerial.cpp"

#endif
