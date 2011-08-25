/*
 * ServoManager - Running as many servos as there are pins with No Interrups 
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

#define THROTTLE 0
#define THROTTLE_SERVO_PIN 4
#define THROTTLE_COMMAND A0

void setup() {
    // Attach the THROTTLE servo to THROTTLE_SERVO_PIN
    ServoManager.attach(THROTTLE, THROTTLE_SERVO_PIN);
    // set it to 2000 microseconds
    ServoManager.set(THROTTLE, 2000);
}

void loop() {
    // run the servo manager: needs to be called at least once per 20ms (50Hz)
    ServoManager.run();
    // wait between command reads
    delay(5);
    // adjust the servo position according to the command
    ServoManager.mapSet(THROTTLE, analogRead(THROTTLE_COMMAND), 0, 1023);
}