/*
  CFI_DigitalOutputFeature.cpp - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "CFI_DigitalOutputFeature.h"

CFI_DigitalOutputFeature::CFI_DigitalOutputFeature(CFI_ClientFirmata &firmata) : _firmata(&firmata)
{
  for (size_t port = 0; port < 16; port++) {
    digitalPorts[port] = 0;
  }
}

boolean CFI_DigitalOutputFeature::handleSysex(byte command, int argc, byte* argv)
{
  return false;
}

void CFI_DigitalOutputFeature::updateFeature()
{
}

void CFI_DigitalOutputFeature::digitalWritePort(byte port, int value)
{
  byte message[3];

  message[0] = (byte)(CFI_DIGITAL_MESSAGE | port);
  message[1] = (byte)(value & 0x7F);
  message[2] = (byte)(value >> 7 & 0x01);
  _firmata->write(message, 3);
  digitalPorts[port] = value;
}

void CFI_DigitalOutputFeature::setPinMode(byte pin, int mode)
{
  _firmata->setPinMode(pin, mode);
}

void CFI_DigitalOutputFeature::setPinValue(byte pin, bool value)
{
  int port = (pin >> 3) & 0x0F;
  if (value == 0) {
    digitalPorts[port] &= ~(1 << (pin & 0x07));
  } else {
    digitalPorts[port] |= (1 << (pin & 0x07));
  }

  digitalWritePort(port, digitalPorts[port]);
}
