/*
 * Heading
 *	Demonstrating the use of the HMC6352 library
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
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

/*
 * Assuming you use a module from Sparkfun : http://www.sparkfun.com/products/7915
 *
 * To use this sketch, connect the eight pins from your LCD like thus:
 *		HMC6352	Arduino
 *		GND		GND
 *		VCC		+3.3V
 *		SDA		Analog pin 4 (SDA)
 *		SCL		Analog pin 5 (SCL)
 *
 * Since these modules are +3.3V devices, you have to add extra components to
 * connect it to the digital pins of the Arduino (not necessary if you are
 * using a 3.3V variant of the Arduino, such as Sparkfun's Arduino Pro).
 */

#include <Wire.h>
#include <HMC6352.h>

void setup() {
    // Initialize the magnetometer
    Hmc6352.begin();
    // Initialize the serial port
    Serial.begin(9600);
}

void loop() {
    Serial.print("Heading: ");
    Serial.print(Hmc6352.getHeading());
    delay(500);
}

