package com.google.code.ardurct.lcd_pcd8544.examples;

import com.google.code.ardurct.lcd_pcd8544.LCDTest;

public class Font extends LCDTest {
	public void setup() {
		lcdT.begin();
		lcdT.print(" !\"#$%&'()*+,-./0123456789:;<=>?@");
		lcdT.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		lcdT.print("[\\]^_`");
		lcdG.begin();
		lcdG.print("abcdefghijklmnopqrstuvwxyz");
		lcdG.print("{|}");
		lcdG.write(0x7e);
		lcdG.write(0x7f);
	}
	
	public void loop() {
		
	}
}
