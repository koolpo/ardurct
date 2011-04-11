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
 * connect it to the digital pins of the Arduino (not necessary if you are
 * using a 3.3V variant of the Arduino, such as Sparkfun's Arduino Pro).
 */


#define CLOCK_CENTER_X	(LCD_PCD8544_WIDTH / 2)
#define CLOCK_CENTER_Y	(LCD_PCD8544_HEIGHT / 2)
#define CLOCK_RADIUS	((LCD_PCD8544_HEIGHT / 2) - 2)
#define CLOCK_HAND_S	(CLOCK_RADIUS - 5)
#define CLOCK_HAND_M	(CLOCK_HAND_S * 0.9f)
#define CLOCK_HAND_H	(CLOCK_HAND_S * 0.7f)
#define CLOCK_SKEW	4

LCD_PCD8544_VG lcdG(7);
LCD_PCD8544 lcdT(8);

uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
uint8_t days = 0;


void drawHandles(float angleH, float angleM, float angleS, bool on) {
    lcdG.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_CENTER_X + round((CLOCK_HAND_H+CLOCK_SKEW) * cos(angleH)), CLOCK_CENTER_Y + round(CLOCK_HAND_H * sin(angleH)), on, 3);
    lcdG.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_CENTER_X + round((CLOCK_HAND_M+CLOCK_SKEW) * cos(angleM)), CLOCK_CENTER_Y + round(CLOCK_HAND_M * sin(angleM)), on, 2);
    lcdG.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_CENTER_X + round((CLOCK_HAND_S+CLOCK_SKEW) * cos(angleS)), CLOCK_CENTER_Y + round(CLOCK_HAND_S * sin(angleS)), on);
}
    

void printJustified(uint8_t val, uint8_t space) {
    if (val >= 10) lcdT.print((int)(val/10));
    else lcdT.print(space);
    lcdT.print((int)(val%10));
}


void setup() {

    analogWrite(11, 140);
    
    // initialize the screen
    lcdG.begin(2);  
    lcdT.begin(3);  

    // draw the clock frame, compensate for the display dots not being square
    lcdG.fillEllipse(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_RADIUS+CLOCK_SKEW, CLOCK_RADIUS, ON);
    lcdG.fillEllipse(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_RADIUS+CLOCK_SKEW-2, CLOCK_RADIUS-2, OFF);
	// draw the hour lines
    for (uint8_t i=0; i<12; i++) {
        float angle = (-90.0f + i * 30.0f) * 3.1415f / 180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        lcdG.drawLine(CLOCK_CENTER_X + round((CLOCK_RADIUS+CLOCK_SKEW-4) * ca), CLOCK_CENTER_Y+ round((CLOCK_RADIUS-4) * sa), 
            CLOCK_CENTER_X + round((CLOCK_RADIUS+CLOCK_SKEW) * ca), CLOCK_CENTER_Y+ round(CLOCK_RADIUS * sa), ON);
    }
    lcdG.updateDisplay();
    
    lcdT.setCursor(0, 3);
    lcdT.print("Battery: ");

}

void loop() {
    // calculate the angle in radians
    float angleS = (-90.0f + seconds * 6.0f) * 3.1415f / 180.0f;
    // calculate the angle in radians
    float angleM = (-90.0f + minutes * 6.0f) * 3.1415f / 180.0f;
    // calculate the angle in radians (11h59 = 720mn)
    float angleH = (-90.0f + (60*(hours%12) + minutes) * 0.5f) * 3.1415f / 180.0f;
	
    // draw handles
    drawHandles(angleH, angleM, angleS, ON);
    lcdG.updateDisplay();
    
	// draw the text version
    lcdT.setCursor(1, 1);
    printJustified(days, ' ');
    lcdT.print("d ");
    printJustified(hours, ' ');
    lcdT.print(":");
    printJustified(minutes, '0');
    lcdT.print(":");
    printJustified(seconds, '0');
    
    // wait for 1 second
    delay(1000);
    // erase the previous line, to prepare for next drawing
    drawHandles(angleH, angleM, angleS, OFF);

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


