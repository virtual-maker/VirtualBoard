/*
  SpiWrapper.cpp - Part of the VirtualBoard project

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
#include "SpiWrapper.h"

using namespace System::Runtime::InteropServices; // Marshal
using namespace VirtualHardwareNet;


class SpiWrapperPrivate
{
  public:
    msclr::auto_gcroot<SpiNet^> spi;
};

SpiWrapper::SpiWrapper()
{
  _private = new SpiWrapperPrivate();
  _private->spi = gcnew SpiNet();
}

SpiWrapper::~SpiWrapper()
{
  delete _private;
}

void SpiWrapper::begin()
{
  _private->spi->Begin();
}

void SpiWrapper::end()
{
  _private->spi->End();
}

void SpiWrapper::beginTransaction(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
{
  _private->spi->BeginTransaction(clock, bitOrder, dataMode);
}

void SpiWrapper::transfer(const unsigned char* tbuf, const unsigned char* rbuf,
                          unsigned int size)
{
  array<unsigned char>^ tdata = gcnew array<unsigned char>(size);
  Marshal::Copy(System::IntPtr((void *)tbuf), tdata, 0, size);
  array<unsigned char>^ rdata = gcnew array<unsigned char>(size);
  _private->spi->Transfer(tdata, rdata, size);
  Marshal::Copy(rdata, 0, System::IntPtr((void*)rbuf), size);
}
