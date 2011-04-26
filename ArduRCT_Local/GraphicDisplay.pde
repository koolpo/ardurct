#include <LCD_PCD8544_VG.h>

#include "ArduRCT_Local.h"

LCD_PCD8544_VG lcdG(LCD_SCE_GRAPHIC_PIN);

uint8_t gdID[4];

#define GD_ID_START       11
#define GD_BATTERY_START  45
#define GD_BATTERY_WIDTH  35

#define GD_INDICATOR_X    35

#define GD_VAL_X  15
#define GD_TIME_X  1
#define GD_TIME_Y  9
#define GD_MPS_Y  18
#define GD_KPH_Y  26
#define GD_ALT_Y  34
#define GD_DIS_Y  42

#define GD_X 60
#define GD_Y 23
#define GD_R 20
#define GD_SKEW 3


void gdSetup() {
    lcdG.begin(2);

    analogWrite(LCD_BACKLIGHT_PIN, 120);    
    
    lcdG.drawSmallFontText("ID:", 3, 0, 0);
    lcdG.fillRectangle(11, 0, 25, 5, OFF);

    lcdG.drawSmallFontText("M/S:", 4, 0, GD_MPS_Y+1);
    lcdG.drawSmallFontText("K/H:", 4, 0, GD_KPH_Y+1);
    lcdG.drawSmallFontText("ALT:", 4, 0, GD_ALT_Y+1);
    lcdG.drawSmallFontText("DIS:", 4, 0, GD_DIS_Y+1);

    lcdG.drawSmallFontText("LOC", 3, 31, 0);
    lcdG.drawRectangle(GD_BATTERY_START-1, 0, GD_BATTERY_START+GD_BATTERY_WIDTH+1, 4, ON);
    lcdG.drawRectangle(GD_BATTERY_START+GD_BATTERY_WIDTH+2, 1, GD_BATTERY_START+GD_BATTERY_WIDTH+3, 3, ON);
    lcdG.drawLine(0, 6, LCD_PCD8544_WIDTH, 6, ON, 1);
    lcdG.drawLine(28, 0, 28, 6, ON, 1);
    lcdG.drawLine(GD_INDICATOR_X+1, 6, GD_INDICATOR_X+1, LCD_PCD8544_HEIGHT, ON, 1);

    lcdG.updateDisplay();
}


void gdSetID(const char *id) {
    for (uint8_t i=0; i<4; i++) gdID[i] = id[i];
    lcdG.fillRectangle(GD_ID_START, 0, GD_ID_START+16, 5, OFF);
    lcdG.drawSmallFontText(id, 4, GD_ID_START, 0);
    lcdG.drawLine(GD_ID_START, 6, GD_ID_START+16, 6, ON);
    lcdG.updateDisplay();
}


void gdRefreshBattery(uint8_t batteryPct) {
   // draw the battery level (percentage)
    uint16_t battery = batteryPct * GD_BATTERY_WIDTH / 100;
    lcdG.fillRectangle(GD_BATTERY_START + battery, 1, GD_BATTERY_START + GD_BATTERY_WIDTH, 3, OFF);
    lcdG.fillRectangle(GD_BATTERY_START, 0, GD_BATTERY_START + battery, 4, ON);
    lcdG.updateDisplay();
}    


void gdRefreshTimer(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    lcdG.drawMediumFontTime(hours, minutes, seconds, GD_TIME_X, GD_TIME_Y);
    lcdG.updateDisplay();
}

    
void gdRefresh(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t mpsSpeed, uint16_t kphSpeed, uint16_t altitude, uint16_t distance) {
    lcdG.drawMediumFontValueRA(mpsSpeed, 2, ' ', GD_VAL_X, GD_MPS_Y, true);
    lcdG.drawMediumFontValueRA(kphSpeed, 2, ' ', GD_VAL_X, GD_KPH_Y, true);
    lcdG.drawMediumFontValueRA(altitude, 2, ' ', GD_VAL_X, GD_ALT_Y, true);
    lcdG.drawMediumFontValueRA(distance, 2, ' ', GD_VAL_X, GD_DIS_Y, true);
    lcdG.updateDisplay();
}


void gdDrawIndicator(int8_t value, uint8_t y) {
    if (value < 0) lcdG.drawLine(GD_INDICATOR_X, y+3, GD_INDICATOR_X, y+5, ON);
    else if (value > 0) lcdG.drawLine(GD_INDICATOR_X, y, GD_INDICATOR_X, y+2, ON);
}


void gdDrawIndicators(int8_t mps, int8_t kph, int8_t altitude, int8_t distance) {
    gdDrawIndicator(mps, GD_MPS_Y);
    gdDrawIndicator(kph, GD_KPH_Y);
    gdDrawIndicator(altitude, GD_ALT_Y);
    gdDrawIndicator(distance, GD_DIS_Y);
}  

void gdDraw() {
    lcdG.drawMediumFontValueRA(analogRead(JOYSTICK_THROTTLE_PIN), 4, ' ', 50, GD_MPS_Y, false);
    lcdG.drawMediumFontValueRA(analogRead(JOYSTICK_YAW_PIN), 4, ' ', 50, GD_KPH_Y, false);
    lcdG.drawMediumFontValueRA(analogRead(JOYSTICK_PITCH_PIN), 4, ' ', 50, GD_ALT_Y, false);
    lcdG.drawMediumFontValueRA(analogRead(JOYSTICK_ROLL_PIN), 4, ' ', 50, GD_DIS_Y, false);
}
