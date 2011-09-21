#include <NewSoftSerial.h>
#include <XBee.h>
#include <ServoManager.h>

#include "ArduRCT_Remote.h"


void setup() { 
    // Setup the serial port
    Serial.begin(SERIAL_BAUDRATE);
    // Setup the radio
    radioSetup();
    
    // we will prevent the radio sending or receiving characters while the servo pulse is sent, for precise timing
    ServoManager.setup(XBEE_RTS);
    // attach all the servos to the manager, don't apply changes immediately to synchronize them later
    for (int i=0; i<NB_CHANNELS; i++) ServoManager.attach(i, FIRST_SERVO+i, SERVO_MANAGER_SYNCHRONIZE);  
    // apply the changes
    ServoManager.synchronizeChangeRequests();
} 

void loop() {
    // process the radio
    radioProcessReceive();
    radioProcessTransmit();
} 

