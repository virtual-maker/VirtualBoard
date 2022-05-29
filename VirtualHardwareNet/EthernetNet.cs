/*
  EthernetNet.cs - Part of the VirtualBoard project

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
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Text;

namespace VirtualHardwareNet
{
    public class EthernetNet
    {
        public static readonly int MAX_UDP_CLIENTS = 8;
        public static readonly int MAX_CLIENTS = 8;
        public static readonly int MAX_SERVERS = 2;

        public string ErrorMessage { get; private set; }
        public string SocketErrorCode { get; private set; }
        public int ErrorCode { get; private set; }

        private EthernetUdpNet[] _udpClients = new EthernetUdpNet[MAX_UDP_CLIENTS];
        private EthernetClientNet[] _clients = new EthernetClientNet[MAX_CLIENTS];
        private EthernetServerNet[] _servers = new EthernetServerNet[MAX_SERVERS];
        private IPAddress _localIpAddress;
        IPAddress _subnetMask;
        IPAddress _gatewayIpAddress;
        IPAddress _dnsIpAddress;

        public EthernetNet()
        {
            GetIpV4Configuration(out _localIpAddress, out _subnetMask, out _gatewayIpAddress, out _dnsIpAddress);
        }

        public uint localIpAddress()
        {
            return BitConverter.ToUInt32(_localIpAddress.GetAddressBytes(), 0);
        }

        public uint dnsServerAddress()
        {
            return BitConverter.ToUInt32(_dnsIpAddress.GetAddressBytes(), 0);
        }

        public uint gatewayAddress()
        {
            return BitConverter.ToUInt32(_gatewayIpAddress.GetAddressBytes(), 0);
        }

        public uint subnetMask()
        {
            return BitConverter.ToUInt32(_subnetMask.GetAddressBytes(), 0);
        }

        public static bool GetIpV4Configuration(out IPAddress localIpAddress, out IPAddress subnetMask, out IPAddress gatewayIpAddress, out IPAddress dnsIpAddress)
        {
            // Get local IP address
            // see: https://stackoverflow.com/a/27376368

            // Get the subnet mask of a given local IP address
            // see: 
            // https://stackoverflow.com/a/27839690
            // http://www.java2s.com/Code/CSharp/Network/GetSubnetMask.htm

            // Get the gateway address of a given local IP address
            // see: https://stackoverflow.com/a/13635038

            // Get the DNS address of a given local IP address
            // https://stackoverflow.com/a/39312116

            localIpAddress = IPAddress.None;
            subnetMask = IPAddress.None;
            gatewayIpAddress = IPAddress.None;
            dnsIpAddress = IPAddress.None;

            // Get local IP address
            using (Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, 0))
            {
                try {
                    socket.Connect("8.8.8.8", 65530);
                    IPEndPoint endPoint = socket.LocalEndPoint as IPEndPoint;
                    localIpAddress = endPoint.Address;
                }
                catch (Exception) {
                    return false;
                }
            }
            foreach (NetworkInterface adapter in NetworkInterface.GetAllNetworkInterfaces()) {
                if (adapter.OperationalStatus != OperationalStatus.Up) {
                    continue;
                }
                var ipProperties = adapter.GetIPProperties();

                foreach (UnicastIPAddressInformation unicastIPAddressInformation in ipProperties.UnicastAddresses) {
                    if (unicastIPAddressInformation.Address.AddressFamily == AddressFamily.InterNetwork) {
                        if (localIpAddress.Equals(unicastIPAddressInformation.Address)) {
                            // Get subnet mask
                            subnetMask = unicastIPAddressInformation.IPv4Mask;

                            // Get gateway address
                            foreach (var gatewayIPAddressInformation in ipProperties.GatewayAddresses) {
                                if (gatewayIPAddressInformation.Address.AddressFamily == AddressFamily.InterNetwork) {
                                    gatewayIpAddress = gatewayIPAddressInformation.Address;
                                    break;
                                }
                            }

                            // Get DNS address
                            foreach (var dnsAddress in ipProperties.DnsAddresses) {
                                if (dnsAddress.AddressFamily == AddressFamily.InterNetwork) {
                                    dnsIpAddress = dnsAddress;
                                    break;
                                }
                            }
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        public EthernetUdpNet NewUdpClient(ref int socketNumber)
        {
            lock (_udpClients) {
                for (int i = 0; i < _udpClients.Length; i++) {
                    if (_udpClients[i] == null || !_udpClients[i].Allocated) {
                        if (_udpClients[i] == null) {
                            _udpClients[i] = new EthernetUdpNet();
                        }

                        _udpClients[i].Allocated = true;
                        socketNumber = i;
                        return _udpClients[i];
                    }
                }
            }

            socketNumber = -1;
            return null;
        }

        private bool _disposeUdpClient(int socketNumber)
        {
            lock (_udpClients) {
                if (_udpClients[socketNumber] != null) {
                    _udpClients[socketNumber] = null;
                    return true;
                }
            }
            return false;
        }

        private bool tryGetUdpClient(int socketNumber, out EthernetUdpNet updClient)
        {
            updClient = null;
            if (socketNumber < 0 || socketNumber >= MAX_UDP_CLIENTS) {
                return false;
            }

            updClient = _udpClients[socketNumber];
            if (updClient == null) {
                return false;
            }

            return true;
        }

        public EthernetClientNet NewClient(ref int socketNumber)
        {
            lock (_clients) {
                for (int i = 0; i < _clients.Length; i++) {
                    if (_clients[i] == null || !_clients[i].Allocated) {
                        if (_clients[i] == null) {
                            _clients[i] = new EthernetClientNet();
                        }

                        _clients[i].Allocated = true;
                        socketNumber = i;
                        return _clients[i];
                    }
                }
            }

            socketNumber = -1;
            return null;
        }

        private bool tryGetClient(int socketNumber, out EthernetClientNet client)
        {
            client = null;
            if (socketNumber < 0 || socketNumber >= MAX_CLIENTS) {
                return false;
            }

            client = _clients[socketNumber];
            if (client == null) {
                return false;
            }

            return true;
        }

        private EthernetServerNet newServer(ref int socketNumber)
        {
            for (int i = 0; i < _servers.Length; i++) {
                if (_servers[i] == null) {
                    _servers[i] = new EthernetServerNet(this);
                    socketNumber = i;
                    return _servers[i];
                }
            }
            socketNumber = -1;
            return null;
        }

        private bool tryGetServer(int socketNumber, out EthernetServerNet server)
        {
            server = null;
            if (socketNumber < 0 || socketNumber >= MAX_CLIENTS) {
                return false;
            }

            server = _servers[socketNumber];
            if (server == null) {
                return false;
            }

            return true;
        }

        public string clientErrorMessage(int socketNumber)
        {
            if (socketNumber == -1) {
                return ErrorMessage;
            }

            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.ErrorMessage;
            }

            return null;
        }

        public string clientSocketErrorCode(int socketNumber)
        {
            if (socketNumber == -1) {
                return SocketErrorCode;
            }

            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.SocketErrorCode;
            }

            return null;
        }

        public int clientErrorCode(int socketNumber)
        {
            if (socketNumber == -1) {
                return ErrorCode;
            }

            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.ErrorCode;
            }

            return -1;
        }

        public int clientConnect(string hostname, ushort port, ref int socketNumber)
        {
            EthernetClientNet client = NewClient(ref socketNumber);
            if (client != null) {
                return client.connect(hostname, port);
            }

            // all TcpClient sockets used
            setExceptionMessage(new SocketException(10024));
            return -1;
        }

        public int clientAvailable(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.available();
            }

            return 0;
        }

        public byte clientConnected(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.connected();
            }

            return 0;
        }

        public int clientPeek(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.peek();
            }

            return -1;
        }

        public void clientFlush(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                client.flush();
            }
        }

        public void clientStop(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                client.stop();
            }
        }

        public byte clientStatus(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.status();
            }

            return 0x00; // ETHERNETCLIENT_W5100_CLOSED
        }

        public void clientClose(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                client.close();
            }
        }

        public ushort clientWrite(int socketNumber, byte[] buf, ushort size)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return (ushort)client.write(buf, size);
            }

            return 0;
        }

        public int clientRead(int socketNumber, byte[] buf, ushort bytes)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.read(ref buf, bytes);
            }

            return -1;
        }

        public string clientRemoteIpAddress(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.remoteIpAddress();
            }

            return null;
        }

        public ushort clientRemotePort(int socketNumber)
        {
            EthernetClientNet client;
            if (tryGetClient(socketNumber, out client)) {
                return client.remotePort();
            }

            return 0;
        }

        public string serverErrorMessage(int socketNumber)
        {
            EthernetServerNet server;
            if (tryGetServer(socketNumber, out server)) {
                return server.ErrorMessage;
            }

            return null;
        }

        public string serverSocketErrorCode(int socketNumber)
        {
            EthernetServerNet server;
            if (tryGetServer(socketNumber, out server)) {
                return server.SocketErrorCode;
            }

            return null;
        }

        public int serverErrorCode(int socketNumber)
        {
            EthernetServerNet server;
            if (tryGetServer(socketNumber, out server)) {
                return server.ErrorCode;
            }

            return -1;
        }

        public int serverBegin(string ipAddress, ushort port, ref int socketNumber)
        {
            EthernetServerNet server = newServer(ref socketNumber);
            if (server != null) {
                return server.begin(ipAddress, port);
            }

            // all TcpClient sockets used
            setExceptionMessage(new SocketException(10024));
            return -1;
        }

        public int serverAccept(int socketNumber)
        {
            EthernetServerNet server;
            if (tryGetServer(socketNumber, out server)) {
                return server.accept();
            }

            return -1;
        }

        private void setExceptionMessage(Exception e)
        {
            ErrorMessage = e.Message;
            if (e is SocketException) {
                var socketException = e as SocketException;
                ErrorCode = socketException.ErrorCode;
                SocketErrorCode = socketException.SocketErrorCode.ToString();
            } else {
                ErrorCode = -1;
                SocketErrorCode = "Unknown Ethernet .Net Error";
            }
        }
        //----------------------

        public int udpBegin(ushort port, ref int socketNumber)
        {
            EthernetUdpNet client;
            client = NewUdpClient(ref socketNumber);
            if (client != null) {
                return client.begin(port);
            }

            // all UdpClient sockets used
            setExceptionMessage(new SocketException(10024));
            return -1;
        }

        public int udpBeginMulticast(uint ipAddress, ushort port, ref int socketNumber)
        {
            EthernetUdpNet client = NewUdpClient(ref socketNumber);
            if (client != null) {
                IPAddress multicastAddress = new IPAddress(ipAddress);
                return client.beginMulticast(multicastAddress, port);
            }

            // all UdpClient sockets used
            setExceptionMessage(new SocketException(10024));
            return -1;
        }

        public int udpBeginPacket(int socketNumber, string ipStr, ushort port)
        {
            EthernetUdpNet client;
            if (tryGetUdpClient(socketNumber, out client)) {
                return client.beginPacket(ipStr, port);
            }
            return -1;
        }

        public int udpEndPacket(int socketNumber)
        {
            EthernetUdpNet client;
            if (tryGetUdpClient(socketNumber, out client)) {
                return client.endPacket();
            }
            return -1;
        }

        public int udpParsePacket(int socketNumber, out uint remoteIpAddress, out ushort remotePort)
        {
            remoteIpAddress = 0;
            remotePort = 0;

            EthernetUdpNet client;
            if (tryGetUdpClient(socketNumber, out client)) {
                return client.parsePacket(out remoteIpAddress, out remotePort);
            }
            return -1;
        }

        public uint udpWrite(int socketNumber, byte[] buf, uint size)
        {
            EthernetUdpNet client;
            if (tryGetUdpClient(socketNumber, out client)) {
                return client.write(buf, size);
            }
            return 0;
        }

        public int udpRead(int socketNumber, ref byte[] buf, uint bytes)
        {
            EthernetUdpNet client;
            if (tryGetUdpClient(socketNumber, out client)) {
                return client.read(ref buf, bytes);
            }
            return -1;
        }

        public int udpPeek(int socketNumber)
        {
            EthernetUdpNet client;
            if (tryGetUdpClient(socketNumber, out client)) {
                return client.peek();
            }
            return -1;
        }

        public void udpClose(int socketNumber)
        {
            EthernetUdpNet client;
            if (tryGetUdpClient(socketNumber, out client)) {
                client.close();
                _disposeUdpClient(socketNumber);
            }
        }
        //----------------------
    }
}
