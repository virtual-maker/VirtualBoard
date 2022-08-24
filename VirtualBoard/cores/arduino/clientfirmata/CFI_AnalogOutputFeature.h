/*
  CFI_AnalogOutputFeature.h - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_AnalogOutputFeature_h
#define CFI_AnalogOutputFeature_h

#include "CFI_ClientFirmata.h"
#include "CFI_ClientFirmataFeature.h"

class CFI_ClientFirmata;

class CFI_AnalogOutputFeature: public CFI_ClientFirmataFeature
{
  public:
    CFI_AnalogOutputFeature(CFI_ClientFirmata &firmata);

    void setPinMode(byte pin, int mode);
    void setPinValue(byte pin, int value);
    void analogWritePort(byte analogPin, int value);

    boolean handleSysex(byte command, int argc, byte* argv);
    void updateFeature();

  private:
    CFI_ClientFirmata *_firmata;
	bool _pwmPins[16] = { 0 }; // pin was set to pin mode PIN_MODE_PWM
};

#endif
