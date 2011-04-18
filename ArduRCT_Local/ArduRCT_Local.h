#ifndef ARDURCT_LOCAL_H
#define ARDURCT_LOCAL_H

#include <WProgram.h>
#include <inttypes.h>

#include "ArduRCT_Local_Config.h"

#define THROTTLE 0
#define YAW 1
#define PITCH 2
#define ROLL 3

// TextDisplay
void tdSetup();
void tdRefreshSignalAndBattery(uint8_t signalPct, uint8_t batteryPct);
void tdRefresh(uint8_t throttle, uint8_t yaw, uint8_t pitch, uint8_t roll);

// GraphicDisplay
void gdSetup();
void gdSetID(const char *id);
void gdRefreshBattery(uint8_t batteryPct);
void gdRefresh(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t mpsSpeed, uint16_t kphSpeed, uint16_t altitude, uint16_t distance);
void gdDrawIndicators(int8_t mps, int8_t kph, int8_t altitude, int8_t distance);
void gdDraw();

// Joysticks
void joystickSetup();
uint8_t joystickGet(uint8_t channel);

#endif
