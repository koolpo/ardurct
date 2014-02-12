package com.google.code.ardurct.hardware;

import java.util.Hashtable;

import com.google.code.ardurct.libraries.IArduinoDefines;

public class Digital implements IArduinoDefines {
	
	public static Hashtable<Integer, Integer> pins = new Hashtable<Integer, Integer>();
	
	public static void write(int pin, int value) {
		pins.put(pin, value == LOW ? LOW : HIGH);
	}
	
	public static int read(int pin) {
		if (!pins.containsKey(pin)) pins.put(pin, HIGH);
		return pins.get(pin);
	}
}
