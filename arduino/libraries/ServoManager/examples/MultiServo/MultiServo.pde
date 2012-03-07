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
#define SERIAL_BAUDRATE 57600

#define FRAME_PIN 14

uint8_t index = 0;

void setup() {
    // Initialize the Serial
    Serial.begin(SERIAL_BAUDRATE);
    // Setup a pulse pin to be high whenever at least one servo pin is high
    ServoManager.setup(FRAME_PIN);
    // Attach the servos to the pins
    for (uint8_t i=0; i<NB_SERVOS; i++) ServoManager.attach(i, FIRST_SERVO_PIN+i, SERVO_MANAGER_SYNCHRONIZE);
    // Apply the changes and print the new positions
    ServoManager.synchronizeChangeRequests();
    ServoManager.debug();
}

void loop() {
    // read the Serial to adjust the positions
    if (Serial.available()) {
        uint8_t val = Serial.read();
        // if Clear, center all servos
        if (val == 'C') {
            // don't apply changes immediately to synchronize them later
            for (uint8_t i=0; i<NB_SERVOS; i++) ServoManager.setAngle(i, 90, SERVO_MANAGER_SYNCHRONIZE);
            index = 0;
        // else use a to z to set the position of the servo
        } else if ((val >= 'a') && (val <= 'z')) {
            // don't apply changes immediately to synchronize them later
            ServoManager.setAngle(index, val == 'z' ? 180 : (val - 'a') * 7, SERVO_MANAGER_SYNCHRONIZE);
            index++;
        }
        if (index == NB_SERVOS) index = 0;
        if (index == 0) {
            // Apply the changes and print the new positions
            ServoManager.synchronizeChangeRequests();
            ServoManager.debug();
        }    
    }
}