/*
  CFI_AnalogOutputFeature.cpp - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "CFI_AnalogOutputFeature.h"

CFI_AnalogOutputFeature::CFI_AnalogOutputFeature(CFI_ClientFirmata &firmata) : _firmata(&firmata)
{
  for (size_t i = 0; i < 16; i++) {
    _pwmPins[i] = false;
  }
}

boolean CFI_AnalogOutputFeature::handleSysex(byte command, int argc, byte* argv)
{
  return false;
}

void CFI_AnalogOutputFeature::updateFeature()
{
}

void CFI_AnalogOutputFeature::analogWritePort(byte analogPin, int value)
{
  byte message[3];

  message[0] = (byte)(CFI_ANALOG_MESSAGE | analogPin);
  message[1] = (byte)(value & 0x7F);
  message[2] = (byte)(value >> 7);
  _firmata->write(message, 3);
}

void CFI_AnalogOutputFeature::setPinMode(byte pin, int mode)
{
  _firmata->setPinMode(pin, mode);
}

void CFI_AnalogOutputFeature::setPinValue(byte analogPin, int value)
{
  int port = analogPin;
  if (port < 16) {

    if (!_pwmPins[analogPin]) {
      setPinMode(analogPin, CFI_PIN_MODE_PWM);
      _pwmPins[analogPin] = true;
    }
    analogWritePort(analogPin, value);
  }
}
