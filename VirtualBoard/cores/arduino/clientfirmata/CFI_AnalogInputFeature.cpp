/*
  CFI_AnalogInputFeature.cpp - ClientFirmata library
  Copyright (C) 2018 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "CFI_AnalogInputFeature.h"

CFI_AnalogInputFeature::CFI_AnalogInputFeature(CFI_ClientFirmata &firmata) : _firmata(&firmata)
{
  for (size_t i = 0; i < 16; i++) {
    _analogPorts[i] = 0;
    _reportPorts[i] = false;
  }
  _firmata->attach(*this);
}

void CFI_AnalogInputFeature::setAnalogPort(byte analogPin, int value)
{
  if (analogPin < 16) {
    _analogPorts[analogPin] = value;
  }
}

void CFI_AnalogInputFeature::setPinMode(byte pin, int mode)
{
  // ToDo: This fits only for AVR Arduinos and direct MKRx Firmata hosts
  byte analogPin = A0 + pin;
  _firmata->setPinMode(analogPin, mode);
}

void CFI_AnalogInputFeature::reportAnalogPort(byte port)
{
  byte message[2];

  message[0] = (byte)(CFI_REPORT_ANALOG | port);
  message[1] = 1;
  _firmata->write(message, 2);

  _reportPorts[port] = true;
}

int CFI_AnalogInputFeature::getPinValue(byte analogPin)
{
  int value = 0;
  if (analogPin < 16) {
    value = _analogPorts[analogPin];
    if (!_reportPorts[analogPin]) {
      setPinMode(analogPin, CFI_PIN_MODE_ANALOG);
      reportAnalogPort(analogPin);
    }
  }
  return value;
}

boolean CFI_AnalogInputFeature::handleSysex(byte command, int argc, byte* argv)
{
  return false;
}

void CFI_AnalogInputFeature::updateFeature()
{
}
