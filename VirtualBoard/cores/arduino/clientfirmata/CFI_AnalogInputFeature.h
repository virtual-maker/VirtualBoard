/*
  CFI_AnalogInputFeature.h - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_AnalogInputFeature_h
#define CFI_AnalogInputFeature_h

#include "CFI_ClientFirmata.h"
#include "CFI_ClientFirmataFeature.h"

class CFI_ClientFirmata;

class CFI_AnalogInputFeature: public CFI_ClientFirmataFeature
{
  public:
    CFI_AnalogInputFeature(CFI_ClientFirmata &firmata);

    void setPinMode(byte pin, int mode);
    int getPinValue(byte analogPin);
    void setAnalogPort(byte analogPin, int value);

    boolean handleSysex(byte command, int argc, byte* argv);
    void updateFeature();

  private:
    CFI_ClientFirmata *_firmata;
	int _analogPorts[16] = { 0 }; // all analog input ports (one signal pin is one port)
	bool _reportPorts[16] = { 0 }; // 1 = report this port, 0 = silence

    void reportAnalogPort(byte port);
};

#endif
