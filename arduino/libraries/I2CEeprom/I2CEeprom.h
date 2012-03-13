/*
 * I2CEeprom - Interface to I2C eeprom
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * Versions
 *	v1.0	Initial release
 */
 
#ifndef I2C_EEPROM_H
#define I2C_EEPROM_H

#include "../Wire/Wire.h"
#include <inttypes.h>
#include <Arduino.h>

class I2CEeprom {

	public:
		I2CEeprom(uint8_t deviceAddress);

		void begin();
		
		void writeUInt8_t(uint16_t address, uint8_t data);
		
		void writeBuffer(uint16_t address, uint8_t* data, uint8_t length);
		
		uint8_t readUInt8_t(uint16_t address);
		
		void readBuffer(uint16_t address, uint8_t *buffer, uint8_t length);

	private:
		uint8_t _deviceAddress;

};

#endif  /* I2C_EEPROM_H */
