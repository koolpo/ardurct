package com.google.code.ardurct.touchscreen;

import com.google.code.ardurct.HardwareSerial;

public interface IExample {
	
	public void setup();
	
	public void loop();
	
	public void setTouchscreen(TouchScreen touchscreen);
	
	public void setHardwareSerial(HardwareSerial serial);

	public void start();
	
	public void stop();
}
