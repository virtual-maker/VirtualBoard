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

#ifndef SPI_h
#define SPI_h

#include <inttypes.h>
#include "Stream.h"

#if defined(VM_USE_VIRTUAL_SPI)
#include "VirtualSpiWrapper.h"
#elif defined(VM_USE_HARDWARE)
#include "SpiWrapper.h"
#endif

#define SPI_HAS_TRANSACTION

#define MSBFIRST 0
#define LSBFIRST SPI_LSB_FIRST

#define SPI_CLOCK_BASE 16000000		// 16Mhz

#define SPI_CLOCK_DIV1 1
#define SPI_CLOCK_DIV2 2
#define SPI_CLOCK_DIV4 4
#define SPI_CLOCK_DIV8 8
#define SPI_CLOCK_DIV16 16
#define SPI_CLOCK_DIV32 32
#define SPI_CLOCK_DIV64 64
#define SPI_CLOCK_DIV128 128
#define SPI_CLOCK_DIV256 256

// SPI Data mode
#define SPI_MODE0 0 //SPI_MODE_0
#define SPI_MODE1 1 //SPI_MODE_1
#define SPI_MODE2 2 //SPI_MODE_2
#define SPI_MODE3 3 //SPI_MODE_3

/**
 * SPISettings class
 */
class SPISettings
{

  public:
    /**
     * @brief SPISettings constructor.
     */
    SPISettings()
    {
      init(SPI_CLOCK_BASE, MSBFIRST, SPI_MODE0);
    }
    /**
     * @brief SPISettings constructor.
     *
     * @param clock SPI clock speed in Hz.
     * @param bitOrder SPI bit order.
     * @param dataMode SPI data mode.
     */
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
    {
      init(clock, bitOrder, dataMode);
    }

    uint32_t clock; //!< @brief SPI clock.
    uint8_t border; //!< @brief SPI bit order.
    uint8_t dmode; //!< @brief SPI data mode.

  private:
    /**
     * @brief Initialized class members.
     *
     * @param clk SPI clock.
     * @param bitOrder SPI bit order.
     * @param dataMode SPI data mode.
     */
    void init(uint32_t clk, uint8_t bitOrder, uint8_t dataMode)
    {
      clock = clk;
      border = bitOrder;
      dmode = dataMode;
    }

    friend class SPIClass;
};

/**
 * SPI class
 */
class SPIClass
{

  public:
    /**
     * @brief SPIClass constructor.
     */
    SPIClass();
    /**
     * @brief Start SPI operations.
     */
    void begin(int busNo = 0);
    /**
     * @brief End SPI operations.
     */
    void end();
    /**
     * @brief Sets the SPI bit order.
     *
     * @param bit_order The desired bit order.
     */
    void setBitOrder(uint8_t bit_order);
    /**
     * @brief Sets the SPI data mode.
     *
     * @param data_mode The desired data mode.
     */
    void setDataMode(uint8_t data_mode);
    /**
     * @brief Sets the SPI clock divider and therefore the SPI clock speed.
     *
     * @param divider The desired SPI clock divider.
     */
    void setClockDivider(uint16_t divider);
    /**
    * @brief Transfer a single byte
    *
    * @param data Byte to send
    * @return Data returned via spi
    */
    uint8_t transfer(uint8_t data);
    /**
    * @brief Transfer two bytes as uint16_t
    *
    * @param data to send
    * @return Data returned via spi
    */
    uint16_t transfer16(uint16_t data);
    /**
    * @brief Transfer a buffer of data
    *
    * @param tbuf Transmit buffer
    * @param rbuf Receive buffer
    * @param len Length of the data
    */
    void transfer(void *tbuf, void *rbuf, uint32_t len);
    /**
    * @brief Transfer a buffer of data without an rx buffer
    *
    * @param buf Pointer to a buffer of data
    * @param len Length of the data
    */
    void transfer(void *buf, uint32_t len);
    /**
     * @brief Start SPI transaction.
     *
     * @param settings for SPI.
     */
    void beginTransaction(SPISettings settings);
    /**
     * @brief End SPI transaction.
     */
    void endTransaction();
    /**
     * @brief Not implemented.
     *
     * @param interruptNumber ignored parameter.
     */
    void usingInterrupt(uint8_t interruptNumber);
    /**
     * @brief Not implemented.
     *
     * @param interruptNumber ignored parameter.
     */
    void notUsingInterrupt(uint8_t interruptNumber);

  private:
    void init();
};

extern SPIClass SPI;

#include "SPI.cpp"

#endif
