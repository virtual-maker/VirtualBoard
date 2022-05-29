/*
  VirtualBoard.h - Part of the VirtualBoard project

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

#ifndef VirtualBoard_h
#define VirtualBoard_h

#include "VirtualBoardIncludes.h"

bool _repeatMainLoop = true;

// see:
// https://docs.microsoft.com/en-us/windows/console/registering-a-control-handler-function
//
BOOL WINAPI CtrlHandler(unsigned long fdwCtrlType)
{
  switch (fdwCtrlType) {
    // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
      printf("Ctrl-C event\n\n");
      break;

    // Handle the console window close signal.
    case CTRL_CLOSE_EVENT:
      break;

    case CTRL_BREAK_EVENT:
      printf("Ctrl-Break event\n\n");
      break;

    case CTRL_LOGOFF_EVENT:
      printf("Ctrl-Logoff event\n\n");
      break;

    case CTRL_SHUTDOWN_EVENT:
      printf("Ctrl-Shutdown event\n\n");
      break;

    default:
      return false;
  }
  _repeatMainLoop = false;
  return TRUE;
}

void _yield(void)
{
#if !defined(VM_USE_HARDWARE)
  // Slow down CPU usage if the main loop is short
  Sleep(10);
#endif
}

int main(int argc, char *argv[])
{
  if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
    printf("\nERROR: Could not set control handler");
    return 1;
  }

#if defined(VM_USE_HARDWARE)
#if defined(VM_HW_SERIAL_NUMBER)
  gpioWrapper.begin(VM_HW_SERIAL_NUMBER);
#else
  gpioWrapper.begin(NULL);
#endif
#endif

  setup(); // Call sketch setup

  while (_repeatMainLoop) {
    _yield();
    loop(); // Call sketch loop
  }

#if defined(VM_USE_HARDWARE)
  gpioWrapper.end();
#endif

  return 0;
}

#endif