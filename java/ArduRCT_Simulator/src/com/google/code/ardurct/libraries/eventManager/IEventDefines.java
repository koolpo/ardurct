package com.google.code.ardurct.libraries.eventManager;

public interface IEventDefines {
	
	public final static int EVENT_NO_VALUE = 0xFF;
	
	public final static int EVENT_TIME_TICK = 0x10;

	public final static int EVENT_TIME = 0x20;	
	public final static int EVENT_TIME_SECOND = 0x22;
	public final static int EVENT_TIME_MINUTE = 0x23;
	public final static int EVENT_TIME_HOUR = 0x24;
	public final static int EVENT_TIME_DAY = 0x25;
	public final static int EVENT_TIME_MONTH = 0x26;
	public final static int EVENT_TIME_YEAR = 0x27;	
	public final static int EVENT_TIME_ALARM = 0x2F;
	
	public final static int EVENT_BUTTON = 0x30;	
	public final static int EVENT_BUTTON_PRESSED = 0x31;
	public final static int EVENT_BUTTON_RELEASED = 0x32;
	public final static int EVENT_BUTTON_REPEATING = 0x33;

	public final static int RTC_HOUR10   = 0;
	public final static int RTC_HOUR1    = 1;
	public final static int RTC_MINUTE10 = 2;
	public final static int RTC_MINUTE1  = 3;
	public final static int RTC_SECOND10 = 4;
	public final static int RTC_SECOND1  = 5;

	public final static int MONDAY    =  0;
	public final static int TUESDAY   =  1;
	public final static int WEDNESDAY =  2;
	public final static int THURSDAY  =  3;
	public final static int FRIDAY    =  4;
	public final static int SATURDAY  =  5;
	public final static int SUNDAY    =  6;

	public final static int JANUARY    = 1;
	public final static int FEBRUARY   = 2;
	public final static int MARCH      = 3;
	public final static int APRIL      = 4;
	public final static int MAY        = 5;
	public final static int JUNE       = 6;
	public final static int JULY       = 7;
	public final static int AUGUST     = 8;
	public final static int SEPTEMBER  = 9;
	public final static int OCTOBER    = 10;
	public final static int NOVEMBER   = 11;
	public final static int DECEMBER   = 12;

	public final static boolean RTC_WITH_DAY_OF_WEEK = true;
	public final static boolean RTC_WITHOUT_DAY_OF_WEEK = false;
	public final static boolean RTC_WITH_SECONDS = true;
	public final static boolean RTC_WITHOUT_SECONDS = false;

	public final static int RTC_ALARM_ON = 0x80;
	public final static int RTC_ALARM_OFF = 0x00;

}
