#include "ArduRCT_Local.h"
#include <WProgram.h>
#include <avr/pgmspace.h>

unsigned char highlight[] = { 0x7f, 0x7f, 0x3E, 0x1C, 0x08 };

#define MENU_SIZE 4
const PROGMEM unsigned char menu[][16] = {
    "<..>",
    "Alt1 channels",
    "Alt2 channels",
    "Radio"
    "Backlight"
};

#define MENU_NONE 0

// Menu states
#define ENTER_TRIGGERED 1
#define ENTER_PUSHED 2
#define ENTER_WAITING 3
#define UP_TRIGGERED 4
#define UP_PUSHED 5
#define UP_WAITING 6
#define DOWN_TRIGGERED 7
#define DOWN_PUSHED 8
#define DOWN_WAITING 9
#define MENU_IDLE 10

// Menu levels
#define MENU_MAIN 1
#define MENU_ALT1 2
#define MENU_ALT2 3
#define MENU_RADIO 4
#define MENU_SETUP 5


uint8_t menuState = MENU_NONE;
uint8_t menuLevel = 0;
uint8_t menuLine = 0;



void menuSetup() {
    pinMode(ENTER_PIN, INPUT);
    digitalWrite(ENTER_PIN, HIGH);
    pinMode(UP_PIN, INPUT);
    digitalWrite(UP_PIN, HIGH);
    pinMode(DOWN_PIN, INPUT);
    digitalWrite(DOWN_PIN, HIGH);
}

void menuHighlightLine(uint8_t newMenuLine) {
    lcdT.setCursor(0, menuLine + 1);
    lcdT.print(" ");
    menuLine = newMenuLine;
    if (menuLine > MENU_SIZE) menuLine = 0;
    lcdT.drawBitmap(highlight, 0, (menuLine + 1)*8, 5, 8);
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

void menuProcess() {
    switch (menuState) {
        case MENU_NONE:
        case MENU_IDLE:
            if (digitalRead(ENTER_PIN) == 0) menuState = ENTER_TRIGGERED;
            else if (digitalRead(DOWN_PIN) == 0) menuState = DOWN_TRIGGERED;
            else if (digitalRead(UP_PIN) == 0) menuState = UP_TRIGGERED;
            break;
        // as this process is called every 50ms, we debounce here
        case ENTER_TRIGGERED:
            if (digitalRead(ENTER_PIN) == 0) menuState = ENTER_PUSHED;
            else menuState = MENU_IDLE;
            break;
        case UP_TRIGGERED:
            if (digitalRead(UP_PIN) == 0) menuState = UP_PUSHED;
            else menuState = MENU_IDLE;
            break;
        case DOWN_TRIGGERED:
            if (digitalRead(DOWN_PIN) == 0) menuState = DOWN_PUSHED;
            else menuState = MENU_IDLE;
            break;
        // after the action, we have to wait for the button to go up again
        case ENTER_WAITING:
            if (digitalRead(ENTER_PIN) == 1) menuState = MENU_IDLE;
            break;
        case UP_WAITING:
            if (digitalRead(UP_PIN) == 1) menuState = MENU_IDLE;
            break;
        case DOWN_WAITING:
            if (digitalRead(DOWN_PIN) == 1) menuState = MENU_IDLE;
            break;
            
        case ENTER_PUSHED:
            menuState = ENTER_WAITING;
            switch (menuLevel) {
                case MENU_NONE:
                    menuLevel = MENU_MAIN;
                    menuDisplay();
                    break;
                case MENU_MAIN:
                    switch (menuLine) {
                        case 0:
                            menuState = MENU_NONE;
                            break;
                        default:
                            menuLevel = menuLine + 1;
                            menuDisplay();
                            break;
                    }
                    break;
            }
            break;
        case UP_PUSHED:
            menuState = UP_WAITING;
            menuHighlightLine(menuLine-1);
            break;
        case DOWN_PUSHED:
            menuState = DOWN_WAITING;
            menuHighlightLine(menuLine+1);
            break;
    }
}

