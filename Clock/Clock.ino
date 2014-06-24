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
 */

#define GRAPHICS_UI_ELEMENT_FONT FONT_HUGE

#include <Wire.h>
#include <SPI.h>
#include <ArduRCT_EventManager.h>
#include <ArduRCT_Graphics.h>
#include <string.h>
#include <avr/eeprom.h>

#include "pitches.h"
#include "melody.h"

// Change to your version: the following include will automatically create the proper 'Touscruino' object
#define TOUSCRUINO_VERSION 2
#include <ArduRCT_TouScruino.h>

// a speaker should be tied between ground and pin 6 through a small resistor (100 ohm)
#define SPEAKER_PIN 6
// a light sensor should be tied between ground and A0, and a resistor between A5 and VCC
#define LIGHT_SENSOR_PIN A0
// this is the default back_light pin
#define BACKLIGHT_PIN 5

// how many seconds before we return to the TIME screen if no action was taken
#define RETURN_TO_HOME_SCREEN_AFTER 20
// how long to snooze when the snoozing is selected
#define SNOOZE_MINUTES 5
// how many samples to average for the light
#define LIGHT_AVERAGING 10

#define HOME_SCREEN 1
#define SET_DATE_SCREEN 2
#define SET_ALARMS_SCREEN 3
#define SET_TIME_SCREEN 4
#define SET_ALARM_TIME_SCREEN 5 // 5 to 12

#define DATE 1
#define TIME 2
#define ALARM_TIME 3
#define ALARM_TOGGLE 4
#define ALARM_MELODY 5

#define DAY_MINUS 10
#define DAY_ADJUST 11
#define DAY_PLUS 12
#define MONTH_JAN 13
#define MONTH_FEB 14
#define MONTH_MAR 15
#define MONTH_APR 16
#define MONTH_MAY 17
#define MONTH_JUN 18
#define MONTH_JUL 19
#define MONTH_AUG 20
#define MONTH_SEP 21
#define MONTH_OCT 22
#define MONTH_NOV 23
#define MONTH_DEC 24
#define YEAR_MINUS 25
#define YEAR_ADJUST 26
#define YEAR_PLUS 27
#define DAY_OF_WEEK 28

#define ALARM_SUN 39
#define ALARM_MON 40
#define ALARM_TUE 41
#define ALARM_WED 42
#define ALARM_THU 43
#define ALARM_FRI 44
#define ALARM_SAT 45
#define ALARM_ICON 46

#define HOUR_PLUS 50
#define MINUTE_PLUS 51
#define HOUR_MINUS 52
#define MINUTE_MINUS 53
#define TIME_ADJUST 54
#define TIME_TITLE 55

#define VALIDATE 100

ArduRCT_GraphicsUIScreen homeScreen;
ArduRCT_GraphicsUIElement date(DATE, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement time(TIME, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement alarm_time(ALARM_TIME, &drawElement, &handleAction);
ArduRCT_GraphicsUIToggle alarm_toggle(ALARM_TOGGLE, 0, &handleAction);
ArduRCT_GraphicsUIElement alarm_melody(ALARM_MELODY, &drawElement, &handleAction);

ArduRCT_GraphicsUIScreen setDateScreen;
ArduRCT_GraphicsUIButton day_minus(DAY_MINUS, "-", &handleAction);
ArduRCT_GraphicsUIElement day_adjust(DAY_ADJUST, &drawElement);
ArduRCT_GraphicsUIButton day_plus(DAY_PLUS, "+", &handleAction);
ArduRCT_GraphicsUIOption month_jan(MONTH_JAN, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_feb(MONTH_FEB, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_mar(MONTH_MAR, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_apr(MONTH_APR, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_may(MONTH_MAY, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_jun(MONTH_JUN, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_jul(MONTH_JUL, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_aug(MONTH_AUG, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_sep(MONTH_SEP, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_oct(MONTH_OCT, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_nov(MONTH_NOV, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIOption month_dec(MONTH_DEC, &drawElement, &handleAction, 1);
ArduRCT_GraphicsUIButton year_minus(YEAR_MINUS, "-", &handleAction);
ArduRCT_GraphicsUIElement year_adjust(YEAR_ADJUST, &drawElement);
ArduRCT_GraphicsUIButton year_plus(YEAR_PLUS, "+", &handleAction);
ArduRCT_GraphicsUIElement day_of_week(DAY_OF_WEEK, &drawElement);

ArduRCT_GraphicsUIScreen setAlarmsScreen;
ArduRCT_GraphicsUIElement alarm_sun(ALARM_SUN, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement alarm_mon(ALARM_MON, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement alarm_tue(ALARM_TUE, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement alarm_wed(ALARM_WED, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement alarm_thu(ALARM_THU, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement alarm_fri(ALARM_FRI, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement alarm_sat(ALARM_SAT, &drawElement, &handleAction);
ArduRCT_GraphicsUIElement alarm_icon(ALARM_ICON, &drawElement);

ArduRCT_GraphicsUIScreen setTimeScreen;
ArduRCT_GraphicsUIButton hour_plus(HOUR_PLUS, "+", &handleAction);
ArduRCT_GraphicsUIButton minute_plus(MINUTE_PLUS, "+", &handleAction);
ArduRCT_GraphicsUIButton hour_minus(HOUR_MINUS, "-", &handleAction);
ArduRCT_GraphicsUIButton minute_minus(MINUTE_MINUS, "-", &handleAction);
ArduRCT_GraphicsUIElement time_adjust(TIME_ADJUST, &drawElement);
ArduRCT_GraphicsUIElement time_title(TIME_TITLE, &drawElement);

ArduRCT_GraphicsUIButton validate(VALIDATE, &drawElement, &handleAction);
    
// define one handler which will draw the date and time every time an EVENT_TIME_* occurs
ArduRCT_EventHandler timeEventHandler(EVENT_TIME, &handleTimeEvent);
// define one handler which will play the melody
ArduRCT_EventHandler alarmEventHandler(EVENT_TIME_ALARM, &handleAlarmEvent);

// define an analog sensor for the light
ArduRCT_Analog light(LIGHT_SENSOR_PIN);
// and its handler
ArduRCT_EventHandler lightChangeEventHandler(EVENT_ANALOG, EVENT_ANY_VALUE, 0, 0, &handleLightEvent);

uint8_t screenId = HOME_SCREEN;
uint8_t watchdog = 0;
int32_t nextNoteTime = 0;
int8_t noteIndex = -1;
bool notePlaying = false; 
uint8_t backlight = 127;
uint8_t timeDigits[] = { 0xFF, 0xFF, 0xFF, 0xFF }; 
uint8_t day, month;
uint16_t year;
uint8_t alarmOfDOW = 0;
uint8_t lightValueIndex = 0xFF;
uint16_t lightValues[LIGHT_AVERAGING];

void setup() {
    Touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    Touscruino.fillScreen(BLACK);
    Touscruino.setRotation(GRAPHICS_ROTATION_90);
    Touscruino.enableGraphicsUI();

    // define the outputs
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    setFrequency(1000);
    setBacklight(backlight);
    
    // register the sensors
//    Touscruino.registerAnalog(&light);
    // register the handlers
    Touscruino.registerEventHandler(&timeEventHandler);
    Touscruino.registerEventHandler(&alarmEventHandler);
//    Touscruino.registerEventHandler(&lightChangeEventHandler);

    // connect to the external RTC chip 
//    Touscruino.getRTC()->setHasMCP7941x(true);
    // get the alarms from eeprom
    Touscruino.getRTC()->useEEPROMToStoreAlarms(0x80);
    // set off the alarms
    for (int i=0; i<7; i++) Touscruino.getRTC()->setDOWAlarmOff(i);
    
    // make sure the time has been updated once before we draw
    Touscruino.getRTC()->updateTime();
    
    // setup the UI screens
    setupHomeScreen();
    setupSetDateScreen();
    setupSetAlarmsScreen();
    setupSetTimeScreen();
    // display the home screen
    setScreen(HOME_SCREEN);
}

void loop() {
    // this function will check if any event happened since last check
    // and call relevant EventHandler's if required
    Touscruino.manageEvents();
    // continue to play the melody
    continueAlarm();
}

void setupHomeScreen() {
    Touscruino.addScreen(&homeScreen);
    homeScreen.addElement(&date, 4, 4, 316, 40);
    homeScreen.addElement(&time, 4, 50, 312, 140);
    homeScreen.addElement(&alarm_toggle, 245, 198, 65, 35);
    homeScreen.addElement(&alarm_melody, 4, 195, 45, 40);
    homeScreen.addElement(&alarm_time, 50, 195, 180, 40);
}

void setupSetDateScreen() {
    Touscruino.addScreen(&setDateScreen);
    setDateScreen.addElement(&day_minus, 135, 20, 50, GUI_AS);
    setDateScreen.addElement(&day_adjust, GUI_ROPWM, GUI_SAP, 50, GUI_SAP);
    setDateScreen.addElement(&day_plus, GUI_ROPWM, GUI_SAP, 50, GUI_SAP);
    setDateScreen.addElement(&day_of_week, 20, GUI_SAP, 80, GUI_SAP);
    setDateScreen.addElement(&month_jan, 20, 70, 70, 36);
    setDateScreen.addElement(&month_feb, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_mar, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_apr, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_may, 20, GUI_BOP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_jun, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_jul, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_aug, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_sep, 20, GUI_BOP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_oct, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_nov, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&month_dec, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    setDateScreen.addElement(&year_minus, 20, 190, 50, GUI_AS);
    setDateScreen.addElement(&year_adjust, GUI_ROPWM, GUI_SAP, 80, GUI_SAP);
    setDateScreen.addElement(&year_plus, GUI_ROPWM, GUI_SAP, 50, GUI_SAP);
    setDateScreen.addElement(&validate, 262, 200, GUI_SAP, GUI_SAP);
    
    setDateScreen.getElement(MONTH_JAN+Touscruino.getRTC()->getMonth()-1)->setValue(GRAPHICS_UI_PRESSED);
    day_of_week.editable = false;
    day_adjust.editable = false;
    year_adjust.editable = false;
//    day_minus.repeatable = true;
//    day_plus.repeatable = true;
//    year_minus.repeatable = true;
//    year_plus.repeatable = true;
}

void setupSetAlarmsScreen() {
    Touscruino.addScreen(&setAlarmsScreen);
    setAlarmsScreen.addElement(&alarm_sun, 60, 3, 180, 34);
    setAlarmsScreen.addElement(&alarm_mon, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    setAlarmsScreen.addElement(&alarm_tue, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    setAlarmsScreen.addElement(&alarm_wed, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    setAlarmsScreen.addElement(&alarm_thu, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    setAlarmsScreen.addElement(&alarm_fri, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    setAlarmsScreen.addElement(&alarm_sat, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    setAlarmsScreen.addElement(&alarm_icon, 10, 10, 45, 45);
    setAlarmsScreen.addElement(&validate);
    
    alarm_icon.editable = false;
}

void setupSetTimeScreen() {
    Touscruino.addScreen(&setTimeScreen);
    setTimeScreen.addElement(&hour_plus, 50, 8, 80, GUI_AS);
    setTimeScreen.addElement(&minute_plus, 190, GUI_SAP, GUI_SAP, GUI_SAP);
    setTimeScreen.addElement(&hour_minus, 50, 150, 80, GUI_AS);
    setTimeScreen.addElement(&minute_minus, 190, GUI_SAP, GUI_SAP, GUI_SAP);
    setTimeScreen.addElement(&time_adjust, 35, 55, 250, 80);
    setTimeScreen.addElement(&time_title, 4, 195, 92, 30);
    setTimeScreen.addElement(&validate);
    
    time_title.editable = false;
//    hour_plus.repeatable = true;
//    minute_plus.repeatable = true;
//    hour_minus.repeatable = true;
//    minute_minus.repeatable = true;
}


bool handleAction(uint8_t id, uint8_t state, int16_t value) {
    watchdog = 0;
    ArduRCT_RealTimeClock *rtc = Touscruino.getRTC();
    if (state == GRAPHICS_UI_RELEASED) {
        if (id == VALIDATE) {
            if (screenId >= SET_ALARM_TIME_SCREEN) setScreen(SET_ALARMS_SCREEN);
            else setScreen(HOME_SCREEN);
        } else if (id == DATE) {
            setScreen(SET_DATE_SCREEN);
        } else if (id == TIME) {
            if (rtc->isAlarmRinging()) snoozeAlarm();
            else setScreen(SET_TIME_SCREEN);
        } else if (id == ALARM_TIME) {
            if (rtc->isAlarmRinging()) stopAlarm();
            else setScreen(SET_ALARMS_SCREEN);
        } else if (id >= ALARM_SUN && id <= ALARM_SAT) {
            setScreen(SET_ALARM_TIME_SCREEN + (id-ALARM_SUN));
        } else if (id >= MONTH_JAN && id <= MONTH_DEC) {
            month = id - MONTH_JAN + 1;
            if (day > rtc->getMonthLastDay(year, month)) day = rtc->getMonthLastDay(year, month);
            setDateScreen.drawElement(DAY_ADJUST);
            setDateScreen.drawElement(DAY_OF_WEEK);
        } else if (id == ALARM_TOGGLE) {
            if (value != 0) {
                for (uint8_t i=0; i<7; i++) rtc->setDOWAlarmOn(i);
            } else {
                if (rtc->isAlarmRinging() || rtc->isAlarmSnoozing()) stopAlarm();
                for (uint8_t i=0; i<7; i++) rtc->setDOWAlarmOff(i);
            }
        }
    }
    if (state == GRAPHICS_UI_RELEASED || state == GRAPHICS_UI_REPEATED) {
        if (id == DAY_PLUS || id == DAY_MINUS) {
            if (day == rtc->getMonthLastDay(year, month) && id == DAY_PLUS) day = 1;
            else if (day == 1 && id == DAY_MINUS) day = rtc->getMonthLastDay(year, month);
            else day += (id == DAY_PLUS ? 1 : -1);
            setDateScreen.drawElement(DAY_ADJUST);
            setDateScreen.drawElement(DAY_OF_WEEK);
        } else if (id == YEAR_PLUS || id == YEAR_MINUS) {
            year += (id == YEAR_PLUS ? 1 : -1);
            if (day > rtc->getMonthLastDay(year, month)) day = rtc->getMonthLastDay(year, month);
            setDateScreen.drawElement(DAY_ADJUST);
            setDateScreen.drawElement(DAY_OF_WEEK);
            setDateScreen.drawElement(YEAR_ADJUST);
        } else if (id == HOUR_PLUS || id == HOUR_MINUS) {
            uint8_t hour = timeDigits[0]*10 + timeDigits[1];
            if (id == HOUR_PLUS && hour == 23) hour = 0;
            else if (id == HOUR_MINUS && hour == 0) hour = 23;
            else hour += (id == HOUR_PLUS ? 1 : -1);
            timeDigits[0] = hour / 10;
            timeDigits[1] = hour % 10;
            setTimeScreen.drawElement(TIME_ADJUST);
        } else if (id == MINUTE_PLUS || id == MINUTE_MINUS) {
            uint8_t minute = timeDigits[2]*10 + timeDigits[3];
            if ((id == MINUTE_PLUS) && (minute == 59)) minute = 0;
            else if ((id == MINUTE_MINUS) && (minute == 0)) minute = 59;
            else minute += (id == MINUTE_PLUS ? 1 : -1);
            timeDigits[2] = minute / 10;
            timeDigits[3] = minute % 10;
            setTimeScreen.drawElement(TIME_ADJUST);
        }
    }
    return true;
}

void setScreen(uint8_t newScreenId) {
    ArduRCT_RealTimeClock *rtc = Touscruino.getRTC();
    uint8_t oldScreenId = screenId;
    screenId = newScreenId;
    uint8_t hour = timeDigits[0]*10 + timeDigits[1];
    uint8_t minute = timeDigits[2]*10 + timeDigits[3];
    if (screenId == HOME_SCREEN) {
        // save modified values
        if (oldScreenId == SET_DATE_SCREEN) rtc->setDate(year, month, day);
        else if (oldScreenId == SET_TIME_SCREEN) rtc->setTime(hour, minute, 0);
        else if (oldScreenId >= SET_ALARM_TIME_SCREEN) rtc->setDOWAlarmTime(oldScreenId-SET_ALARM_TIME_SCREEN, hour, minute, alarm_toggle.getValue());
        // go to the new screen
        for (int i=0; i<4; i++) timeDigits[i] = 0xFF;
        Touscruino.setGraphicsUIScreen(&homeScreen);
    } else if (screenId == SET_DATE_SCREEN) {
        // prepare the screen
        day = rtc->getDay();
        month = rtc->getMonth();
        year = rtc->getYear();
        Touscruino.setGraphicsUIScreen(&setDateScreen);
    } else if (screenId == SET_ALARMS_SCREEN) {
        if (oldScreenId >= SET_ALARM_TIME_SCREEN) rtc->setDOWAlarmTime(oldScreenId-SET_ALARM_TIME_SCREEN, hour, minute, alarm_toggle.getValue());
        Touscruino.setGraphicsUIScreen(&setAlarmsScreen);
    } else if (screenId >= SET_TIME_SCREEN) {
        if (screenId > SET_TIME_SCREEN) {
             for (int i=0; i<4; i++) timeDigits[i] = rtc->getDOWAlarmDigit(screenId-SET_ALARM_TIME_SCREEN, i);
        }
        Touscruino.setGraphicsUIScreen(&setTimeScreen);
    } 
}

void drawElement(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    ArduRCT_RealTimeClock *rtc = Touscruino.getRTC();
    char *dateAsString = rtc->getDateAsString(year, month, day, RTC_WITH_DAY_OF_WEEK, 0);
    int bgColor = (state >= GRAPHICS_UI_PRESSED ?  GRAPHICS_UI_COLOR_RELEASED : WHITE);
    if (id == VALIDATE) {
        Touscruino.drawLine(x+13, y+17, x+20, y+25, state >= GRAPHICS_UI_PRESSED ? GREEN : 0x06E0, 4);
        Touscruino.drawLine(x+20, y+25, x+37, y+8, state >= GRAPHICS_UI_PRESSED ? GREEN : 0x06E0, 4);
    } else if (id == DATE) {
        Touscruino.fillRectangle(x, y, width, height, bgColor);
        Touscruino.setBackgroundColor(bgColor);
        Touscruino.drawString(rtc->getDateAsString(RTC_WITH_DAY_OF_WEEK), x+16, x+6, state >= GRAPHICS_UI_REPEATED ? BLUE: BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
        Touscruino.setBackgroundColor(WHITE);
    } else if (id == DAY_OF_WEEK) {
        Touscruino.fillRectangle(x, y, width, height, WHITE);
        Touscruino.drawString(dateAsString, x+10, y+8, BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
    } else if (id == DAY_ADJUST) {
        Touscruino.fillRectangle(x, y, width, height, WHITE);
        Touscruino.drawString(&dateAsString[RTC_DATE_STRING_DAY], x+10, y+8, BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
    } else if ((id >= MONTH_JAN) && (id <= MONTH_DEC)) {
        Touscruino.drawString(rtc->getMonthAsString(id-MONTH_JAN+1), x+8, y+6, BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
    } else if (id == YEAR_ADJUST) {
        Touscruino.fillRectangle(x, y, width, height, WHITE);
        Touscruino.drawString(&dateAsString[RTC_DATE_STRING_YEAR], x+5, y+8, BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
    } else if (id == TIME) {
        if (bgColor != WHITE) Touscruino.fillRectangle(x, y, width, height, bgColor); 
        uint16_t sColor = ((rtc->getSecond() % 2) == 1 ? BLACK : bgColor);
        Touscruino.fillRectangle(x+148, y+38, 15, 15, sColor);
        Touscruino.fillRectangle(x+148, y+86, 15, 15, sColor);
        drawTimeDigit(RTC_HOUR10, x, y+10, bgColor);
        drawTimeDigit(RTC_HOUR1, x+72, y+10, bgColor);
        drawTimeDigit(RTC_MINUTE10, x+174, y+10, bgColor);
        drawTimeDigit(RTC_MINUTE1, x+246, y+10, bgColor);
    } else if (id == ALARM_TIME) {
        Touscruino.fillRectangle(x, y, width, height, bgColor);
        alarmOfDOW = getAlarmDOWToDisplay();
        Touscruino.setBackgroundColor(bgColor);
        Touscruino.drawString(rtc->getDOWAlarmTimeAsString(alarmOfDOW, true), x+10, y+10, BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
        Touscruino.setBackgroundColor(WHITE);
    } else if (id == ALARM_MELODY) {
        Touscruino.fillRectangle(x, y, width, height, bgColor);
        drawAlarmIcon(x+12, y+20);
    } else if (id == TIME_TITLE) {
        if (screenId == SET_TIME_SCREEN) Touscruino.drawString("Time", x+10, y+10, BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
        else {
            Touscruino.drawString(rtc->getDOWAsString(screenId-SET_ALARM_TIME_SCREEN), x+55, y+10, BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
            drawAlarmIcon(x+12, y+20);
        }
    } else if (id == ALARM_ICON) {
        Touscruino.fillRectangle(x, y, width, height, WHITE);
        drawAlarmIcon(x+12, y+20);
    } else if (id >= ALARM_SUN && id <= ALARM_SAT) {
        Touscruino.fillRectangle(x, y, width, height, bgColor);
        Touscruino.setBackgroundColor(bgColor);
        Touscruino.drawString(rtc->getDOWAlarmTimeAsString(id-ALARM_SUN, true), x+10, y+6, BLACK, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
        Touscruino.setBackgroundColor(WHITE);
    } else if (id == TIME_ADJUST) {
        Touscruino.fillRectangle(x, y, width, height, WHITE);
        if (timeDigits[0] != 0) Touscruino.drawBigDigit(timeDigits[0], x, y, 50, 80, BLACK, 14, GRAPHICS_STYLE_ADVANCED);
        Touscruino.drawBigDigit(timeDigits[1], x+60, y, 50, 80, BLACK, 14, GRAPHICS_STYLE_ADVANCED);
        Touscruino.drawBigDigit(timeDigits[2], x+140, y, 50, 80, BLACK, 14, GRAPHICS_STYLE_ADVANCED);
        Touscruino.drawBigDigit(timeDigits[3], x+200, y, 50, 80, BLACK, 14, GRAPHICS_STYLE_ADVANCED);
        Touscruino.fillRectangle(x+120, y+17, 10, 10, BLACK);
        Touscruino.fillRectangle(x+120, y+51, 10, 10, BLACK);
    }
}

void drawTimeDigit(int digit, int x, int y, int bgColor) {
    if (timeDigits[digit-RTC_HOUR10] != Touscruino.getRTC()->getTimeDigit(digit) || timeDigits[digit-RTC_HOUR10] == 0xFF || bgColor != WHITE) {
        if (bgColor == WHITE && timeDigits[digit-RTC_HOUR10] != 0xFF) Touscruino.fillRectangle(x, y, 64, 120, WHITE); 
        timeDigits[digit-RTC_HOUR10] = Touscruino.getRTC()->getTimeDigit(digit);
        if (digit == RTC_HOUR10 && timeDigits[digit-RTC_HOUR10] == 0) return;
        Touscruino.drawBigDigit(timeDigits[digit-RTC_HOUR10], x, y, 64, 120, BLACK, 18, GRAPHICS_STYLE_ADVANCED);
    }
}

void drawAlarmIcon(int x, int y) {
    uint16_t color = BLACK;
    if (Touscruino.getRTC()->isAlarmRinging()) color = RED;
    else if (Touscruino.getRTC()->isAlarmSnoozing()) color = BLUE;
    Touscruino.drawRectangle(x-8, y-8, 8, 16, color, 2);
    Touscruino.drawLine(x, y-8, x+6, y-14, color, 2);
    Touscruino.drawLine(x, y+8, x+6, y+14, color, 2);
    Touscruino.drawLine(x+6, y-14, x+6, y+14, color, 2);
    if (Touscruino.getRTC()->isAlarmRinging() || screenId == SET_ALARMS_SCREEN) {
        drawNote(x+14, y+2, color);
        drawNote(x+22, y, color);
    } else if (Touscruino.getRTC()->isAlarmSnoozing()) {
        Touscruino.drawString("Z", x+14, y-2, color, FONT_MEDIUM);
        Touscruino.drawString("Z", x+22, y-6, color, FONT_SMALL);
    }
}

void drawNote(int x, int y, int color) {
    Touscruino.fillRectangle(x, y, 4, 2, color);
    Touscruino.fillRectangle(x+1, y-1, 2, 4, color);
    Touscruino.drawLine(x+3, y-11, x+3, y, color, 1);
    Touscruino.drawLine(x+3, y-11, x+6, y-11, color, 1);
}

uint8_t getAlarmDOWToDisplay() {
    ArduRCT_RealTimeClock *rtc = Touscruino.getRTC();
    uint8_t dow = rtc->getDOW();
    if (rtc->isAlarmSnoozing() || rtc->isAlarmRinging()) return dow;
    rtcAlarm_t *alarmTime = rtc->getDOWAlarmTime(dow);
    uint8_t hour = (alarmTime->hour & ~RTC_ALARM_ON);
    if (rtc->getHour() > hour) dow ++;
    else if (rtc->getHour() == hour && rtc->getMinute() > alarmTime->minute) dow ++;
    if (dow > 6) dow = 0;
    return dow;
}

int8_t handleTimeEvent(uint8_t eventId) {
    if (screenId != HOME_SCREEN) {
        if (eventId != EVENT_TIME_SECOND) return EVENT_HANDLING_DONE; 
        watchdog ++;
        if (watchdog > RETURN_TO_HOME_SCREEN_AFTER) setScreen(HOME_SCREEN);
    } else {
        if (eventId == EVENT_TIME_SECOND) homeScreen.drawElement(TIME);
        else if (eventId == EVENT_TIME_MINUTE && alarmOfDOW != getAlarmDOWToDisplay()) homeScreen.drawElement(ALARM_TIME);
        else if (eventId == EVENT_TIME_DAY) homeScreen.drawElement(DATE);
    }
    return EVENT_HANDLING_DONE;
}

int8_t handleLightEvent(uint8_t eventId, uint8_t pin, int16_t value, int16_t change) {
    if (lightValueIndex == 0xFF) {
        for (uint8_t i=0; i<LIGHT_AVERAGING; i++) lightValues[i] = value;
    }
    lightValueIndex ++;
    if (lightValueIndex == LIGHT_AVERAGING) lightValueIndex = 0;
    lightValues[lightValueIndex] = value;
    uint16_t lightValue = 0;
    for (uint8_t i=0; i<LIGHT_AVERAGING; i++) lightValue += lightValues[i];
    lightValue = lightValue / LIGHT_AVERAGING;
    lightValue = map(lightValue, 0, 512, 5, 250);
    if (lightValue < 5 || lightValue > 250) return EVENT_HANDLING_DONE;
    if (abs(backlight-lightValue) < 3) return EVENT_HANDLING_DONE;
    if (!Touscruino.getRTC()->isAlarmRinging()) setBacklight(lightValue);
    return EVENT_HANDLING_DONE;
}

int8_t handleAlarmEvent(uint8_t eventId) {
    startAlarm();
    return EVENT_HANDLING_DONE;
}

void startAlarm() {
    setBacklight(255);
    uint32_t noteDuration = (melodyNoteDurations[0] * MELODY_SPEED_DIV) / MELODY_SPEED_MUL;
    setFrequency(melodyNotes[0]);
    nextNoteTime = millis() + noteDuration;
    noteIndex = 0;
    notePlaying = true;
    setSpeakerOn(true);
    if (screenId != HOME_SCREEN) setScreen(HOME_SCREEN);
    homeScreen.drawElement(ALARM_MELODY);
}

void continueAlarm() {
    // if the RTC has judged the alarm rang too long, stop it
    if (!Touscruino.getRTC()->isAlarmRinging() && noteIndex != -1) {
        stopAlarm();
        return;
    }
    // if not started, can not continue
    if (noteIndex == -1) return;
    if ((int32_t)(millis() - nextNoteTime) < 0) return;

    uint32_t noteDuration;
    if (notePlaying) {
        // we have finished the note, we silence
        noteDuration = MELODY_SILENCE * MELODY_SPEED_DIV / MELODY_SPEED_MUL;
        setSpeakerOn(false);
        noteIndex ++;
        if (noteIndex >= MELODY_LENGTH) {
            noteIndex = 0;
            nextNoteTime += 1000 * MELODY_SECONDS_BETWEEN_PLAYS;
        }
    } else {
        noteDuration = melodyNoteDurations[noteIndex];
        noteDuration = noteDuration * 100;
        noteDuration = noteDuration * MELODY_SPEED_DIV / MELODY_SPEED_MUL;
        if (melodyNotes[noteIndex] != 0) {
            setFrequency(melodyNotes[noteIndex]);
            setSpeakerOn(true);
        }
    }  
    notePlaying = !notePlaying;
    nextNoteTime += noteDuration;
}

void stopAlarm() {
    noteIndex = -1;
    setSpeakerOn(false);
    setFrequency(1000);
    setBacklight(backlight);
    Touscruino.getRTC()->stopAlarm();
    homeScreen.drawElement(ALARM_MELODY);
}

void snoozeAlarm() {
    noteIndex = -1;
    setSpeakerOn(false);
    setFrequency(1000);
    setBacklight(backlight);
    Touscruino.getRTC()->snoozeAlarm(SNOOZE_MINUTES);
    for (int i=0; i<4; i++) timeDigits[i] = 0xFF;
    Touscruino.fillRectangle(time.x, time.y, time.width, time.height, WHITE); 
    homeScreen.drawElement(TIME);
    homeScreen.drawElement(ALARM_MELODY);
}

void setFrequency(uint16_t frequency) {
    // disable interrupts
    TIMSK1 = 0;
    // set the OCR to the correct value
    uint32_t ocra = F_CPU / frequency / 64 / 2 - 1;
    // fast pwm mode, divide by 64
    TCCR1A = 0b00000011;
    TCCR1B = 0b00011011;
    // reset the counter
    TCNT1 = 0;
    OCR1A = ocra;
}

void setSpeakerOn(bool on) {
    // toggle SPEAKER_PIN on OCR1A match
    TCCR1A = (TCCR1A & 0b00111111) | (on ? 0b01000000 : 0b00000000);
    if (!on) digitalWrite(SPEAKER_PIN, 0);
}

void setBacklight(uint8_t value) {
    if (value < 5) digitalWrite(BACKLIGHT_PIN, LOW);
    else if (value > 250) digitalWrite(BACKLIGHT_PIN, HIGH);
    else {
        uint32_t ocrb = OCR1A;
        ocrb = (ocrb * value) >> 8;
        OCR1B = ocrb;
        backlight = value;
        // set BACKLIGHT_PIN on OCR1B match, clear it on BOTTOM
        TCCR1A = (TCCR1A & 0b11001111) | (value ? 0b00110000 : 0b00000000);
    }
}

