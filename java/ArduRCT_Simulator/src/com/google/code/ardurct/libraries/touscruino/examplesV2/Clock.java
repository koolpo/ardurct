package com.google.code.ardurct.libraries.touscruino.examplesV2;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_RealTimeClock;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_RealTimeClock.rtcAlarm_t;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruino;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class Clock extends TouscruinoFirmware {

	// ------------------ Java specific ------------------------- //
	private touchPanelCB handleTouchPanel = new touchPanelCB();
	private timeHandlerCB drawDateAndTime = new timeHandlerCB();
	// ------------------ Java specific ------------------------- //

/**
beg white black
rot 90
!drawDate setup
s 20 10 "Tue 20 Apr 2014" white h b
!drawTime setup
bd 5 55 64 120 14 8 white
bd 80 55 64 120 14 8 white
bd 175 55 64 120 14 8 white
bd 250 55 64 120 14 8 white
fr 154 90 12 12 white
fr 154 125 12 12 white
!drawAlarm setup
s 95 202 "20:20" white h b
r 204 205 10 16 white 2
l 213 205 220 198 white 2
l 213 220 220 227 white 2
l 225 197 225 227 white 2
l 221 197 224 197 white 2
l 221 227 224 227 white 2
!drawNote
fr 232 214 4 2 white
fr 233 213 2 4 white
vl 235 214 204 white
hl 235 205 238 white
**/
	
	static final int RETURN_TO_TIME_TRIGGER = 30;
	
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
	
	ArduRCT_TouScruino Touscruino = new ArduRCT_TouScruino();
	
	ArduRCT_EventHandler timeEventHandler = new ArduRCT_EventHandler(EVENT_TIME, drawDateAndTime);
	ArduRCT_EventHandler touchPanelEventHandler = new ArduRCT_EventHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, EVENT_ANY_VALUE, EVENT_ANY_VALUE, handleTouchPanel);

	public void setup() {		
		Touscruino.begin(WHITE, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		Touscruino.fillScreen(BLACK);
		Touscruino.setRotation(GRAPHICS_ROTATION_90);
		
		Touscruino.registerEventHandler(timeEventHandler);

		// set the RTC
		Touscruino.getRTC().setDateTime(2012, DECEMBER, 31, 23, 45, 0);
		Touscruino.getRTC().setAlarmTime(THURSDAY, 23, 50, RTC_ALARM_ON);
		Touscruino.getRTC().setAlarmTime(FRIDAY, 1, 05, RTC_ALARM_ON);

		// draw the time
		screen = SCREEN_TIME;
		drawScreen();
	}

	/** ---- ArduRCT-Designer ---- **
	 * !test
	 * rot 90
	 * fs black
	 * s "Tue 24 Apr 2014" 20 10 white h t t
	 * bd 8 5 50 62 120 WHITE 14 a
	 * bd 8 75 50 62 120 WHITE 14 a
	 * bd 8 180 50 62 120 WHITE 14 a
	 * bd 8 250 50 62 120 WHITE 14 a
	 * fr 152 85 12 12 white
	 * fr 152 125 12 12 white
	 * s "6:30" 140 195 white h t t
	 ** ---- ArduRCT-Designer ---- **/

	public void test() {
		Touscruino.setRotation(GRAPHICS_ROTATION_90);
		Touscruino.fillScreen(BLACK);
		Touscruino.drawString("Tue 24 Apr 2014", 20, 10, WHITE, FONT_HUGE, FONT_BOLD, OVERLAY);
		Touscruino.drawBigDigit(8, 5, 50, 62, 120, WHITE, 14, GRAPHICS_STYLE_ADVANCED);
		Touscruino.drawBigDigit(8, 75, 50, 62, 120, WHITE, 14, GRAPHICS_STYLE_ADVANCED);
		Touscruino.drawBigDigit(8, 180, 50, 62, 120, WHITE, 14, GRAPHICS_STYLE_ADVANCED);
		Touscruino.drawBigDigit(8, 250, 50, 62, 120, WHITE, 14, GRAPHICS_STYLE_ADVANCED);
		Touscruino.fillRectangle(152, 85, 12, 12, WHITE);
		Touscruino.fillRectangle(152, 125, 12, 12, WHITE);
		Touscruino.drawString("6:30", 140, 195, WHITE, FONT_HUGE, FONT_BOLD, OVERLAY);
	}

	public void loop() {
		delay(5);
	    Touscruino.manageEvents();
	}
		
	class timeHandlerCB implements IEventCallback {
		public int handle(int eventType) {
		    // show the alarm status, whatever the screen
		    showAlarmStatus();

		    if (screen != SCREEN_TIME) {
		        watchdog ++;
		        if (watchdog > RETURN_TO_TIME_TRIGGER) changeScreen(SCREEN_TIME);
		    	return EVENT_HANDLING_DONE;
		    }
		    
		    ArduRCT_RealTimeClock rtc = Touscruino.getRTC();
		    // blink the seconds
		    if ((rtc.getSecond() % 2) == 0) drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS);
		    else drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS);
		    		    
		    if (eventType == EVENT_TIME_MINUTE) {
				// draw hour and minutes, digit by digit
			    for (int i=0; i<4; i++) {
			        if (timeDigit[i] != rtc.getTimeDigit(i)) {
			            // erase the digit
			        	Touscruino.drawBigDigit(timeDigit[i], timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, DIGIT_THICKNESS, DIGIT_STYLE);
			            // draw the new one
			            if ((i != 0) || (timeDigit[i] != 0)) Touscruino.drawBigDigit(rtc.getTimeDigit(i), timeDigit_x[i], 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, DIGIT_THICKNESS, DIGIT_STYLE);
			            timeDigit[i] = rtc.getTimeDigit(i);
			        }
			    }
		    }

		    // if date as changed, redraw the date and alarm 
		    if (eventType == EVENT_TIME_DAY) {
		    	Touscruino.drawString(rtc.getDateAsString(ArduRCT_RealTimeClock.RTC_WITH_DAY_OF_WEEK), DATE_X, DATE_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        if (rtc.isAlarmOn(Touscruino.getRTC().getDayOfWeek())) {
		        	Touscruino.drawString("Alarm", ALARM_STRING_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        	Touscruino.drawString(Touscruino.getRTC().getAlarmTimeAsString(rtc.getDayOfWeek()), ALARM_X, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		        } else {
		        	Touscruino.fillRectangle(ALARM_STRING_X, ALARM_Y, 12*Touscruino.getFontCharWidth(FONT_MEDIUM), Touscruino.getFontHeight(FONT_MEDIUM), BLACK);
		        }
		    }

			return EVENT_HANDLING_DONE;
		}

		public int handle(int type, int value, int x, int y) { return EVENT_HANDLING_VOID; }
		public int handle(int type, int value) { return EVENT_HANDLING_VOID; }
	}

	class touchPanelCB implements IEventCallback {
		public int handle(int type, int value, int x, int y) {
			if (type == EVENT_TOUCHPANEL_PRESSED) Touscruino.fillRectangle(x-1, y-1, 3, 3, BLUE);
			else if (type == EVENT_TOUCHPANEL_DRAGGED) Touscruino.fillRectangle(x-1, y-1, 3, 3, RED);
			return EVENT_HANDLING_DONE;
		}
		public int handle(int type, int value) { return EVENT_HANDLING_VOID; }
		public int handle(int type) { return EVENT_HANDLING_VOID; }
	}
	
	void changeScreen(int newScreen) {
	    ArduRCT_RealTimeClock rtc = Touscruino.getRTC();
	    if (screen != SCREEN_TIME) {
			if (screen == SCREEN_SET_DATE) rtc.setDate(field[0], field[1], field[2]);
			else if (screen == SCREEN_SET_TIME) rtc.setTime(field[0], field[1], 0);
			else if (screen >= SCREEN_SET_ALARM_D1) rtc.setAlarmTime(screen - SCREEN_SET_ALARM_D1, field[0], field[1], field[2]);
	    }
		screen = newScreen;
		if (screen > SCREEN_SET_ALARM_D7) screen = SCREEN_TIME;
		fieldMax[0] = 23;
		fieldMax[1] = 59;
		activeField = 0;
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
			field[2] = (alarm.hour & RTC_ALARM_ON) != 0 ? 1 : 0;
			fieldMax[2] = 1;
			nbFields = 3;
			activeField = 2;
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
		Touscruino.fillScreen(BLACK);
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
			x += Touscruino.getFontCharWidth(FONT_MEDIUM)*4;
			Touscruino.drawString(Touscruino.getRTC().getDayOfWeekAsString(screen-SCREEN_SET_ALARM_D1), x, 5, ORANGE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
			drawTimeAdjust();
			drawAlarmAdjust();
		}
	}
	
	void drawTimeAdjust() {
		timeDigit[0] = field[0] / 10;
		timeDigit[1] = field[0] % 10;
		timeDigit[2] = field[1] / 10;
		timeDigit[3] = field[1] % 10;
		Touscruino.fillRectangle(20, 35, 32*4+10+4, 50, BLACK);
		Touscruino.drawBigDigit(timeDigit[0], 15, 35, 28, 50, activeField == 0 ? RED : WHITE, 6, DIGIT_STYLE);
		Touscruino.drawBigDigit(timeDigit[1], 15+32, 35, 28, 50, activeField == 0 ? RED : WHITE, 6, DIGIT_STYLE);
		drawColumn(15+32*2+3, 35, 50, WHITE, 6);
		Touscruino.drawBigDigit(timeDigit[2], 15+32*2+10, 35, 28, 50, activeField == 1 ? RED : WHITE, 6, DIGIT_STYLE);
		Touscruino.drawBigDigit(timeDigit[3], 15+32*3+10, 35, 28, 50, activeField == 1 ? RED : WHITE, 6, DIGIT_STYLE);
	}
	
	void drawAlarmAdjust() {
		int x = 80;
		int y = 108;
		Touscruino.fillRectangle(x-12, y-16, 50, 32, BLACK);
		Touscruino.drawRectangle(x-8, y-8, 8, 16, activeField == 2 ? RED : WHITE, 2);
		Touscruino.drawLine(x, y-8, x+8, y-16, activeField == 2 ? RED : WHITE, 2);
		Touscruino.drawLine(x, y+8, x+8, y+16, activeField == 2 ? RED : WHITE, 2);
		Touscruino.drawLine(x+8, y-16, x+8, y+16, activeField == 2 ? RED : WHITE, 2);
		if (field[2] == 0) {
			Touscruino.drawLine(x-10, y-13, x+16, y+13, RED, 3);
			Touscruino.drawLine(x-10, y+13, x+16, y-13, RED, 3);
		} else {
			drawNote(x+15, y+3,  activeField == 2 ? RED : WHITE);
			drawNote(x+25, y,  activeField == 2 ? RED : WHITE);
		}
	}

	void drawNote(int x, int y, int color) {
		Touscruino.fillRectangle(x, y, 4, 2, color);
		Touscruino.fillRectangle(x+1, y-1, 2, 4, color);
		Touscruino.drawLine(x+3, y-11, x+3, y, color, 1);
		Touscruino.drawLine(x+3, y-11, x+6, y-11, color, 1);
		
	}
	
	void drawDateAdjust() {
		int buffer[] = Touscruino.getRTC().getDateAsString(field[0], field[1], field[2], RTC_WITH_DAY_OF_WEEK);
		int buffer2[] = new int[5];
		int fcw = Touscruino.getFontCharWidth(FONT_HUGE);
		for (int i=0; i<4; i++) buffer2[i] = buffer[i+11];
		buffer2[4] = 0;
		Touscruino.drawString(buffer2, 10, 45, activeField == 0 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
		for (int i=0; i<3; i++) buffer2[i] = buffer[i+7];
		buffer2[3] = 0;
		Touscruino.drawString(buffer2, 100, 45, activeField == 1 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
		for (int i=0; i<3; i++) buffer2[i] = buffer[i];
		buffer2[3] = 0;
		Touscruino.drawString(buffer2, 10 + fcw, 80, WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
		for (int i=0; i<3; i++) buffer2[i] = buffer[i+4];
		buffer2[2] = 0;
		Touscruino.drawString(buffer2, 100 + fcw, 80, activeField == 2 ? RED : WHITE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
	}
	
	void drawColumn(int x, int y, int height, int color, int  thickness) {
	    int ht = thickness/2;
	    Touscruino.fillRectangle(x - ht , y + height/3 - ht, thickness, thickness, color); 
	    Touscruino.fillRectangle(x - ht , y + 2*height/3 - ht, thickness, thickness, color); 
	}

	int drawCenteredString(String s, int y, int fontSize) {
		int x = (Touscruino.getWidth() - Touscruino.getStringWidth(s, fontSize))/2;
		Touscruino.drawString(s, x, y, WHITE, fontSize, FONT_BOLD, NO_OVERLAY);
		y += 5+Touscruino.getFontHeight(fontSize);
		Touscruino.drawLine(0, y, Touscruino.getWidth()-1, y, WHITE, 2);
		return x;
	}
	
	
	void showAlarmStatus() {
	    // show the alarm
	    if (Touscruino.getRTC().isAlarmRinging()) drawAlarmSignal((Touscruino.getRTC().getSecond() % 2) == 0 ? BLACK : RED);
	    else if (Touscruino.getRTC().isAlarmSnoozing()) drawAlarmSignal((Touscruino.getRTC().getSecond() % 2) == 0 ? BLACK : ORANGE);
	    else drawAlarmSignal(BLACK);
	}
	
	void drawAlarmSignal(int color) {
		Touscruino.fillRectangle(2, ALARM_Y, 128-ALARM_Y-2, 128-ALARM_Y-2, color);
		Touscruino.fillRectangle(160-128+ALARM_Y, ALARM_Y, 128-ALARM_Y-2, 128-ALARM_Y-2, color);
	}
}
