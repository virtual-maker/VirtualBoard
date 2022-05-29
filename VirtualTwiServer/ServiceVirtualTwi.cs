/*
  ServiceVirtualTwi.cs - Part of the VirtualBoard project

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
using System.ServiceModel;
using System.Timers;

namespace VirtualTwiServer
{
    // see:
    // http://stackoverflow.com/questions/8315633/turn-on-includeexceptiondetailinfaults-either-from-servicebehaviorattribute-or
    // see:
    // https://stackoverflow.com/questions/1044174/what-steps-do-i-need-to-take-to-use-wcf-callbacks
    // https://derekwill.com/2015/05/11/building-duplex-services-with-wcf/
    [ServiceBehavior(
        IncludeExceptionDetailInFaults = true, 
        InstanceContextMode = InstanceContextMode.Single,
        ConcurrencyMode = ConcurrencyMode.Multiple)]
    public class ServiceVirtualTwi : IServiceVirtualTwi
    {
        private static Dictionary<int, IServiceVirtualTwiCallback> _callbackChannels = new Dictionary<int, IServiceVirtualTwiCallback>();
        private static readonly object _sycnRoot = new object();

        private Timer _pingTimer;

        public ServiceVirtualTwi()
        {
            _pingTimer = new Timer(500);
            _pingTimer.Elapsed += _pingTimer_Elapsed;
            _pingTimer.Enabled = true;
        }

        private void _pingTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            lock (_sycnRoot)
            {
                foreach (var item in _callbackChannels.ToArray())
                {
                    var address = (byte)item.Key;
                    var callbackChannel = item.Value;
                    try
                    {
                        callbackChannel.Ping();
                    }
                    catch
                    {
                        if (address == 0)
                            Console.WriteLine("Communication with TWI master lost.");
                        else
                            Console.WriteLine("Communication with TWI slave {0} lost.", address);
                        removeCallbackChannel(callbackChannel);
                    }
                }
            }
        }

        public void Ping()
        {
            // do nothing
        }

        public void init(byte address)
        {
            try
            {
                var callbackChannel = getCallbackChannel();
                lock (_sycnRoot)
                {
                    if (isValidChannel(address, callbackChannel))
                    {
                        _callbackChannels.Add(address, callbackChannel);
                        if (address == 0)
                            Console.WriteLine("Client connected as TWI master.");
                        else
                            Console.WriteLine("Client connected as TWI slave {0}.", address);
                    }
                }
            }
            catch (Exception ex)
            {
                displayException(ex, address);
            }
        }

        public void disable()
        {
            byte address = 0;
            var callbackChannel = getCallbackChannel();
            try
            {
                lock (_sycnRoot)
                {
                    var item = findDictionaryEntry(callbackChannel);
                    if (item.Value == null)
                        return;

                    address = (byte)item.Key;
                    _callbackChannels.Remove(item.Key);

                    if (address == 0)
                        Console.WriteLine("TWI master disconnected.");
                    else
                        Console.WriteLine("TWI slave {0} disconnected.", address);
                }
            }
            catch (Exception ex)
            {
                displayException(ex, address);
            }
        }

        public void setFrequency(uint clock)
        {
            // not implemented
        }

        public byte readFrom(byte address, ref byte[] rxBuffer, byte quantity, bool sendStop)
        {
            IServiceVirtualTwiCallback callbackChannel;
            if (!_callbackChannels.TryGetValue(address, out callbackChannel))
                return 4;


            if (!clientMaintainanceSuccessful(address, callbackChannel))
                return 4;

            try
            {
                callbackChannel.SlaveTxCallback();
//                Console.WriteLine("Request data from slave: {0}", address);
                return 0;
            }
            catch (Exception ex)
            {
                displayException(ex, address);
                removeCallbackChannel(callbackChannel);
            }
            return 0;
        }

        public byte writeTo(byte address, byte[] txBuffer, byte txBufferLength, bool wait, bool sendStop)
        {
            IServiceVirtualTwiCallback callbackChannel;
            if (!_callbackChannels.TryGetValue(address, out callbackChannel))
                return 4;

            if (!clientMaintainanceSuccessful(address, callbackChannel))
                return 4;

            try
            {
                callbackChannel.SlaveRxCallback(txBuffer, txBufferLength);
//                Console.WriteLine("Send data to slave: {0}", address);
                return 0;
            }
            catch (Exception ex)
            {
                displayException(ex, address);
                removeCallbackChannel(callbackChannel);
            }

            return 4;
        }

        public void transmit(byte[] data, uint quantity)
        {
            byte address = 0;
            IServiceVirtualTwiCallback callbackChannel;
            if (!_callbackChannels.TryGetValue(address, out callbackChannel))
                return;

            if (!clientMaintainanceSuccessful(address, callbackChannel))
                return;

            try
            {
                callbackChannel.MasterRxCallback(data, quantity);
//                Console.WriteLine("Send data to TWI master.");
                return;
            }
            catch (Exception ex)
            {
                displayException(ex, address);
                removeCallbackChannel(callbackChannel);
            }
        }

        private bool isValidChannel(byte address, IServiceVirtualTwiCallback callbackChannel)
        {
            if (_callbackChannels.ContainsKey(address))
            {
                if (address == 0)
                    Console.WriteLine("TWI master is already connected.");
                else
                    Console.WriteLine("TWI slave {0} is already connected.", address);
                return false;
            }
            var item = findDictionaryEntry(callbackChannel);
            if (item.Value != null)
            {
                if (address == 0)
                    Console.WriteLine("Client is already connected as TWI master.");
                else
                    Console.WriteLine("Client is already connected as TWI slave {0}.", address);
                return false;
            }
            return true;
        }

        private static KeyValuePair<int, IServiceVirtualTwiCallback> findDictionaryEntry(IServiceVirtualTwiCallback callbackChannel)
        {
            return _callbackChannels.FirstOrDefault(kvp => kvp.Value == callbackChannel);
        }

        private static IServiceVirtualTwiCallback getCallbackChannel()
        {
            return OperationContext.Current.GetCallbackChannel<IServiceVirtualTwiCallback>();
        }

        private bool clientMaintainanceSuccessful(byte address, IServiceVirtualTwiCallback callbackChannel)
        {
            if (((ICommunicationObject)callbackChannel).State != CommunicationState.Opened)
            {
                if (address == 0)
                    Console.WriteLine("Client connection for TWI master lost.");
                else
                    Console.WriteLine("Client connection for TWI slave {0} lost.", address);

                removeCallbackChannel(callbackChannel);
                return false;
            }
            return true;
        }

        private void removeCallbackChannel(IServiceVirtualTwiCallback callbackChannel)
        {
            var item = findDictionaryEntry(callbackChannel);
            if (item.Value == null)
                return;

            var address = item.Key;
            _callbackChannels.Remove(item.Key);

            if (address == 0)
                Console.WriteLine("Client as TWI master removed.");
            else
                Console.WriteLine("Client as TWI slave {0} removed.", address);
        }

        private static void displayException(Exception ex, byte address)
        {
            if (address == 0)
                Console.WriteLine("Communication with TWI master threw exception.");
            else
                Console.WriteLine("Communication with TWI slave {0} threw exception.", address);
            Console.WriteLine("Exception Type: {0}\nDescription: {1}", ex.GetType(), ex.Message);
        }

    }
}
