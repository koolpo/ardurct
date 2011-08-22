#include <LCD_PCD8544.h>
#include "ArduRCT_Local.h"

LCD_PCD8544 lcdT(LCD_SCE_TEXT_PIN, LCD_SCLK_PIN, LCD_SDIN_PIN, LCD_DC_PIN, LCD_RESET_PIN);

uint8_t tdHeader[] = { 
    0x5F, 0x45, 0x5B, 0x40, 0x54, 0x48, 0x54, 0x40, 0x40, 0x50, 0x50, 0x50, 0x58, 0x58, 0x58, 0x5C,
    0x5C, 0x5C, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x40, 0x40, 0x40, 0x7F, 0x40, 0x40, 0x5F, 0x45, 
    0x5B, 0x40, 0x5F, 0x55, 0x51, 0x40, 0x5F, 0x42, 0x5F, 0x40, 0x40, 0x5F, 0x51, 0x51, 0x51, 0x51, 
    0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 
    0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 
    0x51, 0x5F, 0x4E, 0x4E };

uint8_t tdThrottle = 0;
uint8_t tdYaw = 0;
uint8_t tdPitch = 0;
uint8_t tdRoll = 0;
uint8_t tdAdjust = 0;

#define TD_SIGNAL_START 9
#define TD_SIGNAL_BAR 3
#define TD_BATTERY_START 44
#define TD_BATTERY_WIDTH 37

#define THROTTLE_LINE 1
#define YAW_LINE 2
#define PITCH_LINE 3
#define ROLL_LINE 4
#define ADJUST_LINE 5


void tdSetup() {
    lcdT.begin(2);    
        
    lcdT.setCursor(0, THROTTLE_LINE);
    lcdT.print("Th");    
    lcdT.setCursor(0, YAW_LINE);
    lcdT.print("Ya");    
    lcdT.setCursor(0, PITCH_LINE);
    lcdT.print("Pi");    
    lcdT.setCursor(0, ROLL_LINE);
    lcdT.print("Ro");    
    lcdT.setCursor(0, ADJUST_LINE);
    lcdT.print("Ad");    
}

void tdRefreshSignal(uint8_t signalPct, uint8_t bar, uint8_t strength) {
    if (signalPct >= 17*bar) {
       for (uint8_t i=0; i<TD_SIGNAL_BAR; i++) tdHeader[TD_SIGNAL_START+(bar-1)*TD_SIGNAL_BAR+i] = strength; 
    }
}
    
void tdRefreshSignalAndBattery(uint8_t signalPct, uint8_t batteryPct) {
    uint8_t i;
    
    // draw the signal strength (percentage)
    for (i=0; i<TD_SIGNAL_BAR*5; i++) tdHeader[TD_SIGNAL_START+i] = 0x40; 
    tdRefreshSignal(signalPct, 1, 0x50);
    tdRefreshSignal(signalPct, 2, 0x58);
    tdRefreshSignal(signalPct, 3, 0x5C);
    tdRefreshSignal(signalPct, 4, 0x5E);
    tdRefreshSignal(signalPct, 5, 0x5F);

    // draw the battery level (percentage)
    uint16_t battery = batteryPct * TD_BATTERY_WIDTH / 100;
    for (i=battery; i<TD_BATTERY_WIDTH; i++) tdHeader[TD_BATTERY_START+i] = 0x51;
    for (i=0; i<=battery; i++) tdHeader[TD_BATTERY_START+i] = 0x5F;

    // refresh the header
    lcdT.drawBitmap(tdHeader, 0, 0, 84, 8);    
    
}

void tdRefreshChannels() {
    if (channel[THROTTLE] != tdThrottle) {
        tdThrottle = channel[THROTTLE];
        lcdT.drawProgressBar(2, THROTTLE_LINE, 12, tdThrottle, ON);
    }
    if (channel[YAW] != tdYaw) {
        tdYaw = channel[YAW];
        lcdT.drawProgressBar(2, YAW_LINE, 12, tdYaw, OFF);
    }
    if (channel[PITCH] != tdPitch) {
        tdPitch = channel[PITCH];
        lcdT.drawProgressBar(2, PITCH_LINE, 12, tdPitch, OFF);
    }
    if (channel[ROLL] != tdRoll) {
        tdRoll = channel[ROLL];
        lcdT.drawProgressBar(2, ROLL_LINE, 12, tdRoll, OFF);
    }
    if (channel[ADJUST] != tdAdjust) {
        tdAdjust = channel[ADJUST];
        lcdT.drawProgressBar(2, ADJUST_LINE, 12, tdAdjust, OFF);
    }
}


