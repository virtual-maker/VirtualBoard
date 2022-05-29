/*
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * for the MySensors Arduino library
 * Copyright (C) 2013-2017 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Based on wiringPi Copyright (c) 2012 Gordon Henderson.
 *
 * Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
 * Copyright (c) 2022 Immo Wache
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef interrupt_h
#define interrupt_h

#include <stdint.h>

#define CHANGE 1
#define FALLING 2
#define RISING 3
//#define NONE 4

#ifdef __cplusplus
extern "C" {
#endif

void attachInterrupt(uint8_t interruptNum, void(*userFunc)(void), int mode);
void detachInterrupt(uint8_t interruptNum);
void interrupts();
void noInterrupts();

#ifdef __cplusplus
}
#endif

#endif
