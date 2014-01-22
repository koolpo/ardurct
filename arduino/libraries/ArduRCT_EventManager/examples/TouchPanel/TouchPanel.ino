/*
 * TouchPanel - demonstrate the use of the EventManager for a touch panel
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

#include <Wire.h>
#include <ArduRCT_EventManager.h>

#define LED_PIN 13
#define TP_INT_PIN 30
#define TP_DRAG_TRIGGER 40

// define the eventManager
ArduRCT_EventManager eventManager;
// define the touchpanel
ArduRCT_TouchPanel touchpanel;
// define an event handler for the touch panel
ArduRCT_EventHandler touchpanelHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, 0, 0, &handleTouchPanel)

void setup() {
    Serial.begin(57600);
    
    // prepare the led output
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // register a TouchPanel
    eventManager.registerTouchPanel(&touchpanel);
    
    // register an EventHandler for the panel: handleTouchPanel will be called for every EVENT_TOUCHPANEL
    eventManager.registerEventHandler(&touchpanelHandler);
}

void loop() {
    eventManager.manageEvents();
}

int8_t handleTouchPanel(uint8_t eventType, uint8_t z, int16_t x, int16_t y) {
    if (eventType == EVENT_TOUCHPANEL_PRESSED) {
        digitalWrite(LED_PIN, HIGH);
        Serial.print("P "); Serial.print(x); Serial.print(" "); Serial.println(y);
    } else if (eventType == EVENT_TOUCHPANEL_DRAGGED) {
        Serial.print("D "); Serial.print(x); Serial.print(" "); Serial.println(y);
    } else {
        digitalWrite(LED_PIN, LOW);
        Serial.println("R");
    }
    return EVENT_HANDLING_DONE;  
}