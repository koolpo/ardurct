#include "ArduRCT_Local.h"
#include <WProgram.h>
#include <avr/pgmspace.h>
#include <ButtonMonitor.h>

uint8_t highlight[] = { 0x7f, 0x7f, 0x3E, 0x1C, 0x08 };

#define MENU_SIZE 4
const PROGMEM uint8_t menu[][16] = {
    "<..>",
    "Alt1 channels",
    "Alt2 channels",
    "Radio"
    "Backlight"
};


// Menu levels
#define MENU_NONE 0
#define MENU_MAIN 1
#define MENU_ALT1 2
#define MENU_ALT2 3
#define MENU_RADIO 4
#define MENU_SETUP 5

uint8_t menuLevel = MENU_NONE;
uint8_t menuLine = 0;


void menuSetup() {
    ButtonMonitor.add(ENTER, ENTER_PIN);
    ButtonMonitor.add(UP, UP_PIN);
    ButtonMonitor.add(DOWN, DOWN_PIN);
}


bool menuHighlightLine(uint8_t newMenuLine) {
    lcdT.setCursor(0, menuLine + 1);
    lcdT.print(" ");
    menuLine = newMenuLine;
    if (menuLine > MENU_SIZE) menuLine = 0;
    lcdT.drawBitmap(highlight, 0, (menuLine + 1)*8, 5, 8);
    return true;
}

void menuDisplay() {
    char buffer[16];
    
    switch (menuLevel) {
        case MENU_MAIN:
            for (uint8_t i=0; i<=MENU_SIZE; i++) {
                memcpy_P(buffer, &menu[i], sizeof(menu[0]));
                lcdT.setCursor(1, i+1);
                lcdT.print(buffer);
            }
            menuHighlightLine(1);
            break;
        default:
            for (uint8_t i=2; i<=MENU_SIZE+1; i++) {
                lcdT.setCursor(0, i);
                lcdT.clearLine();
            }
            menuHighlightLine(0);
            break;
    }
}

bool menuProcess() {
    // update the buttons' state
    ButtonMonitor.update();

    if (menuLevel != MENU_NONE) {
        if (ButtonMonitor.isDepressed(UP)) return menuHighlightLine(menuLine-1);
        else if (ButtonMonitor.isDepressed(DOWN)) return menuHighlightLine(menuLine+1);
    }
    if (ButtonMonitor.isDepressed(ENTER)) {
        switch (menuLevel) {
            case MENU_NONE:
                menuLevel = MENU_MAIN;
                menuDisplay();
                break;
            case MENU_MAIN:    
                if (menuLine == 0) menuLevel = MENU_NONE;
                else {
                    menuLevel = menuLine + 1;
                    menuDisplay();
                }
                break;
        }
    }
}

