/*
  CFI_ClientFirmataDebug.h - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_CLIENT_FIRMATA_DEBUG_h
#define CFI_CLIENT_FIRMATA_DEBUG_h

#ifdef CFI_SERIAL_DEBUG
  #define CFI_DEBUG_PRINTLN(x)  Serial.println (x)
  #define CFI_DEBUG_PRINT(x)    Serial.print (x)
#else
  #define CFI_DEBUG_PRINTLN(x)
  #define CFI_DEBUG_PRINT(x)
#endif

#endif /* CFI_CLIENT_FIRMATA_DEBUG_h */
