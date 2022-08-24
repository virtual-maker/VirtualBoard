/*
  CFI_SPIFeature.h - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_SPIFeature_h
#define CFI_SPIFeature_h

#include "CFI_ClientFirmata.h"
#include "CFI_ClientFirmataFeature.h"

#define SPI_BEGIN               0x00 // Initialize the SPI bus for the given channel
#define SPI_DEVICE_CONFIG       0x01
#define SPI_TRANSFER            0x02
#define SPI_WRITE               0x03
#define SPI_READ                0x04
#define SPI_REPLY               0x05
#define SPI_END                 0x06
#define SPI_WRITE_ACK           0x07

#define SPI_SEND_NO_REPLY 0
#define SPI_SEND_NORMAL_REPLY 1
#define SPI_SEND_EMPTY_REPLY 2

#define SPI_MAX_DEVICES 8
#define MAX_SPI_BUF_SIZE 32


class CFI_ClientFirmata;

class CFI_SPIFeature : public CFI_ClientFirmataFeature
{
public:
    CFI_SPIFeature(CFI_ClientFirmata& firmata);

    void begin(byte channel);
    void end(byte channel);
    void deviceConfig(byte deviceId, byte channel, byte dataMode, byte bitOrder, unsigned long maxSpeed, byte csPinOptions, byte csPin);
    void transfer(byte deviceId, byte channel, byte requestId, bool deselectCsPin, byte* outBuffer, byte* inBuffer, byte numWords);

    void setPinMode(byte pin, int mode);
    boolean handleSysex(byte command, int argc, byte* argv);
    void updateFeature();

private:
    void handleSpiResponse(byte command, byte argc, byte* argv);
    void handleSpiReply(byte argc, byte* argv);
    
    CFI_ClientFirmata* _firmata;

    boolean _isAwaitingReply;
    byte _channelDeviceId;
    byte _requestId;
    byte _numWords;
    byte* _inBuffer;
};

#endif
