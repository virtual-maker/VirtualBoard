/*
  CFI_ClientEncoder7Bit.cpp - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "CFI_ClientEncoder7Bit.h"

CFI_ClientEncoder7BitClass::CFI_ClientEncoder7BitClass()
{
	previous = 0;
	shift = 0;
}

void CFI_ClientEncoder7BitClass::startBinaryWrite(byte *outData, int outPos)
{
	_outData = outData;
	_outPos = outPos;

	previous = 0;
	shift = 0;
}

int CFI_ClientEncoder7BitClass::endBinaryWrite()
{
	if (shift > 0) {
		_write(previous);
	}
	return _outPos;
}

void CFI_ClientEncoder7BitClass::_write(byte data)
{
	_outData[_outPos++] = data;
}

void CFI_ClientEncoder7BitClass::writeBinary(byte data)
{
	if (shift == 0) {
		_write(data & 0x7f);
		shift++;
		previous = data >> 7;
	}
	else {
		_write(((data << shift) & 0x7f) | previous);
		if (shift == 6) {
			_write(data >> 1);
			shift = 0;
		}
		else {
			shift++;
			previous = data >> (8 - shift);
		}
	}
}

void CFI_ClientEncoder7BitClass::readBinary(int outBytes, byte *inData, byte *outData)
{
	for (int i = 0; i < outBytes; i++) {
		int j = i << 3;
		int pos = j / 7;
		byte shift = j % 7;
		outData[i] = (inData[pos] >> shift) | ((inData[pos + 1] << (7 - shift)) & 0xFF);
	}
}

CFI_ClientEncoder7BitClass CFI_ClientEncoder7Bit;
