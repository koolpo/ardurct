/*
 * Clock
 *	Demonstrating the use of the LCD_PCD8544_VG library
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

#include <LCD_PCD8544_VG.h>

// Position of the clock
#define CLOCK_CENTER_X	(LCD_PCD8544_WIDTH / 2)
#define CLOCK_CENTER_Y	(LCD_PCD8544_HEIGHT / 2)

// Size of the clock
#define CLOCK_RADIUS	((LCD_PCD8544_HEIGHT / 2) - 2)

// Size of the hands on the clock
#define CLOCK_HAND_S	(CLOCK_RADIUS - 5)
#define CLOCK_HAND_M	(CLOCK_HAND_S * 0.9f)
#define CLOCK_HAND_H	(CLOCK_HAND_S * 0.7f)

// Because the pixels are not square, we increase the size on the horizontal
#define CLOCK_SKEW	4

// LCD_PCD8544(sce, sclk, sdin, dc, reset);
LCD_PCD8544_VG lcd;

uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
uint8_t days = 0;

uint32_t nextLoop;

void drawHands(float angleH, float angleM, float angleS, bool on) {
    lcd.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_CENTER_X + round((CLOCK_HAND_H+CLOCK_SKEW) * cos(angleH)), CLOCK_CENTER_Y + round(CLOCK_HAND_H * sin(angleH)), on, 3);
    lcd.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_CENTER_X + round((CLOCK_HAND_M+CLOCK_SKEW) * cos(angleM)), CLOCK_CENTER_Y + round(CLOCK_HAND_M * sin(angleM)), on, 2);
    lcd.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_CENTER_X + round((CLOCK_HAND_S+CLOCK_SKEW) * cos(angleS)), CLOCK_CENTER_Y + round(CLOCK_HAND_S * sin(angleS)), on);
}

void setup() {
    
    // initialize the screen
    lcd.begin();  

    // draw the clock frame, compensate for the display dots not being square
    lcd.fillEllipse(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_RADIUS+CLOCK_SKEW, CLOCK_RADIUS, ON);
    lcd.fillEllipse(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_RADIUS+CLOCK_SKEW-2, CLOCK_RADIUS-2, OFF);

	// draw the hour lines
    for (uint8_t i=0; i<12; i++) {
        float angle = (-90.0f + i * 30.0f) * 3.1415f / 180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        lcd.drawLine(CLOCK_CENTER_X + round((CLOCK_RADIUS+CLOCK_SKEW-4) * ca), CLOCK_CENTER_Y+ round((CLOCK_RADIUS-4) * sa), 
            CLOCK_CENTER_X + round((CLOCK_RADIUS+CLOCK_SKEW) * ca), CLOCK_CENTER_Y+ round(CLOCK_RADIUS * sa), ON);
    }
	
	// update the screen with all the changes
    lcd.updateDisplay();
	
	nextLoop = millis() + 1000;
}

void loop() {	
    // calculate the angle in radians for seconds
    float angleS = (-90.0f + seconds * 6.0f) * 3.1415f / 180.0f;
    // calculate the angle in radians for minutes
    float angleM = (-90.0f + minutes * 6.0f) * 3.1415f / 180.0f;
    // calculate the angle in radians for hours (11h59 = 720mn)
    float angleH = (-90.0f + (60*(hours%12) + minutes) * 0.5f) * 3.1415f / 180.0f;
	
    // draw hands
    drawHands(angleH, angleM, angleS, ON);
    lcd.updateDisplay();
        
    // wait for 1 second
	while ((int32_t)(millis()-nextLoop) < 0) delayMicroseconds(50);
	nextLoop += 1000;

    // erase the previous lines, to prepare for next drawing
    drawHands(angleH, angleM, angleS, OFF);

    // increase the seconds, minutes, hours and days
    seconds ++;
    if (seconds > 59) {
        seconds = 0;
        minutes ++;
        if (minutes > 59) {
            minutes = 0;
            hours ++;
            if (hours > 24) {
                hours = 0;
                days ++;
            }
        }
    }   
}


