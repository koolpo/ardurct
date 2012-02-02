package com.google.code.ardurct.lcd_pcd8544;

public interface ILCDTest {
	
	public void setup();
	
	public void loop();
	
	public void setDisplays(LCDGraphicPanel lcdG, LCDTextPanel lcdT);

	public void start();
	
	public void stop();
}
