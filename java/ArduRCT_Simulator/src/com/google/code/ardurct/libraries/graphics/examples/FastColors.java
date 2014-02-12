package com.google.code.ardurct.libraries.graphics.examples;

import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.graphics.GraphicsFirmware;

public class FastColors extends GraphicsFirmware {

	ArduRCT_JAVA graphics = new ArduRCT_JAVA();
	
	public void setup() {
	    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    graphics.setBacklight(180);
	    
		int[] s = new int[3];
		s[2] = 0;
	    for (int i=0; i<8; i++) {
	    	for (int j=0; j<32; j++) {
	    		int color = ((i*32+j) << 8) + (i*32+j);
	    		String hex = Integer.toHexString(i*32+j);
	    		s[0] = (hex.length() > 1 ? hex.charAt(0) : '0');
	    		s[1] = (hex.length() > 1 ? hex.charAt(1) : hex.charAt(0));
	    		int x = i * 30;
	    		int y = j * 10; 
	    		graphics.fillRectangle(x+1, y+1, 14, 8, color);
	    		graphics.drawRectangle(x+1, y+1, 14, 8, BLACK, 1);
	    		graphics.drawString(s, x+17, y+1, color, FONT_SMALL, false, false);
	    	}
	    	
	    }
	}

	public void loop() {
	    delay(100);
	}
}
