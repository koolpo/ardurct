#include <Servo.h>

#include "ArduRCT_Remote.h"

Servo servo[NB_CHANNELS];

void setup() { 
    // Setup the serial port
    Serial.begin(SERIAL_BAUDRATE);
    // Setup the radio
    radioSetup();
    // Setup the servos
    for (uint8_t i=0; i<NB_CHANNELS; i++) {
        servo[i].attach(i + FIRST_SERVO);
        servo[i].write(90);
    }
} 

void loop() {
    // process the radio
    radioProcessReceive();
    radioProcessTransmit();
} 

