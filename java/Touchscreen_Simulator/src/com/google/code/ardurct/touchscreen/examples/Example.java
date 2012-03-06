package com.google.code.ardurct.touchscreen.examples;

import com.google.code.ardurct.HardwareSerial;
import com.google.code.ardurct.Wiring;
import com.google.code.ardurct.touchscreen.IDefines;
import com.google.code.ardurct.touchscreen.IExample;
import com.google.code.ardurct.touchscreen.Touchscreen;

public class Example extends Wiring 
implements IExample, Runnable, IDefines {
	
	private Thread t = null;
	private boolean setupComplete = false;
	public Touchscreen touchscreen = null;
	public HardwareSerial Serial = null;
	
	public Example() {
	}
	
	public void setHardwareSerial(HardwareSerial serial) {
		this.Serial = serial;
	}

	public void setTouchscreen(Touchscreen touchscreen) {
		setupComplete = false;
		this.touchscreen = touchscreen;
	}

	@Override
	public void loop() {
		// TODO Auto-generated method stub
	}

	@Override
	public void setup() {
		touchscreen.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touchscreen.setMargin(15);
		touchscreen.setCursor(0, 0);
		touchscreen.print("Select an example from the Examples menu");
	}

	// defined as a macro in colors.hpp, java does not have macros...
	public static int COLOR_565(int r, int g, int b) {
		return ((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3);
	}

	@Override
	public void run() {
		while (true) {
			if (!setupComplete) setup();
			setupComplete = true;
			loop();
		}
	}
	
	public int[] stringToArray(String s) {
		int[] buffer = new int[s.length()+1];
		for (int i=0; i<s.length(); i++) buffer[i] = s.charAt(i);
		buffer[s.length()] = 0;
		return buffer;
	}
	
	public void start() {
		t = new Thread(this);
		t.start();
	}
	
	public void stop() {
		if (t != null) t.interrupt();
	}
}
