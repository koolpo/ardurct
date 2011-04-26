#include "ArduRCT_Remote.h"
#include <ServoManager.h>

uint16_t servoPos[NB_SERVOS];
int8_t frame = -1 * REPEAT_FRAME_START;

void setup() { 
    for (int i=0; i<NB_SERVOS; i++) ServoManager.attach(i, FIRST_SERVO+i);  
    Serial.begin(SERIAL_SPEED);
} 

void loop() {
    // run the servo manager
    ServoManager.run();
    
    // wait to have something to do
    if (Serial.available() > 0) {
        uint8_t val = Serial.read();
        // detect the start of frame
        if ((frame < 0) && (val == FRAME_START)) frame ++;
        else {
            val -= '0';
            if ((frame % 4) == 0) servoPos[frame/4] = val;
            else servoPos[frame/4] = servoPos[frame/4] * 10 + val;
            frame ++;
        }
        if (frame/4 == NB_SERVOS) {
            frame = -1 * REPEAT_FRAME_START;
            for (int i=0; i<NB_SERVOS; i++) ServoManager.set(i, servoPos[i]);
        }
    }
} 

