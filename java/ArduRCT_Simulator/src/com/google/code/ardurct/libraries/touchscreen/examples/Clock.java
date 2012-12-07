package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.RealTimeClock;
import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;

public class Clock extends ArduinoFirmware {
		
	int DIGIT_WIDTH = 32;
	int DIGIT_HEIGHT = 80;
	int DIGIT_THICKNESS = 8;
	int SECONDS_WIDTH = 120;
	int SECONDS_HEIGHT = 6;
	int DIGIT_STYLE = GRAPHICS_STYLE_ADVANCED;

	int TIME_Y = 0;
	int DATE_Y = 89;
	int ALARM_Y = 115;
	int SECONDS_Y = 105;
	int H1_X = 0;
	int H2_X = 38;
	int M1_X = 89;
	int M2_X = 128;
	int COLUMN_X = 79;
	
	int ALARM_STRING_X = 30;
	int ALARM_X = 84;
	int DATE_X = 12;


	RealTimeClock rtc = new RealTimeClock(2012, RealTimeClock.DECEMBER, 31, RealTimeClock.THURSDAY, 23, 45, 0);
	    
	int timeDigit[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	int timeDigit_x[] = { 0, 38, 88, 130 };
	    
	public void setup() {
		Serial.begin(57600);
		
	    touchscreen.begin(WHITE, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.fillScreen(BLACK);
	    //touchscreen.setRotation(GRAPHICS_ROTATION_90);
	     
	    rtc.setAlarmTime(RealTimeClock.THURSDAY, 23, 50, RealTimeClock.RTC_ALARM_ON);
	    rtc.setAlarmTime(RealTimeClock.FRIDAY, 1, 05, RealTimeClock.RTC_ALARM_ON);

	    drawDateAndTime();
	}

	public void loop() {
	    delay(5);
	    
	    if (Serial.available() > 0) {
	    	int read = Serial.read();
	    	if (read == 's') rtc.snoozeAlarm(2);
	    	if (read == 'x') rtc.stopAlarm();
	    }
	    
	    // this will adjust the time for the rtc, if required
	    // returns true if a value changed
	    // can be called as often as wanted, but at least once per second
	    if (!rtc.update()) return;

	    drawDateAndTime();
	    if (rtc.isAlarmRinging()) drawAlarmSignal((rtc.getSecond() % 2) == 0 ? BLACK : RED);
	    else if (rtc.isAlarmSnoozing()) drawAlarmSignal((rtc.getSecond() % 2) == 0 ? BLACK : ORANGE);
	    else drawAlarmSignal(BLACK);
	}

	void drawDateAndTime() {
	    // draw hour and minutes, digit by digit
	    for (int i=0; i<4; i++) {
	        if (timeDigit[i] != rtc.getTimeDigit(i)) {
	            // erase the digit
	        	touchscreen.drawBigDigit(timeDigit[i], timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS, DIGIT_STYLE);
	            // draw the new one
	            if ((i != 0) || (timeDigit[i] != 0)) touchscreen.drawBigDigit(rtc.getTimeDigit(i), timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS, DIGIT_STYLE);
	        }
	    }
	    // blink the seconds
	    if ((rtc.getSecond() % 2) == 0) drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS);
	    else drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS);
	    
	    // draw a progress bar for the seconds
	    if (rtc.getSecond() == 0) touchscreen.fillRectangle((touchscreen.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, SECONDS_WIDTH, SECONDS_HEIGHT, BLACK);
	    else touchscreen.fillRectangle((touchscreen.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, rtc.getSecond() * SECONDS_WIDTH / 60, SECONDS_HEIGHT, YELLOW);

	    // if date as changed, redraw the date and alarm 
	    if (timeDigit[4] != rtc.getDay()) {
	    	touchscreen.drawString(rtc.getDateAsString(RealTimeClock.RTC_WITH_DAY_OF_WEEK), DATE_X, DATE_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	        if (rtc.isAlarmOn(rtc.getDayOfWeek())) {
	        	touchscreen.drawString("Alarm", ALARM_STRING_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	        	touchscreen.drawString(rtc.getAlarmTimeAsString(rtc.getDayOfWeek()), ALARM_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	        } else {
	        	touchscreen.fillRectangle(ALARM_STRING_X, ALARM_Y, 12*touchscreen.getFontCharWidth(FONT_MEDIUM), touchscreen.getFontHeight(FONT_MEDIUM), BLACK);
	        }
	    }

	    // save the information for next redraw
	    for (int i=0; i<4; i++) timeDigit[i] = rtc.getTimeDigit(i);
	    timeDigit[4] = rtc.getDay();  
	}

	void drawColumn(int x, int y, int height, int color, int  thickness) {
	    int ht = thickness/2;
	    touchscreen.fillRectangle(x - ht , y + height/3 - ht, thickness, thickness, color); 
	    touchscreen.fillRectangle(x - ht , y + 2*height/3 - ht, thickness, thickness, color); 
	}
	
	void drawAlarmSignal(int color) {
		touchscreen.fillRectangle(160-128+ALARM_Y, ALARM_Y, 128-ALARM_Y-2, 128-ALARM_Y-2, color);
	}
	
	
	/*
	// assume that height >= 2*width
	private void drawDigit(int d, int x, int y, int width, int height, int color, int thickness) {
		int hw = (width - 2 * thickness)/2;
		int hh = (height - 2 * thickness)/2;
		int ht = thickness/2;
		int r = hw + ht;
		int cx = x + thickness + hw;
		int c1y = y + thickness + hw - 1, 
			c2y = y + ht + hh - hw, 
			c3y = y + hh + ht + 1, 
			c4y = y + hh + hw + thickness, 
			c5y = y - thickness + height - hw,
			c6y = y + thickness + 2 * hw + 1,
			c9y = y - thickness + height - 2 * hw - 2;
		int l1x = x + ht,
			l2x = x + width - ht;
		int l1y = y + ht,
			l2y = y + height - ht - 1;

		switch (d) {
			case 0:
				touchscreen.drawArc(cx, c1y, r, SCREEN_ARC_N, color, thickness);
				touchscreen.drawArc(cx, c5y, r, SCREEN_ARC_S, color, thickness);
				touchscreen.drawLine(l1x, c1y, l1x, c5y, color, thickness);
				touchscreen.drawLine(l2x, c1y, l2x, c5y, color, thickness);
				break;
			case 1:
				touchscreen.drawArc(l1x, l1y, r, SCREEN_ARC_SE, color, thickness);
				touchscreen.drawLine(cx, l1y, cx, l2y, color, thickness);
				touchscreen.drawLine(l1x, l2y, l2x, l2y, color, thickness);
				break;
			case 2:
				touchscreen.drawArc(cx, c1y, r, SCREEN_ARC_N, color, thickness);
				touchscreen.drawArc(cx, c2y, r, SCREEN_ARC_SE, color, thickness);
				touchscreen.drawArc(cx, c4y, r, SCREEN_ARC_NW, color, thickness);
				touchscreen.drawLine(l2x, c1y, l2x, c2y, color, thickness);
				touchscreen.drawLine(l1x, c4y, l1x, l2y, color, thickness);
				touchscreen.drawLine(l1x, l2y, l2x, l2y, color, thickness);
				break;
			case 3:
				touchscreen.drawArc(cx, c1y, r, SCREEN_ARC_N, color, thickness);
				touchscreen.drawArc(cx, c2y, r, SCREEN_ARC_SE, color, thickness);
				touchscreen.drawArc(cx, c4y, r, SCREEN_ARC_NE, color, thickness);
				touchscreen.drawArc(cx, c5y, r, SCREEN_ARC_S, color, thickness);
				touchscreen.drawLine(l2x, c1y, l2x, c2y, color, thickness);
				touchscreen.drawLine(l2x, c4y, l2x, c5y, color, thickness);
				break;
			case 4:
				touchscreen.drawLine(cx, l1y, l1x, c4y, color, thickness);
				touchscreen.drawLine(l1x, c4y, l2x, c4y, color, thickness);
				touchscreen.drawLine(cx, c3y, cx, l2y, color, thickness);
				break;
			case 5:
				touchscreen.drawArc(cx, c4y, r, SCREEN_ARC_NE, color, thickness);
				touchscreen.drawArc(cx, c5y, r, SCREEN_ARC_S, color, thickness);
				touchscreen.drawLine(l2x, c4y, l2x, c5y, color, thickness);
				touchscreen.drawLine(l1x, l1y, l2x, l1y, color, thickness);
				touchscreen.drawLine(l1x, l1y, l1x, c3y, color, thickness);
				touchscreen.drawLine(l1x, c3y, cx, c3y, color, thickness);
				break;
			case 6:
				touchscreen.drawArc(cx, c4y, r, SCREEN_ARC_NE, color, thickness);
				touchscreen.drawArc(cx, c5y, r, SCREEN_ARC_S, color, thickness);
				touchscreen.drawArc(l2x-ht, c6y, r*2, SCREEN_ARC_NW, color, thickness);
				touchscreen.drawLine(l2x, c4y, l2x, c5y, color, thickness);
				touchscreen.drawLine(l1x, c3y, cx, c3y, color, thickness);
				touchscreen.drawLine(l1x, c2y, l1x, c5y, color, thickness);
				break;
			case 7:
				touchscreen.drawArc(l2x, c3y, r, SCREEN_ARC_NW, color, thickness);
				touchscreen.drawLine(l2x, l1y, l2x, c3y-r, color, thickness);
				touchscreen.drawLine(l1x, l1y, l2x, l1y, color, thickness);
				touchscreen.drawLine(cx, c3y, cx, l2y, color, thickness);
				break;
			case 8:
				touchscreen.drawArc(cx, c1y, r, SCREEN_ARC_N, color, thickness);
				touchscreen.drawArc(cx, c2y, r, SCREEN_ARC_S, color, thickness);
				touchscreen.drawArc(cx, c4y, r, SCREEN_ARC_N, color, thickness);
				touchscreen.drawArc(cx, c5y, r, SCREEN_ARC_S, color, thickness);
				touchscreen.drawLine(l1x, c1y, l1x, c2y, color, thickness);
				touchscreen.drawLine(l1x, c4y, l1x, c5y, color, thickness);
				touchscreen.drawLine(l2x, c1y, l2x, c2y, color, thickness);
				touchscreen.drawLine(l2x, c4y, l2x, c5y, color, thickness);
				break;
			case 9:
				touchscreen.drawArc(cx, c2y, r, SCREEN_ARC_SW, color, thickness);
				touchscreen.drawArc(cx, c1y, r, SCREEN_ARC_N, color, thickness);
				touchscreen.drawArc(l1x+ht, c9y, r*2, SCREEN_ARC_SE, color, thickness);
				touchscreen.drawLine(cx, c3y, l2x, c3y, color, thickness);
				touchscreen.drawLine(l2x, c1y, l2x, c9y, color, thickness);
				touchscreen.drawLine(l1x, c1y, l1x, c2y, color, thickness);
				break;
		}
	}
	*/
}
