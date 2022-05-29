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

/**
* This is a software emulation of EEPROM that uses a file for data storage.
* A copy of the eeprom values are also held in memory for faster reading.
*/

#ifndef SoftEeprom_h
#define SoftEeprom_h

#include <stdint.h>

/**
 * SoftEeprom class
 */
class SoftEeprom
{

public:
	/**
	 * @brief SoftEeprom constructor.
	 */
	SoftEeprom(const char *fileName, size_t length);
	/**
	 * @brief SoftEeprom copy constructor.
	 */
	SoftEeprom(const SoftEeprom& other);
	/**
	 * @brief SoftEeprom destructor.
	 */
	~SoftEeprom();
	/**
	 * @brief Read a block of bytes from eeprom.
	 *
	 * @param buf buffer to copy to.
	 * @param addr eeprom address to read from.
	 * @param length number of bytes to read.
	 */
	void readBlock(void* buf, void* addr, size_t length);
	/**
	 * @brief Write a block of bytes to eeprom.
	 *
	 * @param buf buffer to read from.
	 * @param addr eeprom address to write to.
	 * @param length number of bytes to write.
	 */
	void writeBlock(void* buf, void* addr, size_t length);
	/**
	 * @brief Read a byte from eeprom.
	 *
	 * @param addr eeprom address to read from.
	 * @return the read byte.
	 */
	uint8_t readByte(int addr);
	/**
	 * @brief Write a byte to eeprom.
	 *
	 * @param addr eeprom address to write to.
	 * @param value to write.
	 */
	void writeByte(int addr, uint8_t value);
	/**
	 * @brief Overloaded assign operator.
	 *
	 */
	SoftEeprom& operator=(const SoftEeprom& other);

private:
	size_t _length; //!< @brief Eeprom max size.
	char *_fileName; //!< @brief file where the eeprom values are stored.
	uint8_t *_values; //!< @brief copy of the eeprom values held in memory for a faster reading.
};

#endif
