#include "ArduRCT_Local.h"

uint8_t timerSeconds = 0;
uint8_t timerMinutes = 0;
uint8_t timerHours = 0;

void timerStart() {
    timerSeconds = 0;
    timerMinutes = 0;
    timerHours = 0;
}


void timerUpdate() {
    // increase the seconds   
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
