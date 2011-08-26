#include <XBee.h>

#include "ArduRCT_Remote.h"


void setup() { 

    Serial.begin(SERIAL_BAUDRATE);

    radioSetup();
    
    for (int i=0; i<NB_CHANNELS; i++) ServoManager.attach(i, FIRST_SERVO+i);  
} 

void loop() {
    // run the servo manager
    ServoManager.run();
    
    // process the radio
    radioProcessReceive();
    radioProcessTransmit();
} 

