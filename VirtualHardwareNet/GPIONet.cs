/*
  GPIONet.cs - Part of the VirtualBoard project

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
    public class GPIONet : IDisposable
    {
        private GPIO _gpio;

        public void Begin(string serialNumber)
        {
            IOW.Connect(serialNumber);

            _gpio = IOW.Device?.GetPeripheral<GPIO>();
            if (_gpio == null)
            {
                Console.WriteLine("No GPIO capable IO-Warrior detected!");
                return;
            }
        }

        public void End()
        {
            if (IOW.Device != null)
            {
                IOW.Disconnect();
            }
        }

        public void PinMode(byte pin, byte mode)
        {
            // From IO-Warrior 28 datasheet, 
            // V1.0.0.1, page 8:
            // Writing a 1 to a pin causes the open drain driver to
            // be turned off. The pin will be pulled high by an
            // internal pull up resistor of 4 - 8kOhm. Now the pin acts either as
            // an output with a high level, or can be used as an input.

            // This behaviour fits for modes: INPUT_PULLUP and OUTPUT.
            // IO-Warrior is unable to support mode INPUT, 
            // because the pullup resistor is always active.

            DigitalWrite(pin, 1);
        }

        public byte DigitalRead(byte pin)
        {
            if (_gpio != null)
            {
                byte iowPin = _mapPin(pin);
                if (iowPin < 255)
                {
                    return (byte)(_gpio.DigitalRead(iowPin) == PinState.LOW ? 0 : 1);
                }
            }
            return 0;
        }

        public void DigitalWrite(byte pin, byte value)
        {
            if (_gpio != null)
            {
                byte iowPin = _mapPin(pin);
                if (iowPin < 255)
                {
                    _gpio.DigitalWrite(iowPin, value == 0 ?  PinState.LOW : PinState.HIGH);
                }
            }
        }

        public void DigitalWritePort(byte port, byte value)
        {
            throw new NotImplementedException();
        }

        public ushort AnalogRead(byte pin)
        {
            throw new NotImplementedException();
        }

        public void AnalogWrite(byte pin, ushort value)
        {
            throw new NotImplementedException();
        }

        private byte _mapPin(byte pin)
        {
            if (IOW.Device is IOWarrior56)
            {
                return _mapIOWarrior56Pin(pin);
            }
            else if (IOW.Device is IOWarrior28)
            {
                return _mapIOWarrior28Pin(pin); 
            }
            return 255;
        }

        private byte _mapIOWarrior28Pin(byte pin)
        {
            switch (pin)
            {
                case 0:
                    return IOWarrior28.P0_0;
                case 1:
                    return IOWarrior28.P0_1;
                case 2:
                    return IOWarrior28.P0_2;
                case 3:
                    return IOWarrior28.P0_3;
                case 4:
                    return IOWarrior28.P0_4;
                case 5:
                    return IOWarrior28.P0_5;
                case 6:
                    return IOWarrior28.P0_6;
                case 7:
                    return IOWarrior28.P0_7;
                case 8:
                    return IOWarrior28.P1_4;
                case 9:
                    return IOWarrior28.P1_5;
                case 10:
                    return IOWarrior28.P1_6;
                case 11:
                    return IOWarrior28.P1_7;
                case 12:
                    return IOWarrior28.P3_7;
                case 13:
                    return IOWarrior28.P2_0;
                case 14: // A0
                    return IOWarrior28.P1_0;
                case 15: // A1
                    return IOWarrior28.P1_1;
                case 16: // A2
                    return IOWarrior28.P1_2;
                case 17: // A3
                    return IOWarrior28.P1_3;
                case 18:
                    return IOWarrior28.P2_1;
                default:
                    return 255;
            }
        }

        private byte _mapIOWarrior56Pin(byte pin)
        {
            switch (pin)
            {
                case 0:
                    return IOWarrior56.P2_0;
                case 1:
                    return IOWarrior56.P2_1;
                case 2:
                    return IOWarrior56.P2_2;
                case 3:
                    return IOWarrior56.P2_3;
                case 4:
                    return IOWarrior56.P5_1;
                case 5:
                    return IOWarrior56.P6_7;
                case 6:
                    return IOWarrior56.P6_0;
                case 7:
                    return IOWarrior56.P2_7;
                case 8:
                    return IOWarrior56.P3_0;
                case 9:
                    return IOWarrior56.P3_1;
                case 10:
                    return IOWarrior56.P3_2;
                case 11:
                    return IOWarrior56.P3_3;
                case 12:
                    return IOWarrior56.P3_4;
                case 13:
                    return IOWarrior56.P3_5;
                case 14: //A0
                    return IOWarrior56.P0_0;
                case 15: //A1
                    return IOWarrior56.P0_1;
                case 16: //A2
                    return IOWarrior56.P0_2;
                case 17: //A3
                    return IOWarrior56.P0_3;
                case 18: //A4
                    return IOWarrior56.P0_4;
                case 19: //A5
                    return IOWarrior56.P0_5;
                default:
                    return 255;
            }
        }

        #region IDisposable Support

        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // dispose managed state (managed objects).
                }
                // disconnect (dispose) unmanaged IOW object
                IOW.Disconnect();
                disposedValue = true;
            }
        }

        ~GPIONet()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion
    }
}
