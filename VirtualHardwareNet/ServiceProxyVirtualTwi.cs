/*
  ServiceProxyVirtualTwi.cs - Part of the VirtualBoard project

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
using System.ServiceModel.Description;
using System.ServiceModel;
using VirtualTwiServer;
using System.ServiceModel.Channels;
using System.Threading;

namespace VirtualHardwareNet
{
    public class ServiceProxyVirtualTwi : DuplexClientBase<IServiceVirtualTwi>, IServiceVirtualTwi
    {
        public ServiceProxyVirtualTwi(InstanceContext callbackInstance, Binding binding, EndpointAddress remoteAddress)
            : base(callbackInstance, binding, remoteAddress)
        {
        }

        public void Ping()
        {
            Channel.Ping();
        }

        public void init(byte address)
        {
            try
            {
                Channel.init(address);
            }
            catch (Exception ex)
            {
                displayExceptionAndStop(ex);
            }
        }

        public void disable()
        {
            try
            {
                Channel.disable();
            }
            catch (Exception ex)
            {
                displayExceptionAndStop(ex);
            }
        }

        public void setFrequency(uint clock)
        {
            try
            {
                Channel.setFrequency(clock);
            }
            catch (Exception ex)
            {
                displayExceptionAndStop(ex);
            }
        }

        public byte readFrom(byte address, ref byte[] rxBuffer, byte quantity, bool sendStop)
        {
            try
            {
                return Channel.readFrom(address, ref rxBuffer, quantity, sendStop);
            }
            catch (Exception ex)
            {
                displayExceptionAndStop(ex);
            }
            return 4;
        }

        public byte writeTo(byte txAddress, byte[] txBuffer, byte txBufferLength, bool wait, bool sendStop)
        {
            try
            {
                return Channel.writeTo(txAddress, txBuffer, txBufferLength, wait, sendStop);
            }
            catch (Exception ex)
            {
                displayExceptionAndStop(ex);
            }
            return 4;
        }

        public void transmit(byte[] data, uint quantity)
        {
            try
            {
                Channel.transmit(data, quantity);
            }
            catch (Exception ex)
            {
                displayExceptionAndStop(ex);
            }
        }

        private void displayExceptionAndStop(Exception ex)
        {
            Console.WriteLine("Exception Type: {0}\nDescription: {1}", ex.GetType(), ex.Message);
            while (true) { Thread.Sleep(1000); }
        }
    }
}
