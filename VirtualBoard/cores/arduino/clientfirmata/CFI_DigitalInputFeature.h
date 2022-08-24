/*
  CFI_DigitalInputFeature.h - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_DigitalInputFeature_h
#define CFI_DigitalInputFeature_h

#include "CFI_ClientFirmata.h"
#include "CFI_ClientFirmataFeature.h"

class CFI_ClientFirmata;

class CFI_DigitalInputFeature : public CFI_ClientFirmataFeature
{
  public:
    CFI_DigitalInputFeature(CFI_ClientFirmata &firmata);

    void setPinMode(byte pin, int mode);
    bool getPinValue(byte pin);
    void setDigitalPort(byte port, int value);

    boolean handleSysex(byte command, int argc, byte* argv);
    void updateFeature();

  private:
    CFI_ClientFirmata *_firmata;
	int _digitalPorts[16] = { 0 }; // all binary input ports
	bool _reportPorts[16] = { false }; // 1 = report this port, 0 = silence

    void reportDigitalPort(byte port);
};

#endif
