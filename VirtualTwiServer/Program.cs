/*
  Program.cs - Part of the VirtualBoard project

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

namespace VirtualTwiServer
{
    class Program
    {
        private ServiceHost _host;

        static void Main(string[] args)
        {
            Console.Title = "Virtual MKR Hardware Simulation (TWI)";

            var program = new Program();
            program.runServiceHost();
        }

        private void runServiceHost()
        {
            var service = new ServiceVirtualTwi();

            var uri = new Uri("net.pipe://localhost/mkr-virtual-hardware/");
            var binding = new NetNamedPipeBinding();
            _host = new ServiceHost(service, uri);

            // required for singleton instance; see:
            // http://stackoverflow.com/questions/8902203/programmatically-set-instancecontextmode
            //((ServiceBehaviorAttribute)_host.Description.Behaviors[typeof(ServiceBehaviorAttribute)]).InstanceContextMode = InstanceContextMode.Single;

            try
            {
                _host.AddServiceEndpoint(typeof(IServiceVirtualTwi), binding, "twi-server");
                _host.Open();

                Console.WriteLine("Service started. Available in following endpoints:");
                foreach (var serviceEndpoint in _host.Description.Endpoints)
                    Console.WriteLine(serviceEndpoint.ListenUri.AbsoluteUri);

                Console.WriteLine("Press <ENTER> to terminate service.");
                Console.WriteLine();
                Console.ReadLine();

                // Close the ServiceHostBase to shutdown the service.
                _host.Close();
            }
            catch (CommunicationException ce)
            {
                Console.WriteLine("An exception occurred: {0}", ce.Message);
                _host.Abort();
            }
        }
    }
}
