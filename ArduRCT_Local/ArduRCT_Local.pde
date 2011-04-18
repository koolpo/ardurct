#include "ArduRCT_Local.h"



uint8_t counter = 0;
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;

uint16_t altitude = 0;
uint16_t distance = 0;
uint16_t speedMS = 0;


void setup() {
    tdSetup();
    tdRefreshSignalAndBattery(88, 93);

    gdSetup();
    gdSetID("8BC2");
    gdRefreshBattery(85);
    
    joystickSetup();
}


void loop() {
    gdRefresh(hours, minutes, seconds, speedMS, speedMS*36/10, altitude, distance);
    gdDrawIndicators(1, 1, 0, -1);
    tdRefresh(joystickGet(THROTTLE), joystickGet(YAW), joystickGet(PITCH), joystickGet(ROLL));
    gdDraw();
    
    delay(100);
    counter += 1;
    if (counter > 10) {
        counter = 0;    
        // increase the seconds   
        seconds ++;
        if (seconds > 59) {
            seconds = 0;
            minutes ++;
            if (minutes > 59) {
                minutes = 0;
                hours ++;
            }
        }
    }   

}
