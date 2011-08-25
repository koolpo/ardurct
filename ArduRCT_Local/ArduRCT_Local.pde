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
    
    
    // display tthem unless we are in the menu
    if (menuLevel == MENU_NONE) tdRefreshChannels();
    
    // transmit them to the remote
    radioTransmitChannels();
    
    // process the telemetry
    radioProcessReceive();
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
    else if ((pct <= 40) && (channel > 0)) channel[THROTTLE] -= (50-pct)/10;
    if (channel[THROTTLE] > 100) channel[THROTTLE] = 100;

    for (int i=YAW; i<=ADJUST; i++) channel[i] = percentageGet(i);
    
    channel[ADJUST] = channel[ADJUST]*ADJUST_POSITIONS/100;
}
