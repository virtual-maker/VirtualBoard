/*
  TwiNet.cs - Part of the VirtualBoard project

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
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using Tederean.FastIOW;

namespace VirtualHardwareNet
{
    public class TwiNet
    {
        private I2C _i2c;

        public TwiNet()
        {
        }

        public void Begin()
        {
            _i2c = IOW.Device?.GetPeripheral<I2C>();
            if (_i2c == null)
            {
                Console.WriteLine("No I2C capable IO-Warrior connected!");
                return;
            }
            _i2c.Enable();
        }

        public void End()
        {
            if (_i2c != null)
            {
                _i2c.Disable();
            }
        }

        public void SetFrequency(uint clock)
        {
            if (_i2c != null)
            {
                // ToDo: not yet implemented
            }
        }

        public byte ReadFrom(byte address, ref byte[] rxBuffer, byte quantity, bool sendStop)
        {
            if (_i2c != null)
            {
                rxBuffer = _i2c.ReadBytes(address, quantity);
                return (byte)rxBuffer.Length;
            }
            return 0;
        }

        /// <summary>
        /// Transmits the bytes from txBuffer to I2C device with address.
        /// </summary>
        /// <param name="txAddress">The address of the I2C device</param>
        /// <param name="txBuffer">bytes array to send</param>
        /// <param name="txBufferLength">number of bytes in array</param>
        /// <param name="wait">allow the device to send wait state</param>
        /// <param name="sendStop">send a stop bit after data transmission</param>
        /// <returns>
        /// Returns byte, which indicates the status of the transmission:
        /// 0:success
        /// 1:data too long to fit in transmit buffer
        /// 2:received NACK on transmit of address
        /// 3:received NACK on transmit of data
        /// 4:other error
        ///</returns>
        public byte WriteTo(byte txAddress, byte[] txBuffer, byte txBufferLength, bool wait, bool sendStop)
        {
            if (_i2c != null)
            {
                try
                {
                    _i2c.WriteBytes(txAddress, txBuffer);
                }
                catch (ArgumentException)
                {
                    return 1;
                }
                catch (IOException)
                {
                    return 3;
                }
                catch (InvalidOperationException)
                {
                    return 4;
                }
                return 0; // success
            }
            return 4;
        }
    }
}
