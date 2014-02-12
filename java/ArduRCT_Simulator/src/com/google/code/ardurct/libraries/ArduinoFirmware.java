package com.google.code.ardurct.libraries;

public abstract class ArduinoFirmware extends Wiring 
implements Runnable {
	
	protected boolean setupComplete = false;
	public HardwareSerial Serial = null;
	public HardwareSerial Serial1 = null;
	private Thread t = null;
	private boolean running = false;
	
	public ArduinoFirmware() {
	}
	
	public void setHardwareSerial(HardwareSerial serial) {
		this.Serial = serial;
	}

	public void setHardwareSerial1(HardwareSerial serial) {
		this.Serial1 = serial;
	}
	
	public abstract void loop();

	public abstract void setup();

	@Override
	public void run() {
		while (running) {
			if (!setupComplete) setup();
			setupComplete = true;
			try { Thread.sleep(1); } catch (Exception e) { }
			loop();
		}
	}
	
	public int[] array(String s) {
		int[] a = new int[s.length()+1];
		for (int i=0; i<s.length(); i++) a[i] = s.charAt(i);
		a[s.length()] = 0;
		return a;
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
	
	// defined as a macro in colors.hpp, java does not have macros...
	public static int COLOR_565(int r, int g, int b) {
		return ((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3);
	}	

}
