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

#include "GPIO.h"

GPIOWrapper gpioWrapper;

// Declare a single default instance
GPIOClass GPIO = GPIOClass();

GPIOClass::GPIOClass()
{
}

GPIOClass::GPIOClass(const GPIOClass& other)
{
}

GPIOClass::~GPIOClass()
{
}

void GPIOClass::_pinMode(uint8_t pin, uint8_t mode)
{
#if defined(VM_USE_HARDWARE)
  gpioWrapper.pinMode(pin, mode);
#endif
}

void GPIOClass::_digitalWrite(uint8_t pin, uint8_t value)
{
#if defined(VM_USE_HARDWARE)
  gpioWrapper.digitalWrite(pin, value != 0);
#endif
}

void GPIOClass::_digitalWritePort(uint8_t port, uint8_t value)
{
#if defined(VM_USE_HARDWARE)
  gpioWrapper.digitalWritePort(port, value);
#endif
}

uint8_t GPIOClass::_digitalRead(uint8_t pin)
{
#if defined(VM_USE_HARDWARE)
  return gpioWrapper.digitalRead(pin);
#else
  return 0;
#endif
}

uint16_t GPIOClass::_analogRead(uint8_t pin)
{
#if defined(VM_USE_HARDWARE)
  uint8_t firmataPin = pin - A0;
  return gpioWrapper.analogRead(firmataPin);
#else
  return 0;
#endif
}

void GPIOClass::_analogWrite(uint8_t pin, uint16_t value)
{
#if defined(VM_USE_HARDWARE)
  gpioWrapper.analogWrite(pin, value);
#endif
}

uint8_t GPIOClass::_digitalPinToInterrupt(uint8_t pin)
{
  return pin;
}

GPIOClass& GPIOClass::operator=(const GPIOClass& other)
{
  if (this != &other) {
  }
  return *this;
}
