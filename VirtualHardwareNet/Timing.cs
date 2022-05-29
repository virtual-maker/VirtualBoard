/*
  Timing.cs - Part of the VirtualBoard project

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
using System.Threading;
using System.Threading.Tasks;

namespace VirtualHardwareNet
{
    public class Timing
    {
        private long _lastMicrosTicks;
        private long _lastMillisTicks;
        private uint _lastMillis;
        private uint _lastMicros;


        public Timing()
        {
            _lastMillisTicks = DateTime.UtcNow.Ticks;
            _lastMicrosTicks = DateTime.UtcNow.Ticks;
        }

        public uint Millis()
        {
            long currentTicks = DateTime.UtcNow.Ticks;
            long elapsedTicks = currentTicks - _lastMillisTicks;

            _lastMillis += (uint)(elapsedTicks / 10000);

            _lastMillisTicks = DateTime.UtcNow.Ticks;

            return _lastMillis;
        }

        public uint Micros()
        {
            long currentTicks = DateTime.UtcNow.Ticks;
            long elapsedTicks = currentTicks - _lastMicrosTicks;

            _lastMicros += (uint)(elapsedTicks / 10);

            _lastMicrosTicks = DateTime.UtcNow.Ticks;

            return _lastMicros;
        }
    }
}
