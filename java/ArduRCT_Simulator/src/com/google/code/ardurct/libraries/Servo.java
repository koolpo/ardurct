package com.google.code.ardurct.libraries;

import com.google.code.ardurct.hardware.ServoBank;

public class Servo {
	
	int index;
	ServoBank bank;
	int value;
	
	public Servo(String servoBankName) {
		index = -1;
		value = 90;
		bank = ServoBank.getInstance(servoBankName);
	}
		
	public void attach(int pin) {
		if (bank == null) return;
		index = bank.add(pin);
		bank.setValue(index, value);
	}
	
	public boolean isAttached() {
		return (index != -1);
	}
	
	public void write(int aValue) {
		value = aValue;
		if (value < 0) value = 0;
		if (value > 180) value = 180;
		if (bank == null) return;
		bank.setValue(index, value);
	}
	
	public int read() {
		return value;
	}

	public void writeMicroseconds(int aValue) {
		write(90 + (aValue-2000)*90/1000);
	}
	
	public int readMicroseconds() {
		return 2000 + (value-90)*1000/90;
	}
}
