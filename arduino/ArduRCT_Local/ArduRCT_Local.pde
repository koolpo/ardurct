#include "ArduRCT_Local.h"

uint16_t altitude = 0;
uint16_t distance = 0;
uint16_t speedMS = 0;

uint8_t channel[NB_CHANNELS];

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    
    tdSetup();
    tdRefreshSignalAndBattery(88, 93);

    gdSetup();
    gdSetID("8BC2");
    
    percentageSetup();
    radioSetup();

    menuSetup();

    timerStart();
}


void loop() {
    // check on the menu
    menuProcess();
/**/menuLevel = MENU_NONE;
    
    // update the channels
    updateChannels();
    
    // display them unless we are in the menu
    if (menuLevel == MENU_NONE) tdRefreshChannels();
    
    // process the telemetry
    radioProcessReceive();

    // transmit the channels to the remote
    radioTransmitChannels();
    
    // update the telemetry display 5 times per second
    if (timerCounter % 2 == 0) {    
        gdRefresh(speedMS, speedMS*36/10, altitude, distance);
        gdDrawIndicators(1, 1, 0, -1);
        gdRefreshBattery(percentageGet(BATTERY));
        gdDraw();
        gdUpdateDisplay();
    }            
    
    // update the timer, this will delay a variable time
    timerUpdate();
}

void updateChannels() {
    uint8_t pct = percentageGet(THROTTLE);
    if ((pct >= 60) && (channel[THROTTLE] < 100)) channel[THROTTLE] += (pct-50)/10;
    else if ((pct <= 40) && (channel[THROTTLE] > 0)) channel[THROTTLE] -= (50-pct)/10;
    if (channel[THROTTLE] > 100) channel[THROTTLE] = 100;

    for (int i=YAW; i<ADJUST; i++) {
        pct = percentageGet(i);
        if (pct >= channel[i] + ANALOG_TOLERANCE) channel[i] = pct;
        else if ((channel[i] > ANALOG_TOLERANCE) && (pct <= channel[i] - ANALOG_TOLERANCE)) channel[i] = pct;
    }        
    
    uint8_t adjust = percentagePositionGet(ADJUST, ADJUST_POSITIONS);
    adjust = 
    channel[ADJUST] = adjust*(100/ADJUST_POSITIONS);
}
