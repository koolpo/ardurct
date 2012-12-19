package com.google.code.ardurct.libraries.touscruino.examples;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_RealTimeClock;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_RealTimeClock.rtcAlarm_t;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruinoV1;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class Clock extends TouscruinoFirmware {

	// ------------------ Java specific ------------------------- //
	private timeHandlerCB drawDateAndTime = new timeHandlerCB();
	private buttonsCB handleButtons = new buttonsCB();
	// ------------------ Java specific ------------------------- //

	static final int RETURN_TO_TIME_TRIGGER = 60;
	
	static final int SCREEN_TIME = 0;
	static final int SCREEN_SET_DATE = 1;
	static final int SCREEN_SET_TIME = 2;
	static final int SCREEN_SET_ALARM_D1 = 3;
	static final int SCREEN_SET_ALARM_D7 = 9;
	
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
	    
	int timeDigit[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	int timeDigit_x[] = { 0, 38, 88, 130 };
	
	int screen;
	int activeField = 0;
	int field[] = { 0xFF, 0xFF, 0xFF, 0xFF };
	int fieldMax[] = { 0, 0, 0 };
	int fieldMin[] = { 0, 0, 0 };
	int nbFields = 0;
	
	int watchdog = 0;
	
	ArduRCT_TouScruinoV1 touscruino = new ArduRCT_TouScruinoV1();
	
	ArduRCT_EventHandler timeEventHandler = new ArduRCT_EventHandler(EVENT_TIME, drawDateAndTime);
	ArduRCT_EventHandler buttonEventHandler = new ArduRCT_EventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, handleButtons);

	public void setup() {		
		touscruino.begin(WHITE, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touscruino.fillScreen(BLACK);
		touscruino.setRotation(GRAPHICS_ROTATION_90);
		
		touscruino.registerEventHandler(timeEventHandler);
		touscruino.registerEventHandler(buttonEventHandler);

		// set the RTC
		touscruino.getRTC().setDateTime(2012, DECEMBER, 31, 23, 45, 0);
		touscruino.getRTC().setAlarmTime(THURSDAY, 23, 50, RTC_ALARM_ON);
		touscruino.getRTC().setAlarmTime(FRIDAY, 1, 05, RTC_ALARM_ON);

		// draw the time
		screen = SCREEN_TIME;
		drawScreen();
	}

	public void loop() {
		delay(5);
	    touscruino.update();
	}

	class buttonsCB implements IEventCallback {
		public boolean handle(int eventType, int buttonId) {
			watchdog = 0;
			if (eventType == EVENT_SWITCH_PRESSED) {
				if (buttonId == TOUSCRUINO_MENU) {
					if (touscruino.getRTC().isAlarmRinging() || touscruino.getRTC().isAlarmSnoozing()) {
						touscruino.getRTC().stopAlarm();
						showAlarmStatus();
					} else {
						if (screen != SCREEN_TIME) {
							// memorize the values
							if (screen == SCREEN_SET_DATE) touscruino.getRTC().setDate(field[0], field[1], field[2]);
							else if (screen == SCREEN_SET_TIME) touscruino.getRTC().setTime(field[0], field[1], 0);
							else if (screen >= SCREEN_SET_ALARM_D1) touscruino.getRTC().setAlarmTime(screen - SCREEN_SET_ALARM_D1, field[0], field[1], field[2]);
						}
						changeScreen();
					}
				} else if (buttonId == TOUSCRUINO_ENTER) {
					if (screen == SCREEN_TIME) return true;
					activeField ++;
					if (activeField >= nbFields) activeField = 0;
					if (screen == SCREEN_SET_DATE) drawDateAdjust();
					if (screen >= SCREEN_SET_TIME) drawTimeAdjust();
					if (screen >= SCREEN_SET_ALARM_D1) drawAlarmAdjust();
				} else {
					if (screen == SCREEN_TIME) {
						if (touscruino.getRTC().isAlarmRinging() || touscruino.getRTC().isAlarmSnoozing()) {
							if (buttonId == TOUSCRUINO_UP) touscruino.getRTC().snoozeAlarm(5);
							if (buttonId == TOUSCRUINO_DOWN) touscruino.getRTC().snoozeAlarm(10);
							showAlarmStatus();
						}
					} 
				}
			}
			// we keep PRESSED and REPEATING events if the screen is not SCREEN_TIME
			if ((screen != SCREEN_TIME) && (eventType != EVENT_SWITCH_RELEASED)) {
				if (buttonId == TOUSCRUINO_UP) {
					field[activeField] ++;
					if (field[activeField] > fieldMax[activeField]) field[activeField] = fieldMin[activeField];
					if (screen == SCREEN_SET_DATE) {
						int mld = touscruino.getRTC().getMonthLastDay(field[0], field[1]);
						if (field[2] > mld) field[2] = mld;
					}
				} else if (buttonId == TOUSCRUINO_DOWN) {
					field[activeField] --;
					if (field[activeField] < fieldMin[activeField]) field[activeField] = fieldMax[activeField];
				}
				if ((buttonId == TOUSCRUINO_UP) || (buttonId == TOUSCRUINO_DOWN)) {
					if (screen == SCREEN_SET_DATE) drawDateAdjust();
					if (screen >= SCREEN_SET_TIME) drawTimeAdjust();
					if (screen >= SCREEN_SET_ALARM_D1) drawAlarmAdjust();
				}
			}
			return true;
		}
		public boolean handle(int type, int value, int x, int y) { return false; }
		public boolean handle(int type) { return false; }
	}
	
	
	class timeHandlerCB implements IEventCallback {
		public boolean handle(int eventType) {
		    // show the alarm status
		    showAlarmStatus();

		    if (screen != SCREEN_TIME) {
		        watchdog ++;
		        if (watchdog > RETURN_TO_TIME_TRIGGER) {
		        	// return to SCREEN_TIME
		            screen = SCREEN_SET_ALARM_D7 + 1;
		            changeScreen();
		        }
		    	return true;
		    }
		    
		    ArduRCT_RealTimeClock rtc = touscruino.getRTC();
		    // blink the seconds
		    if ((rtc.getSecond() % 2) == 0) drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS);
		    else drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS);
		    
		    // draw a progress bar for the seconds
		    if (rtc.getSecond() == 0) touscruino.fillRectangle((touscruino.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, SECONDS_WIDTH, SECONDS_HEIGHT, BLACK);
		    else touscruino.fillRectangle((touscruino.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, rtc.getSecond() * SECONDS_WIDTH / 60, SECONDS_HEIGHT, YELLOW);
		    
		    if (eventType == EVENT_TIME_MINUTE) {
				// draw hour and minutes, digit by digit
			    for (int i=0; i<4; i++) {
			        if (timeDigit[i] != rtc.getTimeDigit(i)) {
			            // erase the digit
			        	touscruino.drawBigDigit(timeDigit[i], timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS, DIGIT_STYLE);
			            // draw the new one
			            if ((i != 0) || (timeDigit[i] != 0)) touscruino.drawBigDigit(rtc.getTimeDigit(i), timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS, DIGIT_STYLE);
			            timeDigit[i] = rtc.getTimeDigit(i);
			        }
			    }
		    }

		    // if date as changed, redraw the date and alarm 
		    if (eventType == EVENT_TIME_DAY) {
		    	touscruino.drawString(rtc.getDateAsString(ArduRCT_RealTimeClock.RTC_WITH_DAY_OF_WEEK), DATE_X, DATE_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        if (rtc.isAlarmOn(touscruino.getRTC().getDayOfWeek())) {
		        	touscruino.drawString("Alarm", ALARM_STRING_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        	touscruino.drawString(touscruino.getRTC().getAlarmTimeAsString(rtc.getDayOfWeek()), ALARM_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        } else {
		        	touscruino.fillRectangle(ALARM_STRING_X, ALARM_Y, 12*touscruino.getFontCharWidth(FONT_MEDIUM), touscruino.getFontHeight(FONT_MEDIUM), BLACK);
		        }
		    }

			return true;
		}

		public boolean handle(int type, int value, int x, int y) { return false; }
		public boolean handle(int type, int value) { return false; }
	}

	void changeScreen() {
	    ArduRCT_RealTimeClock rtc = touscruino.getRTC();
		screen ++;
		if (screen > SCREEN_SET_ALARM_D7) screen = SCREEN_TIME;
		fieldMax[0] = 23;
		fieldMax[1] = 59;
		for (int i=0; i<3; i++) fieldMin[i] = 0;
		if (screen == SCREEN_SET_TIME) {
			field[0] = rtc.getHour();
			field[1] = rtc.getMinute();
			nbFields = 2;
		} else if (screen >= SCREEN_SET_ALARM_D1) {
			rtcAlarm_t alarm = rtc.getAlarmTime(screen - SCREEN_SET_ALARM_D1);
			field[0] = (alarm.hour & ~RTC_ALARM_ON);
			timeDigit[0] = field[0] / 10;
			timeDigit[1] = field[0] % 10;
			field[1] = alarm.minute;
			timeDigit[2] = field[1] / 10;
			timeDigit[3] = field[1] % 10;
			field[2] = (alarm.hour & ~RTC_ALARM_ON) != 0 ? 1 : 0;
			fieldMax[2] = 1;
			nbFields = 3;
		} else if (screen == SCREEN_SET_DATE) {
			field[0] = rtc.getYear();
			fieldMax[0] = 2300;
			field[1] = rtc.getMonth();
			fieldMax[1] = 12;
			fieldMin[1] = 1;
			field[2] = rtc.getDay();
			fieldMax[2] = 31;
			fieldMin[2] = 1;
			field[3] = rtc.getDayOfWeek();
			nbFields = 3;
		} else {
			for (int i=0; i<4; i++) timeDigit[i] = 0xFF;			
		}
		touscruino.fillScreen(BLACK);
		activeField = 0;
		drawScreen();	
	}

	void drawScreen() {
		if (screen == SCREEN_TIME) {
			timeEventHandler.handle(EVENT_TIME_MINUTE);
			timeEventHandler.handle(EVENT_TIME_DAY);		
		} else if (screen == SCREEN_SET_TIME) {
			drawCenteredString("Set time",  5, FONT_MEDIUM);
			drawTimeAdjust();
		} else if (screen == SCREEN_SET_DATE) {
			drawCenteredString("Set date",  5, FONT_MEDIUM);
			drawDateAdjust();
		} else {	// set alarms
			int x = drawCenteredString("Set DOW alarm",  5, FONT_MEDIUM);
			x += touscruino.getFontCharWidth(FONT_MEDIUM)*4;
			touscruino.drawString(touscruino.getRTC().getDayOfWeekAsString(screen-SCREEN_SET_ALARM_D1), x, 5, ORANGE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
			drawTimeAdjust();
			drawAlarmAdjust();
		}
	}
	
	void drawTimeAdjust() {
		timeDigit[0] = field[0] / 10;
		timeDigit[1] = field[0] % 10;
		timeDigit[2] = field[1] / 10;
		timeDigit[3] = field[1] % 10;
		touscruino.fillRectangle(20, 35, 32*4+10+4, 50, BLACK);
		touscruino.drawBigDigit(timeDigit[0], 15, 35, 28, 50, activeField == 0 ? RED : WHITE, 6, DIGIT_STYLE);
		touscruino.drawBigDigit(timeDigit[1], 15+32, 35, 28, 50, activeField == 0 ? RED : WHITE, 6, DIGIT_STYLE);
		drawColumn(15+32*2+3, 35, 50, WHITE, 6);
		touscruino.drawBigDigit(timeDigit[2], 15+32*2+10, 35, 28, 50, activeField == 1 ? RED : WHITE, 6, DIGIT_STYLE);
		touscruino.drawBigDigit(timeDigit[3], 15+32*3+10, 35, 28, 50, activeField == 1 ? RED : WHITE, 6, DIGIT_STYLE);
	}
	
	void drawAlarmAdjust() {
		int x = 80;
		int y = 108;
		touscruino.fillRectangle(x-12, y-16, 32, 32, BLACK);
		touscruino.drawRectangle(x-8, y-8, 8, 16, activeField == 2 ? RED : WHITE, 2);
		touscruino.drawLine(x, y-8, x+8, y-16, activeField == 2 ? RED : WHITE, 2);
		touscruino.drawLine(x, y+8, x+8, y+16, activeField == 2 ? RED : WHITE, 2);
		touscruino.drawLine(x+8, y-16, x+8, y+16, activeField == 2 ? RED : WHITE, 2);
		if (field[2] == 0) {
			touscruino.drawLine(x-10, y-13, x+16, y+13, RED, 3);
			touscruino.drawLine(x-10, y+13, x+16, y-13, RED, 3);
		} else {
			drawNote(x+15, y+3, WHITE);
			drawNote(x+25, y, WHITE);
		}
	}

	void drawNote(int x, int y, int color) {
		touscruino.fillRectangle(x, y, 4, 2, color);
		touscruino.fillRectangle(x+1, y-1, 2, 4, color);
		touscruino.drawLine(x+3, y-11, x+3, y, color, 1);
		touscruino.drawLine(x+3, y-11, x+6, y-11, color, 1);
		
	}
	
	void drawDateAdjust() {
		int buffer[] = touscruino.getRTC().getDateAsString(field[0], field[1], field[2], RTC_WITH_DAY_OF_WEEK);
		int buffer2[] = new int[5];
		int fcw = touscruino.getFontCharWidth(FONT_HUGE);
		for (int i=0; i<4; i++) buffer2[i] = buffer[i+11];
		buffer2[4] = 0;
		touscruino.drawString(buffer2, 10, 45, activeField == 0 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
		for (int i=0; i<3; i++) buffer2[i] = buffer[i+7];
		buffer2[3] = 0;
		touscruino.drawString(buffer2, 100, 45, activeField == 1 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
		for (int i=0; i<3; i++) buffer2[i] = buffer[i];
		buffer2[3] = 0;
		touscruino.drawString(buffer2, 10 + fcw, 80, WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
		for (int i=0; i<3; i++) buffer2[i] = buffer[i+4];
		buffer2[2] = 0;
		touscruino.drawString(buffer2, 100 + fcw, 80, activeField == 2 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
	}
	
	void drawColumn(int x, int y, int height, int color, int  thickness) {
	    int ht = thickness/2;
	    touscruino.fillRectangle(x - ht , y + height/3 - ht, thickness, thickness, color); 
	    touscruino.fillRectangle(x - ht , y + 2*height/3 - ht, thickness, thickness, color); 
	}

	int drawCenteredString(String s, int y, int fontSize) {
		int x = (touscruino.getWidth() - touscruino.getStringWidth(s, fontSize))/2;
		touscruino.drawString(s, x, y, WHITE, fontSize, FONT_BOLD, NO_OVERLAY);
		y += 5+touscruino.getFontHeight(fontSize);
		touscruino.drawLine(0, y, touscruino.getWidth()-1, y, WHITE, 2);
		return x;
	}
	
	
	void showAlarmStatus() {
	    // show the alarm
	    if (touscruino.getRTC().isAlarmRinging()) drawAlarmSignal((touscruino.getRTC().getSecond() % 2) == 0 ? BLACK : RED);
	    else if (touscruino.getRTC().isAlarmSnoozing()) drawAlarmSignal((touscruino.getRTC().getSecond() % 2) == 0 ? BLACK : ORANGE);
	    else drawAlarmSignal(BLACK);
	}
	
	void drawAlarmSignal(int color) {
		touscruino.fillRectangle(2, ALARM_Y, 128-ALARM_Y-2, 128-ALARM_Y-2, color);
		touscruino.fillRectangle(160-128+ALARM_Y, ALARM_Y, 128-ALARM_Y-2, 128-ALARM_Y-2, color);
	}
}
