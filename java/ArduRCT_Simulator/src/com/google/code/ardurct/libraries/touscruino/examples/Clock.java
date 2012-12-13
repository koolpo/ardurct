package com.google.code.ardurct.libraries.touscruino.examples;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_RealTimeClock;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruinoV1;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class Clock extends TouscruinoFirmware {

	// ------------------ Java specific ------------------------- //
	private timeHandlerCB drawDateAndTime = new timeHandlerCB();
	private buttonsCB handleButtons = new buttonsCB();
	// ------------------ Java specific ------------------------- //

	int MODE_TIME = 0;
	int MODE_ADJUST_TIME = 1;
	int MODE_ADJUST_DATE = 2;
	int MODE_ADJUST_ALARM_D1 = 3;
	int MODE_ADJUST_ALARM_D7 = 10;
	
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
	
	int mode = MODE_TIME;
	
	ArduRCT_TouScruinoV1 touscruino = new ArduRCT_TouScruinoV1();
	
	ArduRCT_EventHandler timeEventHandler = new ArduRCT_EventHandler(EVENT_TIME, drawDateAndTime);
	ArduRCT_EventHandler buttonEventHandler = new ArduRCT_EventHandler(EVENT_BUTTON, handleButtons);

	public void setup() {
		// ---------------  only for java ----------------------- //
		ArduRCT_JAVA.setLandscapeOrientation();
		// ---------------  only for java ----------------------- //
		
		touscruino.begin(WHITE, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touscruino.fillScreen(BLACK);
		
		touscruino.registerEventHandler(timeEventHandler);
		touscruino.registerEventHandler(buttonEventHandler);

		// set the RTC
		touscruino.getRTC().setTime(2012, ArduRCT_RealTimeClock.DECEMBER, 31, ArduRCT_RealTimeClock.THURSDAY, 23, 45, 0);
		touscruino.getRTC().setAlarmTime(ArduRCT_RealTimeClock.THURSDAY, 23, 50, ArduRCT_RealTimeClock.RTC_ALARM_ON);
		touscruino.getRTC().setAlarmTime(ArduRCT_RealTimeClock.FRIDAY, 1, 05, ArduRCT_RealTimeClock.RTC_ALARM_ON);

	    //graphics.setRotation(GRAPHICS_ROTATION_90);

		// draw the time
		timeEventHandler.process(EVENT_TIME_MINUTE);
		timeEventHandler.process(EVENT_TIME_DAY);
		
	}

	public void loop() {
		delay(5);
	    touscruino.update();
	}

	class buttonsCB implements IEventCallback {
		public boolean handle(int type, int value, int x, int y) {
			if (type == EVENT_BUTTON_PRESSED) {
				if (touscruino.getRTC().isAlarmRinging() || touscruino.getRTC().isAlarmSnoozing()) {
					if (value == UP) touscruino.getRTC().snoozeAlarm(5);
					if (value == DOWN) touscruino.getRTC().snoozeAlarm(10);
					if (value == ENTER) touscruino.getRTC().stopAlarm();	
				    showAlarmStatus();
				}
				Serial.print("B" + value + " ");
			}
			return true;
		}
	}
	
	class timeHandlerCB implements IEventCallback {
		public boolean handle(int type, int value, int x, int y) {
		    // show the alarm status
		    showAlarmStatus();

		    if (mode != MODE_TIME) return true;
		    
		    // blink the seconds
		    if ((touscruino.getRTC().getSecond() % 2) == 0) drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS);
		    else drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS);
		    
		    // draw a progress bar for the seconds
		    if (touscruino.getRTC().getSecond() == 0) touscruino.fillRectangle((touscruino.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, SECONDS_WIDTH, SECONDS_HEIGHT, BLACK);
		    else touscruino.fillRectangle((touscruino.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, touscruino.getRTC().getSecond() * SECONDS_WIDTH / 60, SECONDS_HEIGHT, YELLOW);
		    
		    if (type == EVENT_TIME_MINUTE) {
				// draw hour and minutes, digit by digit
			    for (int i=0; i<4; i++) {
			        if (timeDigit[i] != touscruino.getRTC().getTimeDigit(i)) {
			            // erase the digit
			        	touscruino.drawBigDigit(timeDigit[i], timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS, DIGIT_STYLE);
			            // draw the new one
			            if ((i != 0) || (timeDigit[i] != 0)) touscruino.drawBigDigit(touscruino.getRTC().getTimeDigit(i), timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS, DIGIT_STYLE);
			            timeDigit[i] = touscruino.getRTC().getTimeDigit(i);
			        }
			    }
		    }

		    // if date as changed, redraw the date and alarm 
		    if (type == EVENT_TIME_DAY) {
		    	touscruino.drawString(touscruino.getRTC().getDateAsString(ArduRCT_RealTimeClock.RTC_WITH_DAY_OF_WEEK), DATE_X, DATE_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        if (touscruino.getRTC().isAlarmOn(touscruino.getRTC().getDayOfWeek())) {
		        	touscruino.drawString("Alarm", ALARM_STRING_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        	touscruino.drawString(touscruino.getRTC().getAlarmTimeAsString(touscruino.getRTC().getDayOfWeek()), ALARM_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        } else {
		        	touscruino.fillRectangle(ALARM_STRING_X, ALARM_Y, 12*touscruino.getFontCharWidth(FONT_MEDIUM), touscruino.getFontHeight(FONT_MEDIUM), BLACK);
		        }
		    }

			return true;
		}

		void drawColumn(int x, int y, int height, int color, int  thickness) {
		    int ht = thickness/2;
		    touscruino.fillRectangle(x - ht , y + height/3 - ht, thickness, thickness, color); 
		    touscruino.fillRectangle(x - ht , y + 2*height/3 - ht, thickness, thickness, color); 
		}
		
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
