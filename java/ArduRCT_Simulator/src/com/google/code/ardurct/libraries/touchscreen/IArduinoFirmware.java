package com.google.code.ardurct.libraries.touchscreen;

import com.google.code.ardurct.libraries.HardwareSerial;

public interface IArduinoFirmware {
	
	public void setup();
	
	public void loop();
	
	public void setTouchscreen(TouchScreen touchscreen);
	
	public void setHardwareSerial(HardwareSerial serial);

	public void setHardwareSerial1(HardwareSerial serial);

	public void start();
	
	public void stop();
}
