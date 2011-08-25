/*
 * ButtonMonitor - Monitor buttons with No Interrups 
 *
 * Copyright (c) 2011 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
 #include <ButtonMonitor.h>

#define BUTTON1 0
#define BUTTON1_PIN 2
#define BUTTON2 1
#define BUTTON2_PIN 3

#define LED 13

void setup() {
    // Monitor the buttons
    ButtonMonitor.add(BUTTON1, BUTTON1_PIN);
    ButtonMonitor.add(BUTTON2, BUTTON2_PIN);
    // set the led as an output
	pinMode(LED, OUTPUT);
}

void loop() {
    // monitor the buttons: needs to be called at least once per BUTTON_MONITOR_DEBOUNCE_DELAY_MS (50ms)
    ButtonMonitor.update();
    // wait between command reads
    delay(5);
    // adjust the led according to the command
	if (ButtonMonitor.isDepressed(BUTTON1)) digitalWrite(LED, HIGH);
	if (ButtonMonitor.isDepressed(BUTTON2)) digitalWrite(LED, LOW);
}