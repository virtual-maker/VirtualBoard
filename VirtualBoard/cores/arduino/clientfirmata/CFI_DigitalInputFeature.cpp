/*
  CFI_DigitalInputFeature.cpp - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "CFI_DigitalInputFeature.h"

CFI_DigitalInputFeature::CFI_DigitalInputFeature(CFI_ClientFirmata &firmata) : _firmata(&firmata)
{
  for (size_t i = 0; i < 16; i++) {
    _digitalPorts[i] = 0;
    _reportPorts[i] = false;
  }
  _firmata->attach(*this);
}

void CFI_DigitalInputFeature::setDigitalPort(byte port, int value)
{
  if (port < 16) {
    _digitalPorts[port] = value;
  }
}

void CFI_DigitalInputFeature::setPinMode(byte pin, int mode)
{
  _firmata->setPinMode(pin, mode);

  byte port = (pin >> 3) & 0x0F;
  if (!_reportPorts[port]) {
    reportDigitalPort(port);
  }
}

void CFI_DigitalInputFeature::reportDigitalPort(byte port)
{
  byte message[2];

  message[0] = (byte)(CFI_REPORT_DIGITAL | port);
  message[1] = 1;
  _firmata->write(message, 2);

  _reportPorts[port] = true;
}

bool CFI_DigitalInputFeature::getPinValue(byte pin)
{
  int port = (pin >> 3) & 0x0F;

  byte value = _digitalPorts[port] & (1 << (pin & 0x07));
  return value != 0;
}

boolean CFI_DigitalInputFeature::handleSysex(byte command, int argc, byte* argv)
{
  return false;
}

void CFI_DigitalInputFeature::updateFeature()
{
}
