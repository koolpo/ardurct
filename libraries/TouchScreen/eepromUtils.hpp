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