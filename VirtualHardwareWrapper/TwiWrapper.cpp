/*
  TwiWrapper.cpp - Part of the VirtualBoard project

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
#include "TwiWrapper.h"

using namespace System::Runtime::InteropServices; // Marshal
using namespace VirtualHardwareNet;


class TwiWrapperPrivate
{
  public: msclr::auto_gcroot<TwiNet^> twiNet;
};

TwiWrapper::TwiWrapper()
{
  _private = new class TwiWrapperPrivate();
  _private->twiNet = gcnew TwiNet();
}

TwiWrapper::~TwiWrapper()
{
  delete _private;
}

void TwiWrapper::begin()
{
  _private->twiNet->Begin();
}

void TwiWrapper::end()
{
  _private->twiNet->End();
}

void TwiWrapper::setAddress(unsigned char address)
{
  // not supported
}

void TwiWrapper::attachSlaveTxEvent(void(*function)(void))
{
  // not supported
}

void TwiWrapper::attachSlaveRxEvent(void(*function)(unsigned char*, int))
{
  // not supported
}

void TwiWrapper::setFrequency(unsigned int clock)
{
  _private->twiNet->SetFrequency(clock);
}

unsigned char TwiWrapper::readFrom(unsigned char address, unsigned char* rxBuffer,
                                   unsigned char quantity, unsigned char sendStop)
{
  array<unsigned char>^ data = gcnew array<unsigned char>(quantity);
  unsigned char result = _private->twiNet->ReadFrom(address, data, quantity, sendStop != 0);
  Marshal::Copy(data, 0, System::IntPtr((void *)rxBuffer), result);
  return result;
}

unsigned char TwiWrapper::writeTo(unsigned char txAddress, unsigned char* txBuffer,
                                  unsigned char txBufferLength, unsigned char wait, unsigned char sendStop)
{
  array<unsigned char>^ data = gcnew array<unsigned char>(txBufferLength);
  Marshal::Copy(System::IntPtr((void *)txBuffer), data, 0, txBufferLength);
  return _private->twiNet->WriteTo(txAddress, data, txBufferLength, wait != 0, sendStop != 0);
}

void TwiWrapper::transmit(const unsigned char* txBuffer, unsigned int quantity)
{
  // not supported
}
