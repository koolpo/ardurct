#include "ArduRCT_Local.h"

uint8_t counter = 0;

uint16_t altitude = 0;
uint16_t distance = 0;
uint16_t speedMS = 0;

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    
    tdSetup();
    tdRefreshSignalAndBattery(88, 93);

    gdSetup();
    gdSetID("8BC2");
    
    percentageSetup();
    timerStart();
    radioSetup();
}


void loop() {
    for (int i=1; i<NB_CHANNELS; i++) channel[i] = percentageGet(i);
    uint8_t pct = percentageGet(THROTTLE);
    if ((pct >+ 60) && (channel[THROTTLE] < 100)) channel[THROTTLE] += (pct-50)/10;
    else if ((pct <= 40) && (channel > 0)) channel[THROTTLE] -= (50-pct)/10;
    if (channel[THROTTLE] > 100) channel[THROTTLE] = 100;

    tdRefreshChannels();

    gdRefresh(speedMS, speedMS*36/10, altitude, distance);
    gdDrawIndicators(1, 1, 0, -1);
    gdRefreshBattery(percentageGet(BATTERY));
    gdDraw();
    
    delay(100);
    counter += 1;
    if (counter > 10) {
        counter = 0;
        timerUpdate();
    }
    radioTransmitChannels();
    radioProcessReceive();
}
