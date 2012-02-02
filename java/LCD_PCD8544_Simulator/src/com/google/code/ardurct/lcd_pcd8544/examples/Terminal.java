package com.google.code.ardurct.lcd_pcd8544.examples;

import com.google.code.ardurct.lcd_pcd8544.LCDTest;

public class Terminal extends LCDTest {
		
	public void setup() {
		lcdT.begin();
		lcdT.print("This is a long text which will wrap around on several lines...");
	}
	
	public void loop() {
	}
}
