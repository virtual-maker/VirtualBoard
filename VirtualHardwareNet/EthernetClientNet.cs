/*
  EthernetClientNet.cs - Part of the VirtualBoard project

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
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace VirtualHardwareNet
{
    public class EthernetClientNet : IDisposable
    {
        private const int _constBufferSize = 1024;
        private TcpClient _client;
//        private Thread _clientThread;
        private ByteQueue _receiveQueue = new ByteQueue();
        private ByteQueue _sendQueue = new ByteQueue();
        private readonly ManualResetEvent _mreHandleClient = new ManualResetEvent(false);

        private volatile bool _treadActive;
        private volatile bool _socketConnected = false;

        public string ErrorMessage { get; private set; }
        public string SocketErrorCode { get; private set; }
        public int ErrorCode { get; private set; }

        private volatile bool _allocated;
        public bool Allocated
        {
            get { return _allocated; }
            set { _allocated = value; }
        }

        /// <summary>
        /// EthernetClient constructor.
        /// </summary>
        public EthernetClientNet()
        {
        }

        /// <summary>
        /// Initiate a connection with host:port.
        /// </summary>
        /// <param name="host">name to resolve or a stringified dotted IP address.</param>
        /// <param name="port">to connect to.</param>
        /// <returns>1 if SUCCESS or 0 if FAILURE</returns>
        public int connect(string hostname, ushort port)
        {
            TcpClient client;
            try {
                client = new TcpClient(hostname, port);
            } catch (Exception e) {
                setExceptionMessage(e);
                return 0;
            }
            connect(client);
            return 1;
        }

        public void connect(TcpClient client)
        {
            if (client == null) {
                return;
            }

            close();

            _client = client;
            _client.NoDelay = true;
            _treadActive = true;

            // Keep this client socket allocated (after close() of any old socket connection).
            Allocated = true;

            var clientThread = new Thread(new ParameterizedThreadStart(handleClient));
            clientThread.Start(_client);
            _socketConnected = true;

            Thread.Yield();
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
                SocketErrorCode = "SocketError";
            }
        }

        private void handleClient(object obj)
        {
            NetworkStream clientStream = null;
            try {
                var client = obj as TcpClient;
                clientStream = client.GetStream();
                byte[] buffer = new byte[_constBufferSize];
                int count;
                while (_treadActive) {
                    _socketConnected = isSocketConnected(client);

                    if (clientStream.DataAvailable) {
                        int length = buffer.Length - _receiveQueue.Length;
                        count = clientStream.Read(buffer, 0, length);
                        if (count > 0) {
                            _receiveQueue.Enqueue(buffer, 0, count);
                        }
                    }

                    count = _sendQueue.Length;
                    if (count > 0) {
                        if (count > buffer.Length) {
                            count = buffer.Length;
                        }
                        _sendQueue.Dequeue(buffer, 0, count);
                        clientStream.Write(buffer, 0, count);
                    }
                    // prevent high CPU usage
                    Thread.Sleep(10);
                }
            } catch (Exception e) {
                setExceptionMessage(e);
            } finally {
                _socketConnected = false;

                // One have to close the stream before closing the connection
                // see: https://stackoverflow.com/questions/425235/how-to-properly-and-completely-close-reset-a-tcpclient-connection
                if (clientStream != null) {
                    try {
                        clientStream.Close();
                    } catch (Exception e) {
                        setExceptionMessage(e);
                    }
                }
                _mreHandleClient.Set();
            }
        }

        private bool isSocketConnected(TcpClient client)
        {
            if (client == null || !client.Connected) {
                return false;
            }

            Socket socket = client.Client;
            if (socket == null || !socket.Connected) {
                return false;
            }

            // see:
            // https://stackoverflow.com/questions/6993295/how-to-determine-if-the-tcp-is-connected-or-not
            // https://social.msdn.microsoft.com/Forums/en-US/c857cad5-2eb6-4b6c-b0b5-7f4ce320c5cd/c-how-to-determine-if-a-tcpclient-has-been-disconnected?forum=netfxnetcom
            bool socketClosed = false;

            //if (socket.Poll(01, SelectMode.SelectWrite)
            //    && socket.Poll(01, SelectMode.SelectRead)
            //    && !socket.Poll(01, SelectMode.SelectError))

            if (socket.Poll(0, SelectMode.SelectRead)) {
                byte[] buff = new byte[1];
                try {
                    socketClosed = (socket.Receive(buff, SocketFlags.Peek) == 0);
                } catch (SocketException e) {
                    var error = e.SocketErrorCode;
                    socketClosed = true;
                }
            }
            return !socketClosed;
        }

        public ushort remotePort()
        {
            // see: https://stackoverflow.com/questions/2717381/how-do-i-get-client-ip-address-using-tcpclient
            if (_client == null) {
                return 0;
            }

            var endPoint = _client.Client.RemoteEndPoint as IPEndPoint;
            if (endPoint == null) {
                return 0;
            }

            return (ushort)endPoint.Port;
        }

        public string remoteIpAddress()
        {
            // see: https://stackoverflow.com/questions/2717381/how-do-i-get-client-ip-address-using-tcpclient
            if (_client == null) {
                return null;
            }

            var endPoint = _client.Client.RemoteEndPoint as IPEndPoint;
            if (endPoint == null) {
                return null;
            }

            return endPoint.Address.ToString();
        }

        /// <summary>
        /// Write a byte.
        /// </summary>
        /// <param name="b">byte to write.</param>
        /// <returns>0 if FAILURE or 1 if SUCCESS.</returns>
        public uint write(byte b)
        {
            return write(new byte[] { b }, 1);
        }

        /// <summary>
        /// Write at most 'size' bytes.
        /// </summary>
        /// <param name="buf">Buffer to read from.</param>
        /// <param name="size">of the buffer.</param>
        /// <returns>0 if FAILURE or the number of bytes sent.</returns>
        public uint write(byte[] buf, uint size)
        {
            if (size == 0) {
                return 0;
            }

            int length = _constBufferSize - _sendQueue.Length;
            if (size > length) {
                size = (uint)length;
            }
            _sendQueue.Enqueue(buf, 0, (int)size);

            Thread.Yield();

            return size;
        }

        /// <summary>
        /// Write a string.
        /// </summary>
        /// <param name="str">String to write.</param>
        /// <returns>0 if FAILURE or number of characters sent.</returns>
        public uint write(string str)
        {
            var encoding = new ASCIIEncoding();
            byte[] buffer = encoding.GetBytes(str);
            return write(buffer, (uint)buffer.Length);
        }

        /// <summary>
        /// Returns the number of bytes available for reading.
        /// </summary>
        /// <returns>number of bytes available.</returns>
        public int available()
        {
            Thread.Yield();
            return _receiveQueue.Length;
        }

        /// <summary>
        /// Read a byte.
        /// </summary>
        /// <returns>-1 if no data, else the first byte available.</returns>
        public int read()
        {
            Thread.Yield();

            int result = -1;
            byte[] buffer = new byte[1];
            if (_receiveQueue.Length > 0) {
                if (_receiveQueue.Dequeue(buffer, 0, 1) == 1) {
                    result = buffer[0];
                }
            }

            return result;
        }

        /// <summary>
        /// Read a number of bytes and store in a buffer.
        /// </summary>
        /// <param name="buf">buffer to write to.</param>
        /// <param name="bytes">number of bytes to read.</param>
        /// <returns>-1 if no data or number of read bytes.</returns>
        public int read(ref byte[] buf, uint bytes)
        {
            Thread.Yield();

            int result = -1;
            int length = _receiveQueue.Length;
            length = bytes > length ? length : (int)bytes;
            if (length > 0) {
                result = _receiveQueue.Dequeue(buf, 0, length);
            }

            return result;
        }

        /// <summary>
        /// Returns the next byte of the read queue without removing it from the queue.
        /// </summary>
        /// <returns>-1 if no data, else the first byte of incoming data available.</returns>
        public int peek()
        {
            Thread.Yield();

            int result = -1;
            if (_receiveQueue.Length > 0) {
                result = _receiveQueue.Peek();
            }

            return result;
        }

        /// <summary>
        /// Waits until all outgoing bytes in buffer have been sent.
        /// </summary>
        public void flush()
        {
            if (_client != null) { //(_sock != -1)
                while (true) {
                    //ioctlsocket(_sock, SIOCOUTQ, &count);
                    if (_sendQueue.Length == 0 || connected() == 0) {
                        return;
                    }
                    Thread.Sleep(1);
                }
            }
        }

        /// <summary>
        /// Close the connection gracefully.
        /// Send a FIN and wait 1s for a response. If no response close it forcefully.
        /// </summary>
        public void stop()
        {
            if (_client != null) {
                _client.LingerState = new LingerOption(true, 5);
                close();
            }
        }

        /// <summary>
        /// Connection status.
        /// </summary>
        /// <returns>state according to W5100 library codes.</returns>
        public byte status()
        {
            /*
            #define ETHERNETCLIENT_W5100_CLOSED 0x00
            #define ETHERNETCLIENT_W5100_LISTEN 0x14
            #define ETHERNETCLIENT_W5100_SYNSENT 0x15
            #define ETHERNETCLIENT_W5100_SYNRECV 0x16
            #define ETHERNETCLIENT_W5100_ESTABLISHED 0x17
            #define ETHERNETCLIENT_W5100_FIN_WAIT 0x18
            #define ETHERNETCLIENT_W5100_CLOSING 0x1A
            #define ETHERNETCLIENT_W5100_TIME_WAIT 0x1B
            #define ETHERNETCLIENT_W5100_CLOSE_WAIT 0x1C
            #define ETHERNETCLIENT_W5100_LAST_ACK 0x1D
            */

            if (_client == null) {
                return 0x00;    // ETHERNETCLIENT_W5100_CLOSED
            }

            if (connected() == 1) {
                return 0x17;    // ETHERNETCLIENT_W5100_ESTABLISHED
            }

            return 0x00; // ETHERNETCLIENT_W5100_CLOSED
        }

        /// <summary>
        /// Whether or not the client is connected.
        /// Note that a client is considered connected if the connection has been closed
        /// but there is still unread data.
        /// </summary>
        /// <returns>1 if the client is connected, 0 if not.</returns>
        public byte connected()
        {
            return (byte)(_receiveQueue.Length > 0 || _socketConnected ? 1 :0);

            //if (_receiveQueue.Length > 0)
            //    return 1;

            //if (_client == null || !_client.Connected)
            //    return 0;

            //Socket socket = _client.Client;
            //if (socket == null || !socket.Connected)
            //    return 0;

            //// see: https://social.msdn.microsoft.com/Forums/en-US/c857cad5-2eb6-4b6c-b0b5-7f4ce320c5cd/c-how-to-determine-if-a-tcpclient-has-been-disconnected?forum=netfxnetcom
            //bool socketClosed = false;


            ////if (socket.Poll(01, SelectMode.SelectWrite)
            ////    && socket.Poll(01, SelectMode.SelectRead)
            ////    && !socket.Poll(01, SelectMode.SelectError))

            //if (socket.Poll(0, SelectMode.SelectRead))
            //{
            //    byte[] buff = new byte[1];
            //    try
            //    {
            //        socketClosed = (socket.Receive(buff, SocketFlags.Peek) == 0);
            //    }
            //    catch (SocketException e)
            //    {
            //        var error = e.SocketErrorCode;
            //        socketClosed = true;
            //    }
            //}
            //return (byte)(socketClosed ? 0 : 1);
        }

        /// <summary>
        /// Close the connection.
        /// </summary>
        public void close()
        {
            if (_client != null) {
                // signal and wait for thread handleClient() has finished
                _treadActive = false;
                _mreHandleClient.WaitOne();

                _client.Close();
                _client = null;

                _sendQueue.Clear();
                _receiveQueue.Clear();
            }
            Allocated = false;
        }

        /// <summary>
        /// Bind the conection to the specified local ip.
        /// </summary>
        /// <param name="ip">to connect to.</param>
        public void bind(IPAddress ip)
        {

        }

        /// <summary>
        /// Get the internal socket file descriptor.
        /// </summary>
        /// <returns>an integer, that is the socket number.</returns>
        //public int getSocketNumber()
        //{
        //    return 0;
        //}

        public void Dispose()
        {
            if (_client != null) {
                ((IDisposable)_client).Dispose();
            }
        }
    }
}
