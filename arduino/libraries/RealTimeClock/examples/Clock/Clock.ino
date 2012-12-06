/*
 * Clock - Digital clock
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

// include the library code:
#include <RealTimeClock.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// initialize the RealTimeClock
RealTimeClock rtc(2012, DECEMBER, 6, THURSDAY, 9, 30);

void setup() {
    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);
}

void loop() {
    delay(5);
    
    // this will adjust the time for the rtc, if required
    // returns true if a value changed
    // can be called as often as wanted, but at least once per second
    if (!rtc.update()) return;

    // display the values
    lcd.setCursor(0, 0);
    lcd.print(rtc.getTimeAsString(RTC_WITH_SECONDS));
    lcd.setCursor(0, 1);
    lcd.print(rtc.getDateAsString(RTC_WITH_DAY_OF_WEEK));
}