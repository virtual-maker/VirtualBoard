/*
  IOW.cs - Part of the VirtualBoard project

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
using System.Threading.Tasks;
using Tederean.FastIOW;

namespace VirtualHardwareNet
{
    public class IOW
    {
        public static IOWarrior Device { get; private set; }

        public static void Disconnect()
        {
            if (Device != null)
            {
                FastIOW.CloseConnection();
                Device = null;
            }
        }

        public static void Connect(string serialNumber)
        {
            FastIOW.OpenConnection();

            if (!FastIOW.Connected)
            {
                FastIOW.CloseConnection();
                Console.WriteLine("No IO-Warrior detected!");
                return;
            }

            _printInfos();

            IOWarrior device;

            if (string.IsNullOrEmpty(serialNumber))
            {
                device = FastIOW.GetIOWarriors()
                    .Where(entry => entry is IOWarrior28 || entry is IOWarrior56)
                    .FirstOrDefault();
            }
            else
            {
                device = FastIOW.GetIOWarriors()
                    .Where(entry => entry.SerialNumber == serialNumber && (entry is IOWarrior28 || entry is IOWarrior56))
                    .FirstOrDefault();
            }


            if (device != null)
            {
                Console.WriteLine("\nConnected device:");
                Console.WriteLine(string.Format("|{0,20}|{1,20}|{2,20}|\n", device.Name, string.Format("0x{0:X8}", device.Id), device.SerialNumber));
            }
            else
            {
                FastIOW.CloseConnection();
                if (string.IsNullOrEmpty(serialNumber))
                {
                    Console.WriteLine("No IOWarrior-28 or IOWarrior-56 detected!");
                }
                else
                {
                    Console.WriteLine($"No IOWarrior-28 or IOWarrior-56 with serial number: {serialNumber} detected!");
                }
            }
            Device = device;
        }

        private static void _printInfos()
        {
            Console.WriteLine("\nFound device(s):");
            Console.WriteLine(string.Format("|{0,20}|{1,20}|{2,20}|", "Name", "Unique Identifier", "Serial Number"));

            foreach (var iow in FastIOW.GetIOWarriors())
            {
                Console.WriteLine(string.Format("|{0,20}|{1,20}|{2,20}|", iow.Name, string.Format("0x{0:X8}", iow.Id), iow.SerialNumber));
            }
        }
    }
}
