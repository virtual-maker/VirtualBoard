/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 * Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
 * Copyright (c) 2022 Immo Wache
 */

#include "SPI.h"
#include <utils/log.h>

#if defined(VM_USE_VIRTUAL_SPI)
VirtualSpiWrapper spiWrapper;
#elif defined(VM_USE_HARDWARE)
SpiWrapper spiWrapper;
#else
#define VM_DISABLE_SPI
#endif


// Declare a single default instance
SPIClass SPI = SPIClass();

SPIClass::SPIClass()
{
#if defined(VB_FIRMATA_PORT)
  _spi = new CFI_SPIFeature(GPIO.ClientFirmata);
#endif
}

void SPIClass::begin(int busNo)
{
#if defined(VB_FIRMATA_PORT)
  _isFirstTransaction = true;
  byte channel = 0;
  _spi->begin(channel);
#elif defined(VM_DISABLE_SPI)
  logError("Can't open SPI device\n");
#else
  spiWrapper.begin();
#endif
}

void SPIClass::end()
{
#if defined(VB_FIRMATA_PORT)
  byte channel = 0;
  _spi->end(channel);
#elif defined(VM_DISABLE_SPI)
  logError("Can't close SPI device\n");
#else
  spiWrapper.end();
#endif
}

void SPIClass::setBitOrder(uint8_t bit_order)
{
  logError("Function deprecated: Can't set SPI bit order.\n");
}

void SPIClass::setDataMode(uint8_t data_mode)
{
  logError("Function deprecated: Can't set SPI mode.\n");
}

void SPIClass::setClockDivider(uint16_t divider)
{
  logError("Function deprecated: Can't set SPI clock divider.\n");
}

uint8_t SPIClass::transfer(uint8_t data)
{
  uint8_t result;
  transfer((void*)&data, &result, 1);
  return result;
}

uint16_t SPIClass::transfer16(uint16_t data)
{
  // ToDo: #iwa Do we have to swap data?
  uint16_t result;
  transfer((void*)&data, &result, 2);
  return result;
}

void SPIClass::transfer(void* tbuf, void* rbuf, uint32_t len)
{
#if defined(VB_FIRMATA_PORT)
	byte deviceId = 0;
	byte channel = 0;
	byte requestId = 0;
	bool deselectCsPin = true;
	_spi->transfer(deviceId, channel, requestId, deselectCsPin, (byte*)tbuf, (byte*)rbuf, len);
#elif !defined(VM_DISABLE_SPI)
  spiWrapper.transfer((const unsigned char*)tbuf, (const unsigned char*)rbuf, len);
#endif
}

void SPIClass::transfer(void* buf, uint32_t len)
{
  transfer(buf, buf, len);
}

void SPIClass::beginTransaction(SPISettings settings)
{
#if defined(VB_FIRMATA_PORT)
  if (!_isFirstTransaction) {
    return;
  }
  byte deviceId = 0;
  byte channel = 0;
  byte csPinOptions = 1; // 0x01;
  byte csPin = VB_FIRMATA_SPI_CSN;
  _spi->deviceConfig(deviceId, channel, settings.dmode, settings.border, settings.clock, csPinOptions, csPin);
  _isFirstTransaction = false;
#elif !defined(VM_DISABLE_SPI)
  spiWrapper.beginTransaction(settings.clock, settings.border, settings.dmode);
#endif
}

void SPIClass::endTransaction()
{
  // Do nothing
}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
  (void)interruptNumber;
}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
{
  (void)interruptNumber;
}
