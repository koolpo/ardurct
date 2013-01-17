/*
 * Clock - Digital clock with alarm
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 *  This is a clock complete with different alarms for every week day
 *  MENU    changes the display: TIME, DATE_SET, TIME_SET, ALARM_D1_SET, .., ALARM_D7_SET
 *          if alarm is ringing, stops the alarm
 *  ENTER   moves from field to field in the ???_SET screens
 *  UP      increases the current field value
 *          if alarm is ringing, snoozes the alarm for 5 minutes
 *  DOWN    decreases the current field value
 *          if alarm is ringing, snoozes the alarm for 10 minutes
 */

#include <SPI.h>
#include <ArduRCT_EventManager.h>
#include <ArduRCT_Graphics.h>

// Change to your version: the following include will automatically create the proper 'touscruino' object
#define TOUSCRUINO_VERSION 1
#include <ArduRCT_TouScruino.h>

// how long before we return to the TIME screen if no action was taken
#define RETURN_TO_TIME_TRIGGER 30

#define SCREEN_TIME         0
#define SCREEN_SET_DATE     1
#define SCREEN_SET_TIME     2
#define SCREEN_SET_ALARM_D1 3
#define SCREEN_SET_ALARM_D7 9

#define DIGIT_WIDTH     32
#define DIGIT_HEIGHT    80
#define DIGIT_THICKNESS 8
#define SECONDS_WIDTH   120
#define SECONDS_HEIGHT  6
#define DIGIT_STYLE     GRAPHICS_STYLE_ADVANCED
    
uint8_t timeDigit[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t timeDigit_x[] = { H1_X, H2_X, M1_X, M2_X };

uint8_t screen;
uint8_t activeField = 0;
int16_t field[] = { 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t fieldMax[] = { 0, 0, 0 };
uint8_t fieldMin[] = { 0, 0, 0 };
uint8_t nbFields = 0;
uint8_t watchdog = 0;

ArduRCT_EventHandler timeEventHandler(EVENT_TIME, &drawDateAndTime);
ArduRCT_EventHandler buttonEventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, &handleButtons);

void setup() {
    touscruino.begin(WHITE, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillScreen(BLACK);
    touscruino.setRotation(GRAPHICS_ROTATION_90);
    
    touscruino.registerEventHandler(&timeEventHandler);
    touscruino.registerEventHandler(&buttonEventHandler);

    // set the RTC
    touscruino.getRTC()->setDateTime(2012, DECEMBER, 31, 23, 45, 0);
    touscruino.getRTC()->setAlarmTime(THURSDAY, 23, 50, RTC_ALARM_ON);
    touscruino.getRTC()->setAlarmTime(FRIDAY, 1, 05, RTC_ALARM_ON);

    // draw the time
    screen = SCREEN_TIME;
    drawScreen();
}

void loop() {
    touscruino.update();
}

bool handleButtons(uint8_t eventType, uint8_t buttonId) {
    watchdog = 0;
    if (eventType == EVENT_SWITCH_PRESSED) {
        if (buttonId == TOUSCRUINO_MENU) {
            if (touscruino.getRTC()->isAlarmRinging() || touscruino.getRTC()->isAlarmSnoozing()) {
                touscruino.getRTC()->stopAlarm();
                showAlarmStatus();
            } else {
                changeScreen(screen + 1);
            }
        } else if (buttonId == TOUSCRUINO_ENTER) {
            if (screen == SCREEN_TIME) return true;
            activeField ++;
            if (activeField >= nbFields) activeField = 0;
            if (screen >= SCREEN_SET_TIME) drawTimeAdjust();
            if (screen >= SCREEN_SET_ALARM_D1) drawAlarmAdjust();
            if (screen == SCREEN_SET_DATE) drawDateAdjust();
        } else {
            if (screen == SCREEN_TIME) {
                if (touscruino.getRTC()->isAlarmRinging() || touscruino.getRTC()->isAlarmSnoozing()) {
                    if (buttonId == TOUSCRUINO_UP) touscruino.getRTC()->snoozeAlarm(5);
                    if (buttonId == TOUSCRUINO_DOWN) touscruino.getRTC()->snoozeAlarm(10);
                    showAlarmStatus();
                }
            } 
        }
    }
    // we keep PRESSED and REPEATING events if the screen is not SCREEN_TIME
    if ((screen != SCREEN_TIME) && (eventType != EVENT_SWITCH_RELEASED)) {
        if (buttonId == TOUSCRUINO_UP) {
            field[activeField] ++;
            if (field[activeField] > fieldMax[activeField]) field[activeField] = fieldMin[activeField];
            if (screen == SCREEN_SET_DATE) {
                uint8_t mld = touscruino.getRTC()->getMonthLastDay(field[0], field[1]);
                if (field[2] > mld) field[2] = mld;
            }
        } else if (buttonId == TOUSCRUINO_DOWN) {
            field[activeField] --;
            if (field[activeField] < fieldMin[activeField]) field[activeField] = fieldMax[activeField];
        }
        if ((buttonId == TOUSCRUINO_UP) || (buttonId == TOUSCRUINO_DOWN)) {
            if (screen >= SCREEN_SET_TIME) drawTimeAdjust();
            if (screen >= SCREEN_SET_ALARM_D1) drawAlarmAdjust();
            if (screen == SCREEN_SET_DATE) drawDateAdjust();
        }
    }
    return true;
}

#define H1_X 0
#define H2_X 38
#define M1_X 88
#define M2_X 130
#define COLUMN_X 79
#define ALARM_STRING_X 30
#define ALARM_X 84
#define DATE_X 12
#define TIME_Y 0
#define DATE_Y 89
#define ALARM_Y 115
#define SECONDS_Y 105
bool drawDateAndTime(uint8_t eventType) {
    // show the alarm status
    showAlarmStatus();

    if (screen != SCREEN_TIME) {
        // watchdog is reset every time a button is interacted with
        watchdog ++;
        // check if we have stayed a long time doing nothing
        // if yes, return to SCREEN_TIME
        if (watchdog > RETURN_TO_TIME_TRIGGER) changeScreen(SCREEN_TIME);
        return true;
    }
    
    ArduRCT_RealTimeClock *rtc = touscruino.getRTC();
    // blink the seconds
    if ((rtc->getSecond() % 2) == 0) drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS);
    else drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS);
    
    // draw a progress bar for the seconds
    if (rtc->getSecond() == 0) touscruino.fillRectangle((touscruino.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, SECONDS_WIDTH, SECONDS_HEIGHT, BLACK);
    else touscruino.fillRectangle((touscruino.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, rtc->getSecond() * SECONDS_WIDTH / 60, SECONDS_HEIGHT, YELLOW);
    
    if (eventType == EVENT_TIME_MINUTE) {
        // draw hour and minutes, digit by digit
        for (int i=0; i<4; i++) {
            if (timeDigit[i] != rtc->getTimeDigit(i)) {
                // erase the digit
                touscruino.drawBigDigit(timeDigit[i], timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS, DIGIT_STYLE);
                // draw the new one
                if ((i != 0) || (timeDigit[i] != 0)) touscruino.drawBigDigit(rtc->getTimeDigit(i), timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS, DIGIT_STYLE);
                timeDigit[i] = rtc->getTimeDigit(i);
            }
        }
    }

    // if date as changed, redraw the date and alarm 
    if (eventType == EVENT_TIME_DAY) {
        touscruino.drawString(rtc->getDateAsString(RTC_WITH_DAY_OF_WEEK), DATE_X, DATE_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
        if (rtc->isAlarmOn(rtc->getDayOfWeek())) {
            touscruino.drawString("Alarm", ALARM_STRING_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
            touscruino.drawString(touscruino.getRTC()->getAlarmTimeAsString(rtc->getDayOfWeek()), ALARM_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
        } else {
            touscruino.fillRectangle(ALARM_STRING_X, ALARM_Y, 12*touscruino.getFontCharWidth(FONT_MEDIUM), touscruino.getFontHeight(FONT_MEDIUM), BLACK);
        }
    }

    return true;
}
        
void changeScreen(uint8_t newScreen) {
    ArduRCT_RealTimeClock *rtc = touscruino.getRTC();
    if (screen != SCREEN_TIME) {
        if (screen == SCREEN_SET_DATE) rtc->setDate(field[0], field[1], field[2]);
        else if (screen == SCREEN_SET_TIME) rtc->setTime(field[0], field[1], 0);
        else if (screen >= SCREEN_SET_ALARM_D1) rtc->setAlarmTime(screen - SCREEN_SET_ALARM_D1, field[0], field[1], field[2]);
    }
    screen = newScreen;
    if (screen > SCREEN_SET_ALARM_D7) screen = SCREEN_TIME;
    fieldMax[0] = 23;
    fieldMax[1] = 59;
    for (uint8_t i=0; i<3; i++) fieldMin[i] = 0;
    if (screen == SCREEN_SET_TIME) {
        field[0] = rtc->getHour();
        field[1] = rtc->getMinute();
        nbFields = 2;
    } else if (screen >= SCREEN_SET_ALARM_D1) {
        rtcAlarm_t *alarm = rtc->getAlarmTime(screen - SCREEN_SET_ALARM_D1);
        field[0] = (alarm->hour & ~RTC_ALARM_ON);
        timeDigit[0] = field[0] / 10;
        timeDigit[1] = field[0] % 10;
        field[1] = alarm->minute;
        timeDigit[2] = field[1] / 10;
        timeDigit[3] = field[1] % 10;
        field[2] = (alarm->hour & ~RTC_ALARM_ON) != 0 ? 1 : 0;
        fieldMax[2] = 1;
        nbFields = 3;
    } else if (screen == SCREEN_SET_DATE) {
        field[0] = rtc->getYear();
        fieldMax[0] = 2300;
        field[1] = rtc->getMonth();
        fieldMax[1] = 12;
        fieldMin[1] = 1;
        field[2] = rtc->getDay();
        fieldMax[2] = 31;
        fieldMin[2] = 1;
        field[3] = rtc->getDayOfWeek();
        nbFields = 3;
    } else {
        for (uint8_t i=0; i<4; i++) timeDigit[i] = 0xFF;
    }
    touscruino.fillScreen(BLACK);
    activeField = 0;
    drawScreen();
}

void drawScreen() {
    if (screen == SCREEN_TIME) {
        drawDateAndTime(EVENT_TIME_MINUTE);
        drawDateAndTime(EVENT_TIME_DAY);
    } else if (screen == SCREEN_SET_TIME) {
        drawCenteredString("Set time",  5, FONT_MEDIUM);
        drawTimeAdjust();
    } else if (screen == SCREEN_SET_DATE) {
        drawCenteredString("Set date",  5, FONT_MEDIUM);
        drawDateAdjust();
    } else {	// set alarms
        int x = drawCenteredString("Set DOW alarm",  5, FONT_MEDIUM);
        x += touscruino.getFontCharWidth(FONT_MEDIUM)*4;
        touscruino.drawString(touscruino.getRTC()->getDayOfWeekAsString(screen-SCREEN_SET_ALARM_D1), x, 5, ORANGE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
        drawTimeAdjust();
        drawAlarmAdjust();
    }
}

void drawTimeAdjust() {
    timeDigit[0] = field[0] / 10;
    timeDigit[1] = field[0] % 10;
    timeDigit[2] = field[1] / 10;
    timeDigit[3] = field[1] % 10;
    touscruino.fillRectangle(20, 35, 32*4+10+4, 50, BLACK);
    touscruino.drawBigDigit(timeDigit[0], 15, 35, 28, 50, activeField == 0 ? RED : WHITE, 6, DIGIT_STYLE);
    touscruino.drawBigDigit(timeDigit[1], 15+32, 35, 28, 50, activeField == 0 ? RED : WHITE, 6, DIGIT_STYLE);
    drawColumn(15+32*2+3, 35, 50, WHITE, 6);
    touscruino.drawBigDigit(timeDigit[2], 15+32*2+10, 35, 28, 50, activeField == 1 ? RED : WHITE, 6, DIGIT_STYLE);
    touscruino.drawBigDigit(timeDigit[3], 15+32*3+10, 35, 28, 50, activeField == 1 ? RED : WHITE, 6, DIGIT_STYLE);
}

void drawAlarmAdjust() {
    uint8_t x = 80;
    uint8_t y = 108;
    touscruino.fillRectangle(x-12, y-16, 50, 32, BLACK);
    touscruino.drawRectangle(x-8, y-8, 8, 16, activeField == 2 ? RED : WHITE, 2);
    touscruino.drawLine(x, y-8, x+8, y-16, activeField == 2 ? RED : WHITE, 2);
    touscruino.drawLine(x, y+8, x+8, y+16, activeField == 2 ? RED : WHITE, 2);
    touscruino.drawLine(x+8, y-16, x+8, y+16, activeField == 2 ? RED : WHITE, 2);
    if (field[2] == 0) {
        touscruino.drawLine(x-10, y-13, x+16, y+13, RED, 3);
        touscruino.drawLine(x-10, y+13, x+16, y-13, RED, 3);
    } else {
        drawNote(x+15, y+3, activeField == 2 ? RED : WHITE);
        drawNote(x+25, y, activeField == 2 ? RED : WHITE);
    }
}

void drawNote(uint8_t x, uint8_t y, uint16_t color) {
    touscruino.fillRectangle(x, y, 4, 2, color);
    touscruino.fillRectangle(x+1, y-1, 2, 4, color);
    touscruino.drawLine(x+3, y-11, x+3, y, color, 1);
    touscruino.drawLine(x+3, y-11, x+6, y-11, color, 1);
}

#define DATE_ADJUST_YEAR_X 10
#define DATE_ADJUST_MONTH_X 100
#define DATE_ADJUST_YEAR_Y 45
#define DATE_ADJUST_DAY_Y 80
void drawDateAdjust() {
    // DOW DD MMM YYYY
    char *buffer = touscruino.getRTC()->getDateAsString(field[0], field[1], field[2], RTC_WITH_DAY_OF_WEEK);
    uint8_t fcw = touscruino.getFontCharWidth(FONT_HUGE);
    touscruino.drawString(&buffer[11], DATE_ADJUST_YEAR_X, DATE_ADJUST_YEAR_Y, activeField == 0 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
    buffer[10] = 0;
    touscruino.drawString(&buffer[7], DATE_ADJUST_MONTH_X, DATE_ADJUST_YEAR_Y, activeField == 1 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
    buffer[6] = 0;
    touscruino.drawString(&buffer[4], DATE_ADJUST_MONTH_X + fcw, DATE_ADJUST_DAY_Y, activeField == 2 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
    buffer[3] = 0;
    touscruino.drawString(buffer, DATE_ADJUST_YEAR_X + fcw, DATE_ADJUST_DAY_Y, WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
}

void drawColumn(uint8_t x, uint8_t y, uint8_t height, uint16_t color, uint8_t thickness) {
    uint8_t ht = thickness/2;
    touscruino.fillRectangle(x - ht , y + height/3 - ht, thickness, thickness, color); 
    touscruino.fillRectangle(x - ht , y + 2*height/3 - ht, thickness, thickness, color); 
}

int drawCenteredString(char *s, uint8_t y, uint8_t fontSize) {
    int x = (touscruino.getWidth() - touscruino.getStringWidth(s, fontSize))/2;
    touscruino.drawString(s, x, y, WHITE, fontSize, FONT_BOLD, NO_OVERLAY);
    y += 5+touscruino.getFontHeight(fontSize);
    touscruino.drawLine(0, y, touscruino.getWidth()-1, y, WHITE, 2);
    return x;
}


void showAlarmStatus() {
    // show the alarm
    if (touscruino.getRTC()->isAlarmRinging()) drawAlarmSignal((touscruino.getRTC()->getSecond() % 2) == 0 ? BLACK : RED);
    else if (touscruino.getRTC()->isAlarmSnoozing()) drawAlarmSignal((touscruino.getRTC()->getSecond() % 2) == 0 ? BLACK : ORANGE);
    else drawAlarmSignal(BLACK);
}

void drawAlarmSignal(int color) {
    touscruino.fillRectangle(2, ALARM_Y, 128-ALARM_Y-2, 128-ALARM_Y-2, color);
    touscruino.fillRectangle(160-128+ALARM_Y, ALARM_Y, 128-ALARM_Y-2, 128-ALARM_Y-2, color);
}


/**
 * Uncomment the following flock and replace touscruino.drawBigDigit by drawBigDigit in the code above
 **/
/*
void drawBigDigit(uint8_t d, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint8_t thickness, uint8_t style) {
    // we draw rounded digits
    // assume that height > 2*width
    // thickness should be odd
    uint16_t hw = (width - 2 * thickness)/2;
    uint16_t hh = (height - 2 * thickness)/2;
    uint16_t ht = thickness/2;
    uint16_t r = hw + ht;
    uint16_t cx = x + thickness + hw;
    uint16_t c1y = y + thickness + hw - 1, 
        c2y = y + ht + hh - hw, 
        c3y = y + hh + ht + 1, 
        c4y = y + hh + hw + thickness, 
        c5y = y - thickness + height - hw,
        c6y = y + thickness + 2 * hw + 1,
        c9y = y - thickness + height - 2 * hw - 2;
    uint16_t l1x = x + ht,
        l2x = x + width - ht;
    uint16_t l1y = y + ht,
        l2y = y + height - ht - 1;

    switch (d) {
        case 0:
            touscruino.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            touscruino.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            touscruino.drawLine(l1x, c1y, l1x, c5y, color, thickness);
            touscruino.drawLine(l2x, c1y, l2x, c5y, color, thickness);
            break;
        case 1:
            touscruino.drawArc(l1x, l1y, r, GRAPHICS_ARC_SE, color, thickness);
            touscruino.drawLine(cx, l1y, cx, l2y, color, thickness);
            touscruino.drawLine(l1x, l2y, l2x, l2y, color, thickness);
            break;
        case 2:
            touscruino.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            touscruino.drawArc(cx, c2y, r, GRAPHICS_ARC_SE, color, thickness);
            touscruino.drawArc(cx, c4y, r, GRAPHICS_ARC_NW, color, thickness);
            touscruino.drawLine(l2x, c1y, l2x, c2y, color, thickness);
            touscruino.drawLine(l1x, c4y, l1x, l2y, color, thickness);
            touscruino.drawLine(l1x, l2y, l2x, l2y, color, thickness);
            break;
        case 3:
            touscruino.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            touscruino.drawArc(cx, c2y, r, GRAPHICS_ARC_SE, color, thickness);
            touscruino.drawArc(cx, c4y, r, GRAPHICS_ARC_NE, color, thickness);
            touscruino.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            touscruino.drawLine(l2x, c1y, l2x, c2y, color, thickness);
            touscruino.drawLine(l2x, c4y, l2x, c5y, color, thickness);
            break;
        case 4:
            touscruino.drawLine(cx, l1y, l1x, c4y, color, thickness);
            touscruino.drawLine(l1x, c4y, l2x, c4y, color, thickness);
            touscruino.drawLine(cx, c3y, cx, l2y, color, thickness);
            break;
        case 5:
            touscruino.drawArc(cx, c4y, r, GRAPHICS_ARC_NE, color, thickness);
            touscruino.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            touscruino.drawLine(l2x, c4y, l2x, c5y, color, thickness);
            touscruino.drawLine(l1x, l1y, l2x, l1y, color, thickness);
            touscruino.drawLine(l1x, l1y, l1x, c3y, color, thickness);
            touscruino.drawLine(l1x, c3y, cx, c3y, color, thickness);
            break;
        case 6:
            touscruino.drawArc(cx, c4y, r, GRAPHICS_ARC_NE, color, thickness);
            touscruino.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            touscruino.drawArc(l2x-ht, c6y, r*2, GRAPHICS_ARC_NW, color, thickness);
            touscruino.drawLine(l2x, c4y, l2x, c5y, color, thickness);
            touscruino.drawLine(l1x, c3y, cx, c3y, color, thickness);
            touscruino.drawLine(l1x, c2y, l1x, c5y, color, thickness);
            break;
        case 7:
            touscruino.drawArc(l2x, c3y, r, GRAPHICS_ARC_NW, color, thickness);
            touscruino.drawLine(l2x, l1y, l2x, c3y-r, color, thickness);
            touscruino.drawLine(l1x, l1y, l2x, l1y, color, thickness);
            touscruino.drawLine(cx, c3y, cx, l2y, color, thickness);
            break;
        case 8:
            touscruino.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            touscruino.drawArc(cx, c2y, r, GRAPHICS_ARC_S, color, thickness);
            touscruino.drawArc(cx, c4y, r, GRAPHICS_ARC_N, color, thickness);
            touscruino.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            touscruino.drawLine(l1x, c1y, l1x, c2y, color, thickness);
            touscruino.drawLine(l1x, c4y, l1x, c5y, color, thickness);
            touscruino.drawLine(l2x, c1y, l2x, c2y, color, thickness);
            touscruino.drawLine(l2x, c4y, l2x, c5y, color, thickness);
            break;
        case 9:
            touscruino.drawArc(cx, c2y, r, GRAPHICS_ARC_SW, color, thickness);
            touscruino.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            touscruino.drawArc(l1x+ht, c9y, r*2, GRAPHICS_ARC_SE, color, thickness);
            touscruino.drawLine(cx, c3y, l2x, c3y, color, thickness);
            touscruino.drawLine(l2x, c1y, l2x, c9y, color, thickness);
            touscruino.drawLine(l1x, c1y, l1x, c2y, color, thickness);
            break;
    }
}
*/