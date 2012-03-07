/*
 * Terminal
 *	Demonstrating the use of the LCD_PCD8544 library
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
 * Assuming you use a LCD from Sparkfun : http://www.sparkfun.com/products/10168
 *
 * To use this sketch, connect the eight pins from your LCD like thus:
 *		LCD		Arduino
 *		1-VCC	+3.3V
 *		2-GND	GND
 *		3-SCE	Digital pin 7
 *		4-RST	Digital pin 6
 *		5-D/C	Digital pin 5
 *		6-DN	Digital pin 4
 *		7-SCLK	Digital pin 3
 *		8-LED	+3.3V through a 1k resistor
 *
 * Since these LCDs are +3.3V devices, you have to add extra components to
 * connect them to the digital pins of the Arduino (not necessary if you are
 * using a 3.3V variant of the Arduino, such as Sparkfun's Arduino Pro 3V).
 */
 
#include <LCD_PCD8544.h>

// LCD_PCD8544(sce, sclk, sdin, dc, reset);
LCD_PCD8544 lcd;

void setup() {
    Serial.begin(9600);
    lcd.begin();
}

void loop() {
    if (Serial.available() > 0) 
        lcd.print((uint8_t)Serial.read()); 
}
