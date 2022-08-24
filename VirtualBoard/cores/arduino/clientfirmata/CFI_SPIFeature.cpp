/*
  CFI_SPIFeature.cpp - ClientFirmata library
  Copyright (C) 2018 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "CFI_SPIFeature.h"
#include "CFI_ClientEncoder7Bit.h"

CFI_SPIFeature::CFI_SPIFeature(CFI_ClientFirmata& firmata) : _firmata(&firmata), _channelDeviceId(0),
    _inBuffer(NULL), _isAwaitingReply(false), _numWords(0), _requestId(0)
{
    _firmata->addFeature(*this);
}

void CFI_SPIFeature::begin(byte channel)
{
    byte buffer[5]{};

    buffer[0] = CFI_START_SYSEX;
    buffer[1] = CFI_SPI_DATA;
    buffer[2] = SPI_BEGIN;
    buffer[3] = channel & 0x07;
    buffer[4] = CFI_END_SYSEX;
    _firmata->write(buffer, 5);
}

void CFI_SPIFeature::end(byte channel)
{
    byte buffer[5]{};

    buffer[0] = CFI_START_SYSEX;
    buffer[1] = CFI_SPI_DATA;
    buffer[2] = SPI_END;
    buffer[3] = channel & 0x07;
    buffer[4] = CFI_END_SYSEX;
    _firmata->write(buffer, 5);
}

void CFI_SPIFeature::deviceConfig(byte deviceId, byte channel, byte dataMode, byte bitOrder, unsigned long maxSpeed, byte csPinOptions, byte csPin)
{
    byte buffer[14]{};

    // Following both settings are hard coded
    byte use7bitEncoding = 1; // SPI_TRANSFER command expects the data in encoded "7-bit" mode
    byte wordSize = 0; // 0 = DEFAULT = 8-bits

    buffer[0] = CFI_START_SYSEX;
    buffer[1] = CFI_SPI_DATA;
    buffer[2] = SPI_DEVICE_CONFIG;
    buffer[3] = channel & 0x07 | (deviceId & 0x0F) << 3;
    buffer[4] = (use7bitEncoding & 0x01) << 3 | (dataMode & 0x03) << 1 | bitOrder & 0x01;
    buffer[5] = maxSpeed & 0x7F;
    buffer[6] = (maxSpeed >> 7) & 0x7F;
    buffer[7] = (maxSpeed >> 14) & 0x7F;
    buffer[8] = (maxSpeed >> 21) & 0x7F;
    buffer[9] = (maxSpeed >> 28) & 0x0F;
    buffer[10] = wordSize;
    buffer[11] = csPinOptions & 0x03;
    buffer[12] = csPin & 0x7F;
    buffer[13] = CFI_END_SYSEX;
    _firmata->write(buffer, 14);
}

void CFI_SPIFeature::transfer(byte deviceId, byte channel, byte requestId, bool deselectCsPin, byte* outBuffer, byte* inBuffer, byte numWords)
{
    _channelDeviceId = channel & 0x07 | (deviceId & 0x0F) << 3;
    _requestId = requestId & 0x7F;
    _numWords = numWords & 0x7F;
    _inBuffer = inBuffer;

    byte outDataSize = numWords * 8 / 7 + 1;
    byte* outData = new byte [outDataSize];

    _firmata->startSysex();
    _firmata->write(CFI_SPI_DATA);
    _firmata->write(SPI_TRANSFER);
    _firmata->write(_channelDeviceId);
    _firmata->write(_requestId);
    _firmata->write(deselectCsPin ? 1 : 0);
    _firmata->write(_numWords);

    CFI_ClientEncoder7BitClass encoder;
    encoder.startBinaryWrite(outData, 0);
    for (int i = 0; i < numWords; i++)
    {
        encoder.writeBinary(outBuffer[i]);
    }
    encoder.endBinaryWrite();
    _firmata->write(outData, outDataSize);
    _firmata->endSysex();
    
    delete[] outData;

    // Wait for SPI response from Firmata board
    _isAwaitingReply = true;
    while (_isAwaitingReply)
    {
        // ToDo: Add timeout logic
        _firmata->update();
    }
    // inBuffer was filled in handleSpiResponse(), see below
}

void CFI_SPIFeature::setPinMode(byte pin, int mode)
{
    _firmata->setPinMode(pin, mode);
}

boolean CFI_SPIFeature::handleSysex(byte command, int argc, byte* argv)
{
    switch (command) {
    case CFI_SPI_DATA:
        if (argc < 1)
        {
            CFI_DEBUG_PRINTLN(F("SPI reply: Empty message error"));
            return false;
        }
        handleSpiResponse(argv[0], argc - 1, argv + 1);
        return true;
    }
    return false;
}

void CFI_SPIFeature::handleSpiResponse(byte command, byte argc, byte* argv)
{
    switch (command) {
    case SPI_REPLY:
        handleSpiReply(argc, argv);
        break;
    default:
        CFI_DEBUG_PRINT(F("SPI reply: Unknown command: "));
        CFI_DEBUG_PRINTLN(command);
        break;
    }
}

void CFI_SPIFeature::handleSpiReply(byte argc, byte* argv)
{
    // Check for expected SPI reply
    if (!_isAwaitingReply)
	{
        CFI_DEBUG_PRINTLN(F("SPI reply: Not awaiting reply"));
		return;
	}
    _isAwaitingReply = false;

    // Check for cannel and device Id
	if (argv[0] != _channelDeviceId) {
        CFI_DEBUG_PRINT(F("SPI reply: Unknown channel/deviceId specified: "));
        CFI_DEBUG_PRINTLN(argv[0]);
		return;
	}
    // Check for request Id
    if (argv[1] != _requestId) {
        CFI_DEBUG_PRINT(F("SPI reply: Unknown channel/deviceId specified: "));
        CFI_DEBUG_PRINTLN(argv[1]);
        return;
    }
    // Check for expected data length
    if (argv[2] != _numWords) {
        CFI_DEBUG_PRINT(F("SPI reply: Wrong number of words: "));
        CFI_DEBUG_PRINTLN(argv[2]);
        return;
    }
    CFI_ClientEncoder7BitClass::readBinary(_numWords, argv + 3, _inBuffer);
}

void CFI_SPIFeature::updateFeature()
{
}
