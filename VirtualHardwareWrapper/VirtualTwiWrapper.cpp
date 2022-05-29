/*
  VirtualTwiWrapper.cpp - Part of the VirtualBoard project

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
#include "VirtualTwiWrapper.h"

using namespace System::Runtime::InteropServices; // Marshal
using namespace VirtualHardwareNet;


class VirtualTwiWrapperPrivate
{
  public: msclr::auto_gcroot<VirtualTwiNet^> virtualTwiNet;
};

private ref class VirtualTwiWrapperHelper
{
  public: VirtualTwiWrapper* mPtr;

  public: void OnSlaveTxEvent();
  public: void OnSlaveRxEvent(array<unsigned char>^ data, unsigned int quantity);
};

void VirtualTwiWrapperHelper::OnSlaveTxEvent()
{
  mPtr->OnSlaveTxEvent();
}

// see:
// https://social.msdn.microsoft.com/Forums/vstudio/en-US/ffe5a9e9-df86-4d93-b527-6d6ad3114ea4/ccli-managed-byte-array-to-byte-and-viceversa?forum=vcgeneral
void VirtualTwiWrapperHelper::OnSlaveRxEvent(array<unsigned char>^ data, unsigned int quantity)
{
  unsigned char* buffer = new unsigned char[data->Length];
  Marshal::Copy(data, 0, System::IntPtr(buffer), data->Length);

  mPtr->OnSlaveRxEvent(buffer, quantity);
}

VirtualTwiWrapper::VirtualTwiWrapper()
{
  VirtualTwiWrapperHelper^ helper = gcnew VirtualTwiWrapperHelper;
  helper->mPtr = this;

  _private = new class VirtualTwiWrapperPrivate();
  _private->virtualTwiNet = gcnew VirtualTwiNet();

  _private->virtualTwiNet->SlaveTxEvent += gcnew
      VirtualHardwareNet::VirtualTwiNet::SlaveTxEventDelegate(helper,
          &VirtualTwiWrapperHelper::OnSlaveTxEvent);
  _private->virtualTwiNet->SlaveRxEvent += gcnew
      VirtualHardwareNet::VirtualTwiNet::SlaveRxEventDelegate(helper,
          &VirtualTwiWrapperHelper::OnSlaveRxEvent);
}

VirtualTwiWrapper::~VirtualTwiWrapper()
{
  delete _private;
}

void VirtualTwiWrapper::begin()
{
  _private->virtualTwiNet->init();
}

void VirtualTwiWrapper::end()
{
  _private->virtualTwiNet->disable();
}

void VirtualTwiWrapper::setAddress(unsigned char address)
{
  _private->virtualTwiNet->setAddress(address);
}

void VirtualTwiWrapper::attachSlaveTxEvent(void(*function)(void))
{
  _onSlaveTransmit = (TwiOnSlaveTransmit)function;
}

void VirtualTwiWrapper::attachSlaveRxEvent(void(*function)(unsigned char*, int))
{
  _onSlaveReceive = (TwiOnSlaveReceive)function;
}

void VirtualTwiWrapper::setFrequency(unsigned int clock)
{
  _private->virtualTwiNet->setFrequency(clock);
}

unsigned char VirtualTwiWrapper::readFrom(unsigned char address, unsigned char* rxBuffer,
    unsigned char quantity, unsigned char sendStop)
{
  array<unsigned char>^ data = gcnew array<unsigned char>(quantity);
  unsigned char result = _private->virtualTwiNet->readFrom(address, data, quantity, sendStop != 0);
  Marshal::Copy(data, 0, System::IntPtr((void *)rxBuffer), result);
  return result;
}

unsigned char VirtualTwiWrapper::writeTo(unsigned char txAddress, unsigned char* txBuffer,
    unsigned char txBufferLength, unsigned char wait, unsigned char sendStop)
{
  array<unsigned char>^ data = gcnew array<unsigned char>(txBufferLength);
  Marshal::Copy(System::IntPtr((void *)txBuffer), data, 0, txBufferLength);
  return _private->virtualTwiNet->writeTo(txAddress, data, txBufferLength, wait != 0, sendStop != 0);
}

void VirtualTwiWrapper::transmit(const unsigned char* txBuffer, unsigned int quantity)
{
  array<unsigned char>^ data = gcnew array<unsigned char>(quantity);
  Marshal::Copy(System::IntPtr((void *)txBuffer), data, 0, quantity);
  return _private->virtualTwiNet->transmit(data, quantity);
}

void VirtualTwiWrapper::OnSlaveTxEvent()
{
  if (_onSlaveTransmit != NULL) {
    _onSlaveTransmit();
  }
}

void VirtualTwiWrapper::OnSlaveRxEvent(unsigned char* buffer, unsigned int quantity)
{
  if (_onSlaveReceive != NULL) {
    _onSlaveReceive(buffer, quantity);
  }
}
