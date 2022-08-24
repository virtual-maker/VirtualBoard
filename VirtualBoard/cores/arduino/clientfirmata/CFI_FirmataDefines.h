/*
  CFI_FirmataDefines.h
  Copyright (c) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (C) 2009-2016 Jeff Hoefs.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef CFI_FirmataDefines_h
#define CFI_FirmataDefines_h

#include "CFI_FirmataConstants.h"

// message command bytes (128-255/0x80-0xFF)

#ifdef CFI_DIGITAL_MESSAGE
#undef CFI_DIGITAL_MESSAGE
#endif
#define CFI_DIGITAL_MESSAGE          clientFirmataInternal::DIGITAL_MESSAGE // send data for a digital port (collection of 8 pins)

#ifdef CFI_ANALOG_MESSAGE
#undef CFI_ANALOG_MESSAGE
#endif
#define CFI_ANALOG_MESSAGE           clientFirmataInternal::ANALOG_MESSAGE // send data for an analog pin (or PWM)

#ifdef CFI_REPORT_ANALOG
#undef CFI_REPORT_ANALOG
#endif
#define CFI_REPORT_ANALOG            clientFirmataInternal::REPORT_ANALOG // enable analog input by pin #

#ifdef CFI_REPORT_DIGITAL
#undef CFI_REPORT_DIGITAL
#endif
#define CFI_REPORT_DIGITAL           clientFirmataInternal::REPORT_DIGITAL // enable digital input by port pair

//

#ifdef CFI_SET_PIN_MODE
#undef CFI_SET_PIN_MODE
#endif
#define CFI_SET_PIN_MODE             clientFirmataInternal::SET_PIN_MODE // set a pin to INPUT/OUTPUT/PWM/etc

//

#ifdef CFI_REPORT_VERSION
#undef CFI_REPORT_VERSION
#endif
#define CFI_REPORT_VERSION           clientFirmataInternal::REPORT_VERSION // report protocol version

#ifdef CFI_SYSTEM_RESET
#undef CFI_SYSTEM_RESET
#endif
#define CFI_SYSTEM_RESET             clientFirmataInternal::SYSTEM_RESET // reset from MIDI

//

#ifdef CFI_START_SYSEX
#undef CFI_START_SYSEX
#endif
#define CFI_START_SYSEX              clientFirmataInternal::START_SYSEX // start a MIDI Sysex message

#ifdef CFI_END_SYSEX
#undef CFI_END_SYSEX
#endif
#define CFI_END_SYSEX                clientFirmataInternal::END_SYSEX // end a MIDI Sysex message

//

// extended command set using sysex (0-127/0x00-0x7F)
/* 0x00-0x0F reserved for user-defined commands */

#ifdef CFI_SPI_DATA
#undef CFI_SPI_DATA
#endif
#define CFI_SPI_DATA                 clientFirmataInternal::SPI_DATA // SPI Commands start with this byte

#ifdef CFI_SERVO_CONFIG
#undef CFI_SERVO_CONFIG
#endif
#define CFI_SERVO_CONFIG             clientFirmataInternal::SERVO_CONFIG // set max angle, minPulse, maxPulse, freq

#ifdef CFI_STRING_DATA
#undef CFI_STRING_DATA
#endif
#define CFI_STRING_DATA              clientFirmataInternal::STRING_DATA // a string message with 14-bits per char

#ifdef CFI_I2C_REQUEST
#undef CFI_I2C_REQUEST
#endif
#define CFI_I2C_REQUEST              clientFirmataInternal::I2C_REQUEST // send an I2C read/write request

#ifdef CFI_I2C_REPLY
#undef CFI_I2C_REPLY
#endif
#define CFI_I2C_REPLY                clientFirmataInternal::I2C_REPLY // a reply to an I2C read request

#ifdef CFI_I2C_CONFIG
#undef CFI_I2C_CONFIG
#endif
#define CFI_I2C_CONFIG               clientFirmataInternal::I2C_CONFIG // config I2C settings such as delay times and power pins

#ifdef CFI_REPORT_FIRMWARE
#undef CFI_REPORT_FIRMWARE
#endif
#define CFI_REPORT_FIRMWARE          clientFirmataInternal::REPORT_FIRMWARE // report name and version of the firmware

#ifdef CFI_EXTENDED_ANALOG
#undef CFI_EXTENDED_ANALOG
#endif
#define CFI_EXTENDED_ANALOG          clientFirmataInternal::EXTENDED_ANALOG // analog write (PWM, Servo, etc) to any pin

#ifdef CFI_PIN_STATE_QUERY
#undef CFI_PIN_STATE_QUERY
#endif
#define CFI_PIN_STATE_QUERY          clientFirmataInternal::PIN_STATE_QUERY // ask for a pin's current mode and value

#ifdef CFI_PIN_STATE_RESPONSE
#undef CFI_PIN_STATE_RESPONSE
#endif
#define CFI_PIN_STATE_RESPONSE       clientFirmataInternal::PIN_STATE_RESPONSE // reply with pin's current mode and value

#ifdef CFI_CAPABILITY_QUERY
#undef CFI_CAPABILITY_QUERY
#endif
#define CFI_CAPABILITY_QUERY         clientFirmataInternal::CAPABILITY_QUERY // ask for supported modes and resolution of all pins

#ifdef CFI_CAPABILITY_RESPONSE
#undef CFI_CAPABILITY_RESPONSE
#endif
#define CFI_CAPABILITY_RESPONSE      clientFirmataInternal::CAPABILITY_RESPONSE // reply with supported modes and resolution

#ifdef CFI_ANALOG_MAPPING_QUERY
#undef CFI_ANALOG_MAPPING_QUERY
#endif
#define CFI_ANALOG_MAPPING_QUERY     clientFirmataInternal::ANALOG_MAPPING_QUERY // ask for mapping of analog to pin numbers

#ifdef CFI_ANALOG_MAPPING_RESPONSE
#undef CFI_ANALOG_MAPPING_RESPONSE
#endif
#define CFI_ANALOG_MAPPING_RESPONSE  clientFirmataInternal::ANALOG_MAPPING_RESPONSE // reply with mapping info

#ifdef CFI_SAMPLING_INTERVAL
#undef CFI_SAMPLING_INTERVAL
#endif
#define CFI_SAMPLING_INTERVAL        clientFirmataInternal::SAMPLING_INTERVAL // set the poll rate of the main loop

// pin modes

#ifdef CFI_PIN_MODE_INPUT
#undef CFI_PIN_MODE_INPUT
#endif
#define CFI_PIN_MODE_INPUT           clientFirmataInternal::PIN_MODE_INPUT // same as INPUT defined in Arduino.h

#ifdef CFI_PIN_MODE_OUTPUT
#undef CFI_PIN_MODE_OUTPUT
#endif
#define CFI_PIN_MODE_OUTPUT          clientFirmataInternal::PIN_MODE_OUTPUT // same as OUTPUT defined in Arduino.h

#ifdef CFI_PIN_MODE_ANALOG
#undef CFI_PIN_MODE_ANALOG
#endif
#define CFI_PIN_MODE_ANALOG          clientFirmataInternal::PIN_MODE_ANALOG // analog pin in analogInput mode

#ifdef CFI_PIN_MODE_PWM
#undef CFI_PIN_MODE_PWM
#endif
#define CFI_PIN_MODE_PWM             clientFirmataInternal::PIN_MODE_PWM // digital pin in PWM output mode

#ifdef CFI_PIN_MODE_SERVO
#undef CFI_PIN_MODE_SERVO
#endif
#define CFI_PIN_MODE_SERVO           clientFirmataInternal::PIN_MODE_SERVO // digital pin in Servo output mode

#ifdef CFI_PIN_MODE_SHIFT
#undef CFI_PIN_MODE_SHIFT
#endif
#define CFI_PIN_MODE_SHIFT           clientFirmataInternal::PIN_MODE_SHIFT // shiftIn/shiftOut mode

#ifdef CFI_PIN_MODE_I2C
#undef CFI_PIN_MODE_I2C
#endif
#define CFI_PIN_MODE_I2C             clientFirmataInternal::PIN_MODE_I2C // pin included in I2C setup

#ifdef CFI_PIN_MODE_PULLUP
#undef CFI_PIN_MODE_PULLUP
#endif
#define CFI_PIN_MODE_PULLUP          clientFirmataInternal::PIN_MODE_PULLUP // enable internal pull-up resistor for pin

#ifdef CFI_PIN_MODE_IGNORE
#undef CFI_PIN_MODE_IGNORE
#endif
#define CFI_PIN_MODE_IGNORE          clientFirmataInternal::PIN_MODE_IGNORE // pin configured to be ignored by digitalWrite and capabilityResponse

#ifdef CFI_TOTAL_PIN_MODES
#undef CFI_TOTAL_PIN_MODES
#endif
#define CFI_TOTAL_PIN_MODES          clientFirmataInternal::TOTAL_PIN_MODES

#endif // FirmataConstants_h
