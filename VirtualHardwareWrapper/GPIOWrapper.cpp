/*
  GPIOWrapper.cpp - Part of the VirtualBoard project

  The VirtualBoard library allows editing, building and debugging Arduino sketches
  in Visual C++ and Visual Studio IDE. The library emulates standard Arduino libraries
  and connects them e.g. with the real serial ports and NIC of the computer.
  Optionally, real binary and analogue I/O pins as well as I2C and SPI interfaces
  can be controlled via an IO-Warrior device.
  https://github.com/virtual-maker/VirtualBoard

  Created by Immo Wache <virtual.mkr@gmail.com>
  Copyright (c) 2022 Immo Wache. All right reserved.

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
*/

#include <msclr\auto_gcroot.h>
#include "GPIOWrapper.h"

using namespace System::Runtime::InteropServices; // Marshal
using namespace VirtualHardwareNet;

class GPIOWrapperPrivate
{
  public: msclr::auto_gcroot<GPIONet^> gpio;
};

GPIOWrapper::GPIOWrapper()
{
  _private = new GPIOWrapperPrivate();
  _private->gpio = gcnew GPIONet();
}

GPIOWrapper::~GPIOWrapper()
{
  delete _private;
}

void GPIOWrapper::begin(const char* serialNumber)
{
  _private->gpio->Begin(gcnew System::String(serialNumber));
}

void GPIOWrapper::end()
{
  _private->gpio->End();
}

void GPIOWrapper::pinMode(unsigned char pin, unsigned char mode)
{
  _private->gpio->PinMode(pin, mode);
}

unsigned char GPIOWrapper::digitalRead(unsigned char pin)
{
  unsigned char result = _private->gpio->DigitalRead(pin);
  return result;
}

void GPIOWrapper::digitalWrite(unsigned char pin, unsigned char value)
{
  _private->gpio->DigitalWrite(pin, value);
}

void GPIOWrapper::digitalWritePort(unsigned char port, unsigned char value)
{
  _private->gpio->DigitalWritePort(port, value);
}

unsigned int GPIOWrapper::analogRead(unsigned char pin)
{
  unsigned int result = _private->gpio->AnalogRead(pin);
  return result;
}

void GPIOWrapper::analogWrite(unsigned char pin, unsigned int value)
{
  _private->gpio->AnalogWrite(pin, value);
}

