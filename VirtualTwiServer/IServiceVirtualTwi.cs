﻿/*
  IServiceVirtualTwi.cs - Part of the VirtualBoard project

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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.ServiceModel;

namespace VirtualTwiServer
{
    [ServiceContract(CallbackContract = typeof(IServiceVirtualTwiCallback))]
    public interface IServiceVirtualTwi
    {
        //[OperationContract(IsOneWay = true)]
        [OperationContract(IsOneWay = true)]
        void Ping();

        [OperationContract]
        void init(byte address);

        [OperationContract(IsOneWay = true)]
        void disable();

        [OperationContract(IsOneWay = true)]
        void setFrequency(uint clock);

        [OperationContract]
        byte readFrom(byte address, ref byte[] rxBuffer, byte quantity, bool sendStop);

        [OperationContract]
        byte writeTo(byte txAddress, byte[] txBuffer, byte txBufferLength, bool wait, bool sendStop);

        [OperationContract(IsOneWay = true)]
        void transmit(byte[] data, uint quantity);
    }
}
