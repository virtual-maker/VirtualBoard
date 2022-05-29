/*
  SerialPortWrapper.cpp - Part of the VirtualBoard project

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
#include "SerialPortWrapper.h"

using namespace System::Runtime::InteropServices; // Marshal
using namespace VirtualHardwareNet;


class SerialPortWrapperPrivate
{
public:
	msclr::auto_gcroot<SerialPortNet^> serial;
};

SerialPortWrapper::SerialPortWrapper()
{
	_private = new SerialPortWrapperPrivate();
	_private->serial = gcnew SerialPortNet();
}

SerialPortWrapper::~SerialPortWrapper()
{
	delete _private;
}

void SerialPortWrapper::begin(const char *portName, int baudRate, unsigned char config)
{
	_private->serial->begin(gcnew System::String(portName), baudRate, config);
}

int SerialPortWrapper::available()
{
	return _private->serial->available();
}

int SerialPortWrapper::peek()
{
	return _private->serial->peek();
}

void SerialPortWrapper::flush()
{
	_private->serial->flush();
}

void SerialPortWrapper::end()
{
	_private->serial->end();
}

unsigned int SerialPortWrapper::write(const unsigned char * buf, unsigned int size)
{
	array<unsigned char>^ data = gcnew array<unsigned char>(size);
	Marshal::Copy(System::IntPtr((void *)buf), data, 0, size);
	return _private->serial->write(data, size);
}

int SerialPortWrapper::read(unsigned char * buf, unsigned int bytes)
{
	array<unsigned char>^ data = gcnew array<unsigned char>(bytes);
	int result = _private->serial->read(data, bytes);
	if (result > 0) {
		Marshal::Copy(data, 0, System::IntPtr((void *)buf), result);
	}
	return result;
}
