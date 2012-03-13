#include <I2CEeprom.h>
#include <Wire.h>

I2CEeprom i2cEeprom(0x50);

void setup() {
    uint8_t wBuffer[] = "Hello world";
    uint8_t rBuffer[32];
    rBuffer[31] = 0;
    
    Serial.begin(57600);
    
    i2cEeprom.begin();
    i2cEeprom.writeBuffer(0, wBuffer, sizeof(wBuffer));
    i2cEeprom.readBuffer(0, rBuffer, 30);
    
    Serial.println((char *)rBuffer);
}

void loop() {
    delay(10);
}