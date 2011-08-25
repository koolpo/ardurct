#ifndef ARDURCT_LOCAL_H
#define ARDURCT_LOCAL_H

#include <WProgram.h>
#include <inttypes.h>
#include <LCD_PCD8544.h>

#include "ArduRCT_Local_Config.h"

// loops per second
#define LOOPS_PER_SECOND 10

// channels
#define THROTTLE 0
#define YAW 1
#define PITCH 2
#define ROLL 3
#define ADJUST 4
#define BATTERY 100

#define ADJUST_POSITIONS 3

// menus
#define MENU_NONE 0

// buttons
#define ENTER 1
#define UP 2
#define DOWN 3

// shared variables
extern uint8_t channel[NB_CHANNELS];
extern uint8_t menuLevel;
extern LCD_PCD8544 lcdT;
extern uint8_t timerCounter;

// TextDisplay
void tdSetup();
void tdRefreshSignalAndBattery(uint8_t signalPct, uint8_t batteryPct);
void tdRefreshChannels();

// GraphicDisplay
void gdSetup();
void gdSetID(const char *id);
void gdRefreshBattery(uint8_t batteryPct);
void gdRefreshTimer(uint8_t hours, uint8_t minutes, uint8_t seconds);
void gdRefresh(uint16_t mpsSpeed, uint16_t kphSpeed, uint16_t altitude, uint16_t distance);
void gdDrawIndicators(int8_t mps, int8_t kph, int8_t altitude, int8_t distance);
void gdDraw();
void gdUpdateDisplay();

// Percentage
void percentageSetup();
uint8_t percentageGet(uint8_t channel);

// Timer
void timerStart();
void timerUpdate();

// Radio
void radioSetup();
void radioTransmitChannels();
void radioProcessReceive();

// Menu
void menuSetup();
bool menuProcess();

#endif
