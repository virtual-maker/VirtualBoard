/*
  VirtualTwiNet.cs - Part of the VirtualBoard project

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
using System.ServiceModel;
using System.Text;
using System.Threading;
using VirtualTwiServer;

namespace VirtualHardwareNet
{
    public class VirtualTwiNet
    {
        private static string ServiceEndpointUri = "net.pipe://localhost/mkr-virtual-hardware/twi-server";

        public delegate void SlaveTxEventDelegate();
        public delegate void SlaveRxEventDelegate(byte[] data, uint quantity);

        public event SlaveTxEventDelegate SlaveTxEvent;
        public event SlaveRxEventDelegate SlaveRxEvent;

        private ServiceProxyVirtualTwi _proxy;
        private byte _address;
        private List<byte> _rxBuffer = new List<byte>();

        public VirtualTwiNet()
        {
            //Thread.Sleep(10000);
        }

        private bool connectProxy()
        {
            try
            {
                var serviceCallback = new ServiceVirtualTwiCallback();
                serviceCallback.SlaveRxEvent += ServiceCallback_SlaveRxEvent;
                serviceCallback.SlaveTxEvent += ServiceCallback_SlaveTxEvent;
                serviceCallback.MasterRxEvent += ServiceCallback_MasterRxEvent;

                var instanceContext = new InstanceContext(serviceCallback);
                var binding = new NetNamedPipeBinding();
                var remoteAddress = new EndpointAddress(ServiceEndpointUri);
                tryOpenProxy(instanceContext, binding, remoteAddress);
                return true;
            }
            catch (Exception ex)
            {
                displayExceptionAndStop(ex);
            }
            return false;
        }

        private void tryOpenProxy(InstanceContext instanceContext, NetNamedPipeBinding binding, EndpointAddress remoteAddress)
        {
            var retries = 3;
            for (int i = 0; i < retries; i++)
            {
                try
                {
                    _proxy = new ServiceProxyVirtualTwi(instanceContext, binding, remoteAddress);
                    _proxy.Open();

                    // test if service is alive
                    _proxy.Ping();

                    return;
                }
                catch
                {
                    _proxy.Abort();
                    _proxy = null;
                    Thread.Sleep(1000);
                    continue;
                }
            }
            _proxy = new ServiceProxyVirtualTwi(instanceContext, binding, remoteAddress);
            _proxy.Open();
        }

        private void ServiceCallback_MasterRxEvent(byte[] data, uint quantity)
        {
            _rxBuffer.AddRange(data);
        }

        private void ServiceCallback_SlaveRxEvent(byte[] data, uint quantity)
        {
            if (SlaveRxEvent != null)
                SlaveRxEvent(data, quantity);
        }

        private void ServiceCallback_SlaveTxEvent()
        {
            if (SlaveTxEvent != null)
                SlaveTxEvent();
        }

        public void init()
        {
            if (_proxy == null)
                connectProxy();

            _proxy.init(_address);
        }

        public void setAddress(byte address)
        {
            _address = address;
        }

        public void disable()
        {
            getProxy().disable();
            _address = 0;
        }

        public void setFrequency(uint clock)
        {
            getProxy().setFrequency(clock);
        }

        public byte readFrom(byte address, ref byte[] rxBuffer, byte quantity, bool sendStop)
        {
            _rxBuffer.Clear();

            var res = getProxy().readFrom(address, ref rxBuffer, quantity, sendStop);

            var loops = 0;
            while (_rxBuffer.Count < quantity && loops < 10)
            {
                Thread.Sleep(10);
                loops++;
            }

            var bufferCount = _rxBuffer.Count;
            bufferCount = bufferCount > quantity ? quantity : bufferCount;

            for (int i = 0; i < bufferCount; i++)
            {
                rxBuffer[i] = _rxBuffer[i];
            }
            return quantity;
        }

        public byte writeTo(byte txAddress, byte[] txBuffer, byte txBufferLength, bool wait, bool sendStop)
        {
            return getProxy().writeTo(txAddress, txBuffer, txBufferLength, wait, sendStop);
        }

        public void transmit(byte[] data, uint quantity)
        {
            getProxy().transmit(data, quantity);
        }

        private ServiceProxyVirtualTwi getProxy()
        {
            if (_proxy == null)
            {
                displayExceptionAndStop(new InvalidOperationException("Wire is not initialized, call Wire.begin() first."));
            }
            return _proxy;
        }

        private void displayExceptionAndStop(Exception ex)
        {
            Console.WriteLine("Exception Type: {0}\nDescription: {1}", ex.GetType(), ex.Message);

            Console.WriteLine("Stack Trace:");
            Console.WriteLine(ex.StackTrace.ToString());

            while (true) { Thread.Sleep(1000); }
        }
    }
}
