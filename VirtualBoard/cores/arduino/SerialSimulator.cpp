/*
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * for the MySensors Arduino library
 * Copyright (C) 2013-2017 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
 * Copyright (c) 2022 Immo Wache
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <conio.h>
#include <stdio.h>
#include <clocale>
#include "SerialSimulator.h"

// Declare a single default instance
SerialSimulator Serial = SerialSimulator();

void SerialSimulator::begin(unsigned long baud, uint8_t config)
{
  (void)baud;
  setlocale(LC_ALL, "");
}

int SerialSimulator::available()
{
  if (_kbhit()) {
    return 1;
  } else {
    return 0;
  }
}

int SerialSimulator::read()
{
  if (_kbhit()) {
    return _getch();
  } else {
    return -1;
  }
  //return getchar();
}

size_t SerialSimulator::write(uint8_t b)
{
  return (size_t)::printf("%c", b);
}

size_t SerialSimulator::write(const uint8_t* buf, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		write(buf[i]);
	}
	return size;
}

int SerialSimulator::peek()
{
  return -1;
}

void SerialSimulator::flush()
{
  fflush(stdout);
}

void SerialSimulator::end()
{
  flush();
}

SerialSimulator::operator bool()
{
  return true;
}
