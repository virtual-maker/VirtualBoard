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

#include "GPIOWrapper.h"

#ifndef GPIO_h
#define	GPIO_h

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW 0
#define HIGH 1

/**
 * @brief GPIO class
 */
class GPIOClass
{

public:
    /**
     * @brief GPIOClass constructor.
     */
    GPIOClass();
    /**
     * @brief GPIOClass copy constructor.
     */
    GPIOClass(const GPIOClass& other);
    /**
     * @brief GPIOClass destructor.
     */
    ~GPIOClass();
    /**
     * @brief Configures the specified pin to behave either as an input or an output.
     *
     * @param pin The number of the pin.
     * @param mode INPUT or OUTPUT.
     */
    void _pinMode(uint8_t pin, uint8_t mode);
    /**
     * @brief Write a high or a low value for the given pin.
     *
     * @param pin number.
     * @param value HIGH or LOW.
     */
    void _digitalWrite(uint8_t pin, uint8_t value);

    /**
    * @brief Write a value of byte into the given port.
    *
    * @param port number.
    * @param byte value.
    */
    void _digitalWritePort(uint8_t port, uint8_t value);

    /**
     * @brief Reads the value from a specified pin.
     *
     * @param pin The number of the pin.
     * @return HIGH or LOW.
     */
    uint8_t _digitalRead(uint8_t pin);
    /**
    * @brief Reads the analog value from a specified analog pin.
    *
    * @param pin The number of the pin.
    * @return analog value
    */
    uint16_t _analogRead(uint8_t pin);
    /**
    * @brief Write the analog value to a specified PWM pin.
    *
    * @param pin The number of the pin.
    * @param value analog
    */
    void _analogWrite(uint8_t pin, uint16_t value);
    /**
     * @brief Overloaded assign operator.
     *
     */
    GPIOClass& operator=(const GPIOClass& other);

#if defined(VB_FIRMATA_PORT)
    CFI_ClientFirmata ClientFirmata;
#endif

  private:
#if defined(VB_FIRMATA_PORT)
    CFI_DigitalInputFeature* _digitalInput = NULL;
    CFI_DigitalOutputFeature* _digitalOutput = NULL;
    CFI_AnalogInputFeature* _analogInput = NULL;
    CFI_AnalogOutputFeature* _analogOutput = NULL;
#endif

};

extern GPIOClass GPIO;

#endif
