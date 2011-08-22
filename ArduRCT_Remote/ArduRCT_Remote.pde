#include <ServoManager.h>

#include "ArduRCT_Remote.h"

void setup() { 
    radioSetup();
    for (int i=0; i<NB_CHANNELS; i++) ServoManager.attach(i, FIRST_SERVO+i);  
    Serial.begin(SERIAL_BAUDRATE);
} 

void loop() {
    // run the servo manager
    ServoManager.loop();
    
    // process the radio
    radioProcessReceive();
    radioProcessTransmit();
    
    delay(5);
} 

