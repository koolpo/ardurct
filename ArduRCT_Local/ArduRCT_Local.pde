#include "ArduRCT_Local.h"

uint8_t counter = 0;

uint16_t altitude = 0;
uint16_t distance = 0;
uint16_t speedMS = 0;

uint8_t throttle = 0;

void setup() {
    tdSetup();
    tdRefreshSignalAndBattery(88, 93);

    gdSetup();
    gdSetID("8BC2");
    
    percentageSetup();
    timerStart();
}


void loop() {
    gdRefresh(speedMS, speedMS*36/10, altitude, distance);
    gdDrawIndicators(1, 1, 0, -1);
    tdRefresh(throttle, percentageGet(YAW), percentageGet(PITCH), percentageGet(ROLL), percentageGet(ADJUST));
    gdRefreshBattery(percentageGet(BATTERY));
    gdDraw();
    
    delay(100);
    counter += 1;
    if (counter > 10) {
        counter = 0;
        timerUpdate();
    }
    
    uint8_t pct = percentageGet(THROTTLE);
    if ((pct >+ 60) && (throttle < 100)) throttle += (pct-50)/10;
    else if ((pct <= 40) && (throttle > 0)) throttle -= (50-pct)/10;
    if (throttle > 100) throttle = 100;
}
