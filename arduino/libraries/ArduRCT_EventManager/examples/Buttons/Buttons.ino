/*
 * Buttons - demonstrate the use of the EventManager for pushbuttons
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

#include <ArduRCT_EventManager.h>

#define ON_BUTTON        3
#define OFF_BUTTON       4
#define MOMENTARY_BUTTON 5

#define LED_PIN 13

// define the eventManager
ArduRCT_EventManager eventManager;

void setup() {
    // prepare the led output
    pinMode(LED_PIN, OUTPUT);

    // register some switches
    eventManager.registerSwitch(new ArduRCT_Switch(ON_BUTTON));
    eventManager.registerSwitch(new ArduRCT_Switch(OFF_BUTTON));
    eventManager.registerSwitch(new ArduRCT_Switch(MOMENTARY_BUTTON));
    
    // register an EventHandler for the switches: handleButtons will be called for every EVENT_SWITCH
    eventManager.registerEventHandler(new ArduRCT_EventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, &handleButtons));
}

void loop() {
    eventManager.manageEvents();
}

int8_t handleButtons(uint8_t eventType, uint8_t buttonId) {
    if (eventType == EVENT_SWITCH_PRESSED) {
        if (buttonId == OFF_BUTTON) digitalWrite(13, LOW);            // if the off switch is pressed, switch off the led
        else digitalWrite(LED_PIN, HIGH);                             // else if any other switch is pressed, switch it on
    } else if (eventType == EVENT_SWITCH_RELEASED) {
        if (buttonId == MOMENTARY_BUTTON) digitalWrite(LED_PIN, LOW); // switch off the led if the button is released
    }
    return EVENT_HANDLING_DONE;
}