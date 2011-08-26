#ifndef ARDURCT_REMOTE_H
#define ARDURCT_REMOTE_H

#include <WProgram.h>
#include <inttypes.h>

#include <NewSoftSerial.h>
#include <XBee.h>

#include "ArduRCT_Remote_Config.h"

extern uint8_t servoPosition[NB_CHANNELS];
extern uint8_t radioState;
extern uint8_t radioId[4];
extern uint8_t xbeeCommand;

// Radio
void radioSetup();
void radioProcessReceive();

#endif
