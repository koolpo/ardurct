package com.google.code.ardurct.libraries.touchscreen;

import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.libraries.Wiring;

public abstract class ArduinoFirmware extends Wiring 
implements IArduinoFirmware, Runnable, IDefines {
	
	private Thread t = null;
	private boolean setupComplete = false;
	public TouchScreen touchscreen = null;
	public HardwareSerial Serial = null;
	public HardwareSerial Serial1 = null;
	private boolean running = false;
	
	public ArduinoFirmware() {
	}
	
	public void setHardwareSerial(HardwareSerial serial) {
		this.Serial = serial;
	}

	public void setHardwareSerial1(HardwareSerial serial) {
		this.Serial1 = serial;
	}

	public void setTouchscreen(TouchScreen touchscreen) {
		setupComplete = false;
		this.touchscreen = touchscreen;
	}
	
	public abstract void loop();

	public abstract void setup();

	// defined as a macro in colors.hpp, java does not have macros...
	public static int COLOR_565(int r, int g, int b) {
		return ((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3);
	}

	@Override
	public void run() {
		while (running) {
			if (!setupComplete) setup();
			setupComplete = true;
			loop();
		}
	}
	
	public static int[] stringToArray(String s) {
		int[] buffer = new int[s.length()+1];
		for (int i=0; i<s.length(); i++) buffer[i] = s.charAt(i);
		buffer[s.length()] = 0;
		return buffer;
	}
	
	public void start() {
		running = true;
		t = new Thread(this);
		t.start();
	}
	
	public void stop() {
		running = false;
		if (t != null) t.interrupt();
	}
}
