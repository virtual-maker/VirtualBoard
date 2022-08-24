/*
  CFI_ClientEncoder7Bit.h - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_ClientEncoder7Bit_h
#define CFI_ClientEncoder7Bit_h

//#include <Arduino.h>

#define num7BitOutbytes(a)(((a)*7)>>3)

class CFI_ClientEncoder7BitClass
{
public:
	CFI_ClientEncoder7BitClass();
	void startBinaryWrite(byte *outData, int outPos);
	int endBinaryWrite();
	void writeBinary(byte data);
	static void readBinary(int outBytes, byte *inData, byte *outData);

private:
	void _write(byte data);

	byte previous;
	int shift;
	byte *_outData = NULL;
	int _outPos = 0;
};

extern CFI_ClientEncoder7BitClass CFI_ClientEncoder7Bit;

#endif
