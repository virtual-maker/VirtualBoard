/*
  CFI_ClientFirmata.h - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_ClientFirmata_h
#define CFI_ClientFirmata_h

#include "CFI_ClientFirmataDebug.h"

#include "CFI_FirmataDefines.h"

#define CFI_CLIENT_MAX_DATA_BYTES 512

#define CFI_MAX_FEATURES CFI_TOTAL_PIN_MODES + 1

#include "CFI_ClientFirmataFeature.h"
#include "CFI_DigitalInputFeature.h"
#include "CFI_DigitalOutputFeature.h"
#include "CFI_AnalogInputFeature.h"
#include "CFI_AnalogOutputFeature.h"
#include "CFI_SPIFeature.h"

extern "C" {
    // callback function types
    typedef void (*systemResetCallbackFunction)(void);
    typedef void (*stringCallbackFunction)(char*);
}

class CFI_ClientFirmata
{
public:
    /* constructors */
    CFI_ClientFirmata();
    void begin(Stream& s);
    /* update and feature functions */
    void update();
    void addFeature(CFI_ClientFirmataFeature& capability);
    byte nextInstanceNumber(byte classId);
    /* serial receive handling */
    int available(void);
    void processInput(void);
    void parse(unsigned char value);
    boolean isParsingMessage(void);
    boolean isResetting(void);
    /* serial send handling */
    void sendAnalog(byte pin, int value);
    void sendDigitalPort(byte portNumber, int portData);
    void sendString(const char* string);
    void sendString(byte command, const char* string);
    void sendSysex(byte command, byte bytec, byte* bytev);
    void write(byte c);
    void write(const uint8_t* buf, size_t size);

    /* attach & detach callback functions to messages */
    void attach(byte command, systemResetCallbackFunction newFunction);
    void attach(byte command, stringCallbackFunction newFunction);

    void attach(CFI_DigitalInputFeature& feature);
    void attach(CFI_AnalogInputFeature& feature);

    void detach(byte command);
    /* access pin config */
    byte getPinMode(byte pin);
    void setPinMode(byte pin, byte config);
    /* access pin state */
    int getPinState(byte pin);
    void setPinState(byte pin, int state);

    /* utility methods */
    void sendValueAsTwo7bitBytes(int value);
    void startSysex(void);
    void endSysex(void);

private:
    typedef struct {
        byte classId;
        byte nextInstance;
    } instanceTable_t;

    Stream* _stream;

    static const byte _instanceTableSize = 4;
    instanceTable_t _instanceTable[_instanceTableSize];

    /* input message handling */
    byte waitForData; // this flag says the next serial input will be data
    byte executeMultiByteCommand; // execute this after getting multi-byte data
    byte multiByteChannel; // channel data for multiByteCommands
    byte storedInputData[CFI_CLIENT_MAX_DATA_BYTES];//MAX_DATA_BYTES]; // multi-byte data
    /* sysex */
    boolean parsingSysex;
    int sysexBytesRead;

    boolean resetting;

    /* features handling */
    CFI_ClientFirmataFeature* features[CFI_MAX_FEATURES];
    byte numFeatures;
    CFI_DigitalInputFeature* _digitalInput;
    CFI_AnalogInputFeature* _analogInput;

    /* callback functions */
    systemResetCallbackFunction currentSystemResetCallback;
    stringCallbackFunction currentStringCallback;

    /* private methods ------------------------------ */
    void processSysexMessage(void);
    void two7bitArrayToStr(unsigned char* buffer, byte length);

    boolean featureHandleSysex(byte command, int argc, byte* argv);
    void updateFeatures();
};

#endif /* CFI_ClientFirmata_h */
