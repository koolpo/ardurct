#include <NewSoftSerial.h>
#include <XBee.h>
#include <ServoManager.h>

#include "ArduRCT_Remote.h"


void setup() { 

    Serial.begin(SERIAL_BAUDRATE);

    radioSetup();
    
    // we will prevent the radio sending or receiving characters while the servo pulse is sent for precise timing
    ServoManager.setup(XBEE_RTS);
    // attach all the servos to the manager
    for (int i=0; i<NB_CHANNELS; i++) ServoManager.attach(i, FIRST_SERVO+i);  
} 

void loop() {
    // process the radio
    radioProcessReceive();
    radioProcessTransmit();
} 

