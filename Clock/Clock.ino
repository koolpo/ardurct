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

#include <Wire.h>
#include <SPI.h>
#include <ArduRCT_EventManager.h>
#include <ArduRCT_Graphics.h>
#include <string.h>

#include "pitches.h"
#include "melody.h"

// Change to your version: the following include will automatically create the proper 'Touscruino' object
#define TOUSCRUINO_VERSION 2
#include <ArduRCT_TouScruino.h>

#define SPEAKER_PIN 6
#define BACKLIGHT_PIN 5

// how many seconds before we return to the TIME screen if no action was taken
#define RETURN_TO_TIME_TRIGGER 30

#define SCREEN_TIME         0
#define SCREEN_SET_DATE     1
#define SCREEN_SET_TIME     2
#define SCREEN_SET_ALARM_D1 3
#define SCREEN_SET_ALARM_D7 9

#define DIGIT_STYLE     GRAPHICS_STYLE_ADVANCED
    
uint8_t screen;
uint8_t activeField = 0;
int16_t field[] = { 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t fieldMax[] = { 0, 0, 0 };
uint8_t fieldMin[] = { 0, 0, 0 };
uint8_t nbFields = 0;
uint8_t watchdog = 0;
int32_t nextNoteTime = 0;
int8_t noteIndex = 0;
bool notePlaying = false; 
uint8_t backlight = 127;
int16_t lightValue = 0;

// define one handler which will draw the date and time every time an EVENT_TIME_* occurs
ArduRCT_EventHandler timeEventHandler(EVENT_TIME, &drawDateAndTime);
// define one handler which will play the melody
ArduRCT_EventHandler alarmHandler(EVENT_TIME_ALARM, &startAlarm);
// define one handler for the touchpanel
ArduRCT_EventHandler touchpanelHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, 0, 0, &handleTouchpanel);

// define an analog sensor for the light
ArduRCT_Analog light(A0);
// and its handler
ArduRCT_EventHandler lightHandler(EVENT_ANALOG, EVENT_ANY_VALUE, 0, 0, &handleLight);

void setup() {
    Touscruino.begin(WHITE, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    Touscruino.fillScreen(BLACK);
    Touscruino.setRotation(GRAPHICS_ROTATION_90);
    
    // define the outputs
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    setFrequency(1000);
    setBacklight(127);

    // connect to the external chip 
    Touscruino.getRTC()->setHasMCP7941x(true);
    // set one alarm 1mn after the start time
    Touscruino.getRTC()->setAlarmTime(Touscruino.getRTC()->getDayOfWeek(), Touscruino.getRTC()->getHour(), Touscruino.getRTC()->getMinute()+1, RTC_ALARM_ON);
    
    // register the sensors
    Touscruino.registerAnalog(&light);
    // register the handlers
    Touscruino.registerEventHandler(&timeEventHandler);
    Touscruino.registerEventHandler(&alarmHandler);
    Touscruino.registerEventHandler(&touchpanelHandler);
    Touscruino.registerEventHandler(&lightHandler);

    // draw the time
    screen = SCREEN_TIME;
    drawScreen();
}

void loop() {
    // this function will check if any event happened since last check
    // and call relevant EventHandler's if required
    Touscruino.manageEvents();
    // play the melody if there is an alarm ringing
    if (Touscruino.getRTC()->isAlarmRinging()) continueAlarm();
}

void drawScreen() {
    Touscruino.drawString(Touscruino.getRTC()->getDateAsString(RTC_WITH_DAY_OF_WEEK), 10, 10, WHITE, FONT_HUGE, FONT_BOLD);
    Touscruino.drawString(Touscruino.getRTC()->getAlarmTimeAsString(Touscruino.getRTC()->getDayOfWeek()), 10, 80, WHITE, FONT_MEDIUM, FONT_PLAIN);
    drawDateAndTime(0);
}

int8_t drawDateAndTime(uint8_t eventType) {
    Touscruino.drawString(Touscruino.getRTC()->getTimeAsString(RTC_WITH_SECONDS), 10, 50, WHITE, FONT_HUGE, FONT_BOLD);
}

int8_t handleTouchpanel(uint8_t type, uint8_t value, int16_t x, int16_t y) {
    stopAlarm();
}

int8_t handleLight(uint8_t type, uint8_t pin, int16_t value, int16_t change) {
    if (abs(value-lightValue) < 10) return EVENT_HANDLING_DONE;
    lightValue = value;
    if (!Touscruino.getRTC()->isAlarmRinging()) setBacklight(map(value, 0, 512, 20, 220));
}

int8_t startAlarm(uint8_t eventType) {
    uint32_t noteDuration = melodyNoteDurations[0];
    setFrequency(melodyNotes[0]);
    nextNoteTime = millis() + noteDuration;
    noteIndex = 0;
    notePlaying = true;
    setSpeakerOn(true);
    setBacklight(255);
}

void continueAlarm() {
    if ((int32_t)(millis() - nextNoteTime) < 0) return;
    uint32_t noteDuration = melodyNoteDurations[noteIndex];
    if (notePlaying) {
        // we have finished the note, we silence
        noteDuration = noteDuration / MELODY_NOTE_LENGTH * MELODY_SILENCE_LENGTH;
        setSpeakerOn(false);
        noteIndex ++;
        if (noteIndex >= MELODY_LENGTH) {
            noteIndex = 0;
            nextNoteTime += 1000 * MELODY_SECONDS_BETWEEN_PLAYS;
        }
    } else {
        noteDuration = noteDuration;
        if (melodyNotes[noteIndex] != 0) {
            setFrequency(melodyNotes[noteIndex]);
            setSpeakerOn(true);
        }
    }  
    notePlaying = !notePlaying;
    nextNoteTime += noteDuration;
}

void stopAlarm() {
    Touscruino.getRTC()->stopAlarm();
    setSpeakerOn(false);
    setFrequency(1000);
    setBacklight(backlight);
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

