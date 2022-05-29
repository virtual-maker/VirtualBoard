/*
  EthernetWrapper.h - Part of the VirtualBoard project

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

#pragma once

class EthernetWrapperPrivate;

class __declspec(dllexport) EthernetWrapper
{
private: EthernetWrapperPrivate* _private;

public: EthernetWrapper();

public: ~EthernetWrapper();

//---------------------------------------------

public: unsigned int localIpAddress();

public: unsigned int dnsServerAddress();

public: unsigned int gatewayAddress();

public: unsigned int subnetMask();

//---------------------------------------------

public: const char* clientErrorMessage(int socketNumber);

public: const char* clientSocketErrorCode(int socketNumber);

public: long clientErrorCode(int socketNumber);

public: int clientConnect(const char *hostname, unsigned int port, int *socketNumber);

public: int clientAvailable(int socketNumber);

public: unsigned char clientConnected(int socketNumber);

public: int clientPeek(int socketNumber);

public: void clientFlush(int socketNumber);

public: void clientStop(int socketNumber);

public: unsigned char clientStatus(int socketNumber);

public: void clientClose(int socketNumber);

public: unsigned int clientWrite(int socketNumber, const unsigned char *buf, unsigned int size);

public: int clientRead(int socketNumber, unsigned char *buf, unsigned int bytes);

public: const char* clientRemoteIpAddress(int socketNumber);

public: unsigned int clientRemotePort(int socketNumber);

//---------------------------------------------

public: const char* serverErrorMessage(int socketNumber);

public: const char* serverSocketErrorCode(int socketNumber);

public: long serverErrorCode(int socketNumber);

public: int serverBegin(const char *ipAddress, unsigned int port, int *socketNumber);

public: int serverAccept(int socketNumber);

// ---------------------------------------------

public: int udpBegin(unsigned int port, int *socketNumber);

public: int udpBeginMulticast(unsigned int ipAddress, unsigned int port, int* socketNumber);

public: int udpBeginPacket(int socketNumber, const char *hostname, unsigned int port);

public: int udpEndPacket(int socketNumber);

public: int udpParsePacket(int socketNumber, unsigned int *remoteIpAddress, unsigned int *remotePort);

public: unsigned int udpWrite(int socketNumber, const unsigned char *buf, unsigned int size);

public: int udpRead(int socketNumber, unsigned char *buf, unsigned int bytes);

public: int udpPeek(int socketNumber);

public: void udpClose(int socketNumber);
		
};
