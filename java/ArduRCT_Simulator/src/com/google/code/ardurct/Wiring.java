package com.google.code.ardurct;

import java.util.Date;

public class Wiring {

	public static int OUTPUT = 0; 
	public static int INPUT = 1; 
	public static int HIGH = 1; 
	public static int LOW = 0; 
	
	public static int HEX = 16;
	public static int DEC = 10;


	public long millis() {
		return (new Date()).getTime();
	}

	public void delay(long ms) {
		try {
			Thread.sleep(ms);			
		} catch (Exception e) {
		}
	}

	public void pinMode(int pin, int value) {
	}
	
	public void digitalWrite(int pin, int value) {
		
	}
}
