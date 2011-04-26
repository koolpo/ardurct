#include "ArduRCT_Local.h"

uint8_t counter = 0;

uint16_t altitude = 0;
uint16_t distance = 0;
uint16_t speedMS = 0;


void setup() {
    tdSetup();
    tdRefreshSignalAndBattery(88, 93);

    gdSetup();
    gdSetID("8BC2");
    
    joystickSetup();
	timerStart();
}


void loop() {
	uint32_t temp;

    gdRefresh(speedMS, speedMS*36/10, altitude, distance);
    gdDrawIndicators(1, 1, 0, -1);
    tdRefresh(joystickGet(THROTTLE), joystickGet(YAW), joystickGet(PITCH), joystickGet(ROLL));
	temp = 100 * analogRead(BATTERY_PIN) / 1023;
    gdRefreshBattery(temp & 0x0FF);
    gdDraw();
    
    delay(100);
    counter += 1;
    if (counter > 10) {
        counter = 0;
        timerUpdate();
	}
}
