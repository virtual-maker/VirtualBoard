/*
  VirtualBoardIncludes.h - Part of the VirtualBoard project

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

#ifndef VirtualBoardIncludes_h
#define VirtualBoardIncludes_h

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Additional includes for the Visual C++ compiler

#include "cores/arduino/Arduino.h"
#include "variants/pins_arduino.h"
#include "cores/arduino/binary.h"
#include "cores/arduino/avr/pgmspace.h"
#include "cores/arduino/utils/log.c"
#include "cores/arduino/utils/noniso.cpp"
#include "cores/arduino/SoftEeprom.cpp"
#include "cores/arduino/SerialSimulator.cpp"
#include "cores/arduino/Interrupt.cpp"
#include "cores/arduino/clientfirmata/CFI_ClientFirmata.h"
#include "cores/arduino/GPIO.cpp"
#include "cores/arduino/Arduino.cpp"
#include "cores/arduino/WString.cpp"
#include "cores/arduino/Print.cpp"
#include "cores/arduino/Printable.h"
#include "cores/arduino/Stream.cpp"
#include "cores/arduino/IPAddress.cpp"
#include "cores/arduino/HardwareSerial.cpp"
#include "cores/arduino/serial/serial.cc"
#include "cores/arduino/serial/win.cc"
#include "cores/arduino/StreamString.cpp"
#include "cores/arduino/clientfirmata/CFI_ClientFirmataIncludes.h"

#endif