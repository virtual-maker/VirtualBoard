/*
  CFI_ClientFirmataFeature.h - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_ClientFirmataFeature_h
#define CFI_ClientFirmataFeature_h

class CFI_ClientFirmataFeature
{
  public:
    virtual void setPinMode(byte pin, int mode) = 0;
    virtual boolean handleSysex(byte command, int argc, byte* argv) = 0;
    virtual void updateFeature() = 0;
};

#endif
