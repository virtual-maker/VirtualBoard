/*
  ProcessSynchronization.cs - Part of the VirtualBoard project

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
    public class ProcessSynchronization
    {
        private const string MUTEX_NAME = "MySensors.MyVirtualSensors.ProcessSynchronization";
        private Mutex _mutex;
        private bool _mutexAcquired;

        private long _lastTicks;
        private uint _lastMillis;


        public ProcessSynchronization()
        {
            _mutex = new Mutex(false, MUTEX_NAME);
            _lastTicks = DateTime.UtcNow.Ticks;
            // acquire the mutex and stop all other Mysensor applications
            Wait(0);
        }

        public uint Wait(ushort milliseconds)
        {
            if (_mutexAcquired)
                _mutex.ReleaseMutex();

            long mutexReleasedTicks = DateTime.UtcNow.Ticks;

            // Thread.Sleep(milliseconds);
            // update _lastMillis
            // Millis();

            _mutexAcquired = false;
            try
            {
                // acquire the mutex (or timeout after 60 seconds)
                // will return false if it timed out
                _mutexAcquired = _mutex.WaitOne();
            }
            catch (AbandonedMutexException)
            {
                // abandoned mutexes are still acquired, we just need
                // to handle the exception and treat it as acquisition
                _mutexAcquired = true;
            }

            // calculate remaining milliseconds after get back of the mutex
            long mutexAcquiredTicks = DateTime.UtcNow.Ticks;
            long elapsedTicks = mutexAcquiredTicks - mutexReleasedTicks;
            int elapsedMillis = (int)(elapsedTicks / 10000);
            int remainingMillis = milliseconds - elapsedMillis;
            // if there are remaining millisecends only wait for them
            if (remainingMillis > 0)
            {
                Thread.Sleep(remainingMillis);
            }

            _lastMillis += milliseconds;
            _lastTicks = DateTime.UtcNow.Ticks;
            return Millis();
        }

        public uint Millis()
        {
            long currentTicks = DateTime.UtcNow.Ticks;
            long elapsedTicks = currentTicks - _lastTicks;

            _lastMillis += (uint)(elapsedTicks / 10000);

            _lastTicks = DateTime.UtcNow.Ticks;

            return _lastMillis;
        }
    }
}
