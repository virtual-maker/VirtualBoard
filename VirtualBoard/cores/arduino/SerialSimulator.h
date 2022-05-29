/*
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * for the MySensors Arduino library
 * Copyright (C) 2013-2017 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
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

#ifndef SerialSimulator_h
#define SerialSimulator_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Stream.h"
#include "HardwareSerial.h"

/**
 * @brief A class equivalent to Serial in Arduino but outputs to stdout
 */
class SerialSimulator : public Stream
{

public:
	/**
	 * @brief This function does nothing.
	 *
	 * @param baud Ignored parameter.
	 */
	void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
	/**
	 * @brief This function does nothing.
	 *
	 * @param baud Ignored parameter.
	 */
	void begin(unsigned long baud, uint8_t config);
	/**
	 * @brief This function does nothing.
	 *
	 * @return always returns 1.
	 */
	int available();
	/**
	 * @brief Reads 1 key pressed from the keyboard.
	 *
	 * @return key character pressed cast to an int.
	 */
	int read();
	/**
	 * @brief Writes a single byte to stdout.
	 *
	 * @param b byte to write.
	 * @return -1 if error else, number of bytes written.
	 */
	size_t write(uint8_t b);
	size_t write(const uint8_t* buf, size_t size);

	using Print::write; // pull in write(str) and write(buf, size) from Print

	/**
	 * @brief Not supported.
	 *
	 * @return always returns -1.
	 */
	int peek();
	/**
	 * @brief Flush stdout.
	 */
	void flush();
	/**
	 * @brief Nothing to do, flush stdout.
	 */
	void end();
	/**
	* @brief Overloaded cast operators.
	*
	* Allow Serial objects to be used where a bool is expected.
	*/
	virtual operator bool();
};

extern SerialSimulator Serial;

#endif
