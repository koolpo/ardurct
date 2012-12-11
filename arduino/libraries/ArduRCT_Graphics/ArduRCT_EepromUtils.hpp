/*
 * EEPROM utils - Utility functions to write and read the eeprom
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
#ifndef EEPROM_UTILS_HPP
#define EEPROM_UTILS_HPP 1

#include <avr/eeprom.h>

typedef union {
	int32_t packed;
	struct {
		uint8_t byte0;
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
	} unpacked;
} packed_int32_t;

uint8_t eeprom_read_uint8_t(uint16_t address) {
	return eeprom_read_byte((const uint8_t *)address);
}


void eeprom_write_uint8_t(uint16_t address, uint8_t value) {
    eeprom_write_byte((uint8_t *)address, value);
}


int32_t eeprom_read_int32_t(uint16_t address) {
	packed_int32_t x;

    x.unpacked.byte0 = eeprom_read_byte((const uint8_t *)address);
    x.unpacked.byte1 = eeprom_read_byte((const uint8_t *)address + 1);
    x.unpacked.byte2 = eeprom_read_byte((const uint8_t *)address + 2);
    x.unpacked.byte3 = eeprom_read_byte((const uint8_t *)address + 3);
    return x.packed;
}


void eeprom_write_int32_t(uint16_t address, int32_t value) {
	packed_int32_t x;

	x.packed = value;
    eeprom_write_byte((uint8_t *)address, x.unpacked.byte0);
    eeprom_write_byte((uint8_t *)address + 1, x.unpacked.byte1);
    eeprom_write_byte((uint8_t *)address + 2, x.unpacked.byte2);
    eeprom_write_byte((uint8_t *)address + 3, x.unpacked.byte3);
}

#endif