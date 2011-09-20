/*
 * ServoManager - Running as many servos as there are pins with Timer1 Interrups 
 *   This breaks pwm functions associated with timer 1: pin 9 and pin 10
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
 
 #include <ServoManager.h>

#define NB_SERVOS 8
#define FIRST_SERVO_PIN 4
#define SERIAL_BAUDRATE 19200

#define PULSE_PIN 14

uint8_t index = 0;

void setup() {
    // Initialize the Serial
    Serial.begin(SERIAL_BAUDRATE);
    // Setup a pulse pin to be high whenever at least one servo is pulsed
    ServoManager.setup(PULSE_PIN);
    // Attach the servos to the pins
    for (uint8_t i=0; i<NB_SERVOS; i++) ServoManager.attach(i, FIRST_SERVO_PIN+i);
    ServoManager.debug();
}

void loop() {
    // read the Serial to adjust the positions
    if (Serial.available()) {
        uint8_t val = Serial.read();
        // if Clear, center all servos
        if (val == 'C') {
            for (uint8_t i=0; i<NB_SERVOS; i++) ServoManager.setAngle(i, 90);
            index = 0;
        // else use a to z to set the position of the servo
        } else if ((val >= 'a') && (val <= 'z')) {
            ServoManager.setAngle(index, ((uint8_t)val - 'a') * 7);
            index++;
        }
        if (index == NB_SERVOS) index = 0;
        if (index == 0) ServoManager.debug();
    }
}