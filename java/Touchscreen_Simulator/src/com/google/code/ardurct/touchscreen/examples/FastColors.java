package com.google.code.ardurct.touchscreen.examples;


public class FastColors extends Example {

	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    touchscreen.setBacklight(180);
	    
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
	    		touchscreen.fillRectangle(x+1, y+1, 14, 8, color);
	    		touchscreen.drawRectangle(x+1, y+1, 14, 8, BLACK, 1);
	    		touchscreen.drawString(s, x+17, y+1, color, FONT_SMALL, false, false);
	    	}
	    	
	    }
	}

	public void loop() {
	    delay(100);
	}
}
