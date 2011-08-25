#include "ArduRCT_Local.h"

uint8_t timerCounter;
uint8_t timerSeconds;
uint8_t timerMinutes;
uint8_t timerHours;
uint32_t timerNextTick;

void timerStart() {
    timerCounter = 0;
    timerSeconds = 0;
    timerMinutes = 0;
    timerHours = 0;
    timerNextTick = millis() + 1000/LOOPS_PER_SECOND;
}


void timerUpdate() {
    
    timerCounter++;
    if (timerCounter == LOOPS_PER_SECOND) {
        timerCounter = 0;
        timerSeconds ++;
        if (timerSeconds > 59) {
            timerSeconds = 0;
            timerMinutes ++;
            if (timerMinutes > 59) {
	        timerMinutes = 0;
	        timerHours ++;
            }
        }
        gdRefreshTimer(timerHours, timerMinutes, timerSeconds);
    }

    // wait for the loop to elapse
    // see http://arduino.cc/playground/Code/TimingRollover
    while ((int32_t)(millis() - timerNextTick) < 0) delay(1);
    timerNextTick += 1000/LOOPS_PER_SECOND;
}

