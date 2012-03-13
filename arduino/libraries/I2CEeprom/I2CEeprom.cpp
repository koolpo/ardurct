#include "I2CEeprom.h"

I2CEeprom::I2CEeprom(uint8_t deviceAddress) {
	_deviceAddress = deviceAddress;	
}


void I2CEeprom::begin() {
	Wire.begin();
}


void I2CEeprom::writeUInt8_t(uint16_t address, uint8_t data) {
    Wire.beginTransmission(_deviceAddress);
    Wire.write(address >> 8);     // MSB
    Wire.write(address & 0xFF);   // LSB
    Wire.write(data);
    Wire.endTransmission();
}

void I2CEeprom::writeBuffer(uint16_t address, uint8_t* data, uint8_t length) {
    Wire.beginTransmission(_deviceAddress);
    Wire.write(address >> 8);     // MSB
    Wire.write(address & 0xFF);   // LSB
    for (uint8_t i=0; i<length; i++) Wire.write(data[i]);
    Wire.endTransmission();
}

uint8_t I2CEeprom::readUInt8_t(uint16_t address) {
    Wire.beginTransmission(_deviceAddress);
    Wire.write(address >> 8);     // MSB
    Wire.write(address & 0xFF);   // LSB
    Wire.endTransmission();
    Wire.requestFrom(_deviceAddress, (uint8_t)1);
    if (Wire.available()) return Wire.read();
}

void I2CEeprom::readBuffer(uint16_t address, uint8_t *buffer, uint8_t length) {
    Wire.beginTransmission(_deviceAddress);
    Wire.write(address >> 8);     // MSB
    Wire.write(address & 0xFF);   // LSB
    Wire.endTransmission();
    Wire.requestFrom(_deviceAddress, length);
    for (uint8_t i=0; i<length; i++) {
        if (Wire.available()) buffer[i] = Wire.read();
    }
}
