package com.google.code.ardurct.hardware;

import java.util.Hashtable;

public class Analog {

	public static Hashtable<Integer, Integer> pins = new Hashtable<Integer, Integer>();
	
	public static void write(int pin, int value) {
		pins.put(pin, value);
	}
	
	public static int read(int pin) {
		if (!pins.containsKey(pin)) pins.put(pin, 0);
		return pins.get(pin);
	}
}
