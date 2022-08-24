/*
  CFI_ClientFirmata.cpp - ClientFirmata library
  Copyright (C) 2022 Immo Wache.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

//******************************************************************************
//* Includes
//******************************************************************************

#include "CFI_ClientFirmata.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

//******************************************************************************
//* Support Functions
//******************************************************************************

/**
 * Split a 16-bit byte into two 7-bit values and write each value.
 * @param value The 16-bit value to be split and written separately.
 */
void CFI_ClientFirmata::sendValueAsTwo7bitBytes(int value)
{
  write(value & 0x7F); // LSB
  write(value >> 7 & 0x7F); // MSB
}

/**
 * A helper method to write the beginning of a Sysex message transmission.
 */
void CFI_ClientFirmata::startSysex(void)
{
  write(CFI_START_SYSEX);
}

/**
 * A helper method to write the end of a Sysex message transmission.
 */
void CFI_ClientFirmata::endSysex(void)
{
  write(CFI_END_SYSEX);
#if defined(WIN32) && defined(_DEBUG)
  _stream->flush();
#endif
}

//******************************************************************************
//* Constructors
//******************************************************************************

/**
 * The Firmata class.
 */
CFI_ClientFirmata::CFI_ClientFirmata()
{
  currentStringCallback = NULL;
  currentSystemResetCallback = NULL;
  parsingSysex = false;
  waitForData = 0;
  resetting = false;
  numFeatures = 0;

  memset(_instanceTable, 0, sizeof(_instanceTable));
}

//******************************************************************************
//* Public Methods
//******************************************************************************

/**
 * Assign the ClientFirmata stream transport.
 * @param s A reference to the Stream transport object. This can be any type of
 * transport that implements the Stream interface. Some examples include Ethernet, WiFi
 * and other UARTs on the board (Serial1, Serial2, etc).
 */
void CFI_ClientFirmata::begin(Stream &s)
{
  _stream = &s;

  // systemReset();
  resetting = true;
  write(CFI_SYSTEM_RESET);
#if defined(WIN32) && defined(_DEBUG)
  _stream->flush();
#endif
}

void CFI_ClientFirmata::update()
{
  updateFeatures();
  for (size_t i = available(); i > 0; i--) {
    processInput();
  }
}

void CFI_ClientFirmata::addFeature(CFI_ClientFirmataFeature& capability)
{
  if (numFeatures < CFI_MAX_FEATURES) {
    features[numFeatures++] = &capability;
  }
}

byte CFI_ClientFirmata::nextInstanceNumber(byte classId)
{
  byte result = 255;

  // search for already existing class ids
  for (size_t i = 0; i < _instanceTableSize; i++) {
    if (_instanceTable[i].classId == classId) {
      if (_instanceTable[i].nextInstance < 128) {
        result = (_instanceTable[i].nextInstance)++;
        return result;
      } else {
        return 255;
      }
    }
  }

  // try to add new class id
  for (size_t i = 0; i < _instanceTableSize; i++) {
    if (_instanceTable[i].classId == 0) {
      _instanceTable[i].classId = classId;
      result = _instanceTable[i].nextInstance++;
      return result;
    }
  }
  return 255;
}

//------------------------------------------------------------------------------
// Input Stream Handling

/**
 * A wrapper for Stream::available()
 * @return The number of bytes remaining in the input stream buffer.
 */
int CFI_ClientFirmata::available(void)
{
  return _stream->available();
}

void CFI_ClientFirmata::two7bitArrayToStr(unsigned char *buffer, byte sysexBytesLength)
{
  byte bufferLength = sysexBytesLength / 2;
  byte i = 1;
  byte j = 0;
  while (j < bufferLength) {
    // The string length will only be at most half the size of the
    // stored input buffer so we can decode the string within the buffer.
    buffer[j] += (buffer[i] << 7);
    i++;
    j++;
    buffer[j] = buffer[i];
    i++;
  }
  // Make sure string is null terminated. This may be the case for data
  // coming from client libraries in languages that don't null terminate
  // strings.
  if (buffer[j - 1] != '\0') {
    buffer[j] = '\0';
  }
}

/**
 * Process incoming sysex messages. Handles REPORT_FIRMWARE and STRING_DATA internally.
 * Calls callback function for STRING_DATA and all other sysex messages.
 * @private
 */
void CFI_ClientFirmata::processSysexMessage(void)
{
  switch (storedInputData[0]) { //first byte in buffer is command
    case CFI_REPORT_FIRMWARE:
      two7bitArrayToStr(&storedInputData[3], sysexBytesRead - 3);
      CFI_DEBUG_PRINT("Firmware ");
      CFI_DEBUG_PRINT((char*)&storedInputData[3]);
      CFI_DEBUG_PRINT(" version ");
      CFI_DEBUG_PRINT(storedInputData[1]);
      CFI_DEBUG_PRINT('.');
      CFI_DEBUG_PRINTLN(storedInputData[2]);
      break;
    case CFI_STRING_DATA:
      two7bitArrayToStr(&storedInputData[1], sysexBytesRead - 1);
      CFI_DEBUG_PRINT("STRING_DATA: ");
      CFI_DEBUG_PRINTLN((char*)&storedInputData[1]);
      if (currentStringCallback) {
        (*currentStringCallback)((char *)&storedInputData[1]);
      }
      break;
    default:
      featureHandleSysex(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
  }
}

/**
 * Read a single int from the input stream. If the value is not = -1, pass it on to parse(byte)
 */
void CFI_ClientFirmata::processInput(void)
{
  int inputData = _stream->read(); // this is 'int' to handle -1 when no data
  if (inputData != -1) {
    parse(inputData);
  }
}

/**
 * Parse data from the input stream.
 * @param inputData A single byte to be added to the parser.
 */
void CFI_ClientFirmata::parse(byte inputData)
{
  int command;

  if (parsingSysex) {
    if (inputData == CFI_END_SYSEX) {
      //stop sysex byte
      parsingSysex = false;
      //fire off handler function
      processSysexMessage();
    } else if (inputData > 127) {
      // malformed SYSEX telegram, stop parsing
      parsingSysex = false;
    } else {
      // check for length of SYSEX telegram
      if (sysexBytesRead == CFI_CLIENT_MAX_DATA_BYTES) {
        // malformed SYSEX telegram, stop parsing
        CFI_DEBUG_PRINT("SYSEX message too long (>");
        CFI_DEBUG_PRINT(CFI_CLIENT_MAX_DATA_BYTES);
        CFI_DEBUG_PRINTLN(")!");
        parsingSysex = false;
      } else {
        //normal data byte - add to buffer
        storedInputData[sysexBytesRead] = inputData;
        sysexBytesRead++;
      }
    }
  } else if (waitForData > 0) {
    waitForData--;
    storedInputData[waitForData] = inputData;
    if ((waitForData == 0) && executeMultiByteCommand) { // got the whole message
      switch (executeMultiByteCommand) {
        case CFI_ANALOG_MESSAGE:
          // handle received analog value from channel 0-15
          if (_analogInput) {
            _analogInput->setAnalogPort(multiByteChannel,
                                        (storedInputData[0] << 7)
                                        + storedInputData[1]);
          }
          break;
        case CFI_DIGITAL_MESSAGE:
          // handle received digital port value (14-bit) from channel 0-15
          if (_digitalInput) {
            _digitalInput->setDigitalPort(multiByteChannel,
                                          (storedInputData[0] << 7)
                                          + storedInputData[1]);
          }
          break;
        case CFI_REPORT_VERSION:
          CFI_DEBUG_PRINT("Firmata protocol version ");
          CFI_DEBUG_PRINT(storedInputData[1]);
          CFI_DEBUG_PRINT('.');
          CFI_DEBUG_PRINTLN(storedInputData[0]);
          break;
        default:
          break;
      }
      executeMultiByteCommand = 0;
    }
  } else if (inputData > 0x7F) {
    // remove channel info from command byte if less than 0xF0
    if (inputData < 0xF0) {
      command = inputData & 0xF0;
      multiByteChannel = inputData & 0x0F;
    } else {
      command = inputData;
      // commands in the 0xF* range don't use channel data
    }
    switch (command) {
      case CFI_ANALOG_MESSAGE:
      case CFI_DIGITAL_MESSAGE:
      case CFI_REPORT_VERSION:
        waitForData = 2; // two data bytes needed
        executeMultiByteCommand = command;
        break;
      case CFI_REPORT_ANALOG:
      case CFI_REPORT_DIGITAL:
        waitForData = 1; // one data byte needed
        executeMultiByteCommand = command;
        break;
      case CFI_START_SYSEX:
        parsingSysex = true;
        sysexBytesRead = 0;
        break;
      case CFI_SYSTEM_RESET:
        if (currentSystemResetCallback) {
          (*currentSystemResetCallback)();
        }
        resetting = false;
        break;
      default:
        break;
    }
  }
}

/**
 * @return Returns true if the parser is actively parsing data.
 */
boolean CFI_ClientFirmata::isParsingMessage(void)
{
  return ((waitForData > 0) | parsingSysex);
}

/**
 * @return Returns true if the SYSTEM_RESET message is being executed
 */
boolean CFI_ClientFirmata::isResetting(void)
{
  return resetting;
}

//------------------------------------------------------------------------------
// Output Stream Handling

/**
 * Send an analog message to the Firmata host application. The range of pins is limited to [0..15]
 * when using the ANALOG_MESSAGE. The maximum value of the ANALOG_MESSAGE is limited to 14 bits
 * (16384). To increase the pin range or value, see the documentation for the EXTENDED_ANALOG
 * message.
 * @param pin The analog pin to send the value of (limited to pins 0 - 15).
 * @param value The value of the analog pin (0 - 1024 for 10-bit analog, 0 - 4096 for 12-bit, etc).
 * The maximum value is 14-bits (16384).
 */
void CFI_ClientFirmata::sendAnalog(byte pin, int value)
{
  // pin can only be 0-15, so chop higher bits
  write(CFI_ANALOG_MESSAGE | (pin & 0xF));
  sendValueAsTwo7bitBytes(value);
}

/**
 * Send an 8-bit port in a single digital message (protocol v2 and later).
 * Send 14-bits in a single digital message (protocol v1).
 * @param portNumber The port number to send. Note that this is not the same as a "port" on the
 * physical microcontroller. Ports are defined in order per every 8 pins in ascending order
 * of the Arduino digital pin numbering scheme. Port 0 = pins D0 - D7, port 1 = pins D8 - D15, etc.
 * @param portData The value of the port. The value of each pin in the port is represented by a bit.
 */
void CFI_ClientFirmata::sendDigitalPort(byte portNumber, int portData)
{
  write(CFI_DIGITAL_MESSAGE | (portNumber & 0xF));
  write((byte)portData % 128); // Tx bits 0-6
  write(portData >> 7);  // Tx bits 7-13
}

/**
 * Send a sysex message where all values after the command byte are packet as 2 7-bit bytes
 * (this is not always the case so this function is not always used to send sysex messages).
 * @param command The sysex command byte.
 * @param bytec The number of data bytes in the message (excludes start, command and end bytes).
 * @param bytev A pointer to the array of data bytes to send in the message.
 */
void CFI_ClientFirmata::sendSysex(byte command, byte bytec, byte * bytev)
{
  byte i;
  startSysex();
  write(command);
  for (i = 0; i < bytec; i++) {
    sendValueAsTwo7bitBytes(bytev[i]);
  }
  endSysex();
}

/**
 * Send a string to the Firmata host application.
 * @param command Must be STRING_DATA
 * @param string A pointer to the char string
 */
void CFI_ClientFirmata::sendString(byte command, const char *string)
{
  sendSysex(command, (byte)strlen(string), (byte *)string);
}

/**
 * Send a string to the Firmata host application.
 * @param string A pointer to the char string
 */
void CFI_ClientFirmata::sendString(const char *string)
{
  sendString(CFI_STRING_DATA, string);
}

/**
 * A wrapper for Stream::write(char).
 * Write a single byte to the output stream.
 * @param c The byte to be written.
 */
void CFI_ClientFirmata::write(byte c)
{
  while (_stream->write(c) != 1) {
    Serial.print('~');
    _sleep(5);
  }
}

void CFI_ClientFirmata::write(const uint8_t *buf, size_t size)
{
  size_t sent = _stream->write(buf, size);
  size -= sent;

  while (size > 0) {
    Serial.print('~');
    _sleep(5);
    buf += sent;
    sent = _stream->write(buf, size);
    size -= sent;
  }

#if defined(WIN32) && defined(_DEBUG)
  _stream->flush();
#endif
}

void CFI_ClientFirmata::attach(CFI_DigitalInputFeature & feature)
{
  _digitalInput = &feature;
}

void CFI_ClientFirmata::attach(CFI_AnalogInputFeature & feature)
{
  _analogInput = &feature;
}

/**
 * Attach a callback function for the SYSTEM_RESET command.
 * @param command Must be set to SYSTEM_RESET or it will be ignored.
 * @param newFunction A reference to the system reset callback function to attach.
 */
void CFI_ClientFirmata::attach(byte command, systemResetCallbackFunction newFunction)
{
  switch (command) {
    case CFI_SYSTEM_RESET: currentSystemResetCallback = newFunction; break;
  }
}

/**
 * Attach a callback function for the STRING_DATA command.
 * @param command Must be set to STRING_DATA or it will be ignored.
 * @param newFunction A reference to the string callback function to attach.
 */
void CFI_ClientFirmata::attach(byte command, stringCallbackFunction newFunction)
{
  switch (command) {
    case CFI_STRING_DATA: currentStringCallback = newFunction; break;
  }
}

/**
 * Detach a callback function for a specified command (such as SYSTEM_RESET, STRING_DATA,
 * ANALOG_MESSAGE, DIGITAL_MESSAGE, etc).
 * @param command The ID of the command to detatch the callback function from.
 */
void CFI_ClientFirmata::detach(byte command)
{
  switch (command) {
    case CFI_SYSTEM_RESET: currentSystemResetCallback = NULL; break;
    case CFI_STRING_DATA: currentStringCallback = NULL; break;
  }
}

/**
 * @param pin The pin to get the configuration of.
 * @return The configuration of the specified pin.
 */
byte CFI_ClientFirmata::getPinMode(byte pin)
{
  return 0; // pinConfig[pin];
}

/**
 * Set the pin mode/configuration. The pin configuration (or mode) in Firmata represents the
 * current function of the pin. Examples are digital input or output, analog input, pwm, i2c,
 * serial (uart), etc.
 * @param pin The pin to configure.
 * @param config The configuration value for the specified pin.
 */
void CFI_ClientFirmata::setPinMode(byte pin, byte config)
{
  byte message[3]{};

  message[0] = CFI_SET_PIN_MODE; //(byte)(SET_PIN_MODE);
  message[1] = pin;
  message[2] = config;
  write(message, 3);
}

//******************************************************************************
//* Private Methods
//******************************************************************************

boolean CFI_ClientFirmata::featureHandleSysex(byte command, int argc, byte * argv)
{
  for (byte i = 0; i < numFeatures; i++) {
    if (features[i]->handleSysex(command, argc, argv)) {
      return true;
    }
  }
  CFI_DEBUG_PRINT("Unhandled sysex command ");
  CFI_DEBUG_PRINTLN((char)command);
  return false;
}

void CFI_ClientFirmata::updateFeatures()
{
  for (byte i = 0; i < numFeatures; i++) {
    features[i]->updateFeature();
  }
}
