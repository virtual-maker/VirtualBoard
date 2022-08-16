/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  Modified for the MySensors Arduino library
  by Henrik Ekblad <henrik.ekblad@mysensors.org>
  Copyright (C) 2013-2017 Sensnology AB
  Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors

  Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
  Copyright (c) 2022 Immo Wache

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <algorithm>

#include "utils/stdlib_noniso.h"

#include "interrupt.h"

// Unknown macro for the Visual C++ compiler
#define __attribute__(x)

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))
//#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define word(bh, bl) ((bh <<  8) + bl)

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

using std::string;
using std::min;
using std::max;
using std::abs;

typedef uint8_t byte;
typedef uint8_t boolean;

uint32_t _startupMicros = 0;
uint32_t _startupMillis = 0;

void yield(void);
unsigned long millis(void);
unsigned long micros(void);

// WMath prototypes
long random(long howbig);
long random(long howsmall, long howbig);
void randomSeed(unsigned long seed);
long map(long, long, long, long, long);

extern void _yield(void);
extern void setup(void);
extern void loop(void);

void pinMode(uint8_t pin, uint8_t direction);
void digitalWrite(uint8_t pin, uint8_t value);
uint8_t digitalRead(uint8_t pin);
uint16_t analogRead(uint8_t pin);
void analogWrite(uint8_t pin, uint16_t value);

#define digitalPinToInterrupt(pin) GPIO._digitalPinToInterrupt(pin)
#define NOT_AN_INTERRUPT -1

void delay(unsigned int millisec);
void delayMicroseconds(unsigned int micro);

#endif
