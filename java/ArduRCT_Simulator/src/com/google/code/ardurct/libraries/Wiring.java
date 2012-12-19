package com.google.code.ardurct.libraries;

import java.util.Date;

public class Wiring implements IArduinoDefines {

	private long startTime = (new Date()).getTime();
	
	public long millis() {
		return (new Date()).getTime() - startTime;
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

	public int digitalRead(int pin) {
		return 1;
	}
}
