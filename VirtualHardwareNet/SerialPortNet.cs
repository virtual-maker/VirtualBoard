/*
  SerialPortNet.cs - Part of the VirtualBoard project

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
using System.IO.Ports;
using System.Text;
using System.Threading;

namespace VirtualHardwareNet
{
    public class SerialPortNet : IDisposable
    {
        private SerialPort  _serialPort;
        private ByteQueue _receiveQueue = new ByteQueue();
        private ByteQueue _sendQueue = new ByteQueue();
        private readonly ManualResetEvent _mreHandleSerialPort = new ManualResetEvent(false);

        private volatile bool _threadActive;

        public int ErrorCode { get; private set; }

        /// <summary>
        /// SerialPortNet constructor.
        /// </summary>
        public SerialPortNet()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="portName"></param>
        /// <param name="baudRate"></param>
        /// <param name="config"></param>
        public void begin(string portName, int baudRate, byte config)
        {
            Parity parity = Parity.None;
            int dataBits = 8;
            StopBits stopBits = StopBits.One;


            SerialPort serialPort;
            try
            {
                serialPort = new SerialPort(portName, baudRate, parity, dataBits, stopBits);
                serialPort.Open();
            }
            catch ////(Exception e)
            {
                ////setExceptionMessage(e);
                return;
            }
            _serialPort = serialPort;
            _threadActive = true;

            var serialPortThread = new Thread(new ParameterizedThreadStart(handleSerialPort));
            serialPortThread.Start(_serialPort);

            Thread.Yield();
        }

        private void handleSerialPort(object obj)
        {
            try
            {
                var serialPort = obj as SerialPort;
                byte[] buffer = new byte[128];
                int count;
                while (_threadActive)
                {
                    if (serialPort.BytesToRead > 0)
                    {
                        int length = buffer.Length - _receiveQueue.Length;
                        count = serialPort.Read(buffer, 0, length);
                        if (count > 0)
                        {
                            _receiveQueue.Enqueue(buffer, 0, count);
                        }
                    }

                    count = _sendQueue.Length;
                    if (count > 0)
                    {
                        if (count > buffer.Length)
                            count = buffer.Length;
                        _sendQueue.Dequeue(buffer, 0, count);
                        serialPort.Write(buffer, 0, count);
                    }
                    // prevent high CPU usage
                    Thread.Sleep(10);
                }
            }
            catch ////(Exception e)
            {
                ////setExceptionMessage(e);
            }
            finally
            {
                _mreHandleSerialPort.Set();
            }
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
            if (size == 0)
                return 0;

            int length = 128 - _sendQueue.Length;
            if (size > length)
            {
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
            if (_receiveQueue.Length > 0)
            {
                if (_receiveQueue.Dequeue(buffer, 0, 1) == 1)
                    result = buffer[0];
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
            if (length > 0)
            {
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
            if (_receiveQueue.Length > 0)
            {
                result = _receiveQueue.Peek();
            }

            return result;
        }

        /// <summary>
        /// Waits until all outgoing bytes in buffer have been sent.
        /// </summary>
        public void flush()
        {
            if (_serialPort != null)//(_sock != -1)
            {
                while (true)
                {
                    if (_sendQueue.Length == 0)
                    {
                        return;
                    }
                    Thread.Sleep(1);
                }
            }
        }

        /// <summary>
        /// End the serial connection.
        /// </summary>
        public void end()
        {
            if (_serialPort != null)
            {
                // signal and wait for thread handleSerialPort() has finished
                _threadActive = false;
                _mreHandleSerialPort.WaitOne();

                _serialPort.Close();
                _serialPort = null;

                _sendQueue.Clear();
                _receiveQueue.Clear();
            }
        }

        public void Dispose()
        {
            if (_serialPort != null)
            {
                ((IDisposable)_serialPort).Dispose();
            }
        }
    }
}
