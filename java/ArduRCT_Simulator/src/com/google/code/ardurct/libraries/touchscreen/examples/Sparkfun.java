package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;


public class Sparkfun extends ArduinoFirmware {
	
	// size 57x88
	public String sparkfun = "pc F800 pts 7 " +
			"tf 0 42 44 42 0 88 afe 27 35 30 afnw 17 42 17 rf 17 35 11 7 cf 30 13 13 annw 25 79 17 " +
			"pc FFFF cf 20 32 9 cf 43 11 8 pc F800 afnne 32 14 14 tf 49 18 49 14 51 17";
	
	int scale = 2;
	
	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.setBacklight(180);
	    // memorize the macro
	    touchscreen.executeMacro("w 0 " + sparkfun);
	    // execute it
		touchscreen.executeMacro("e 0", (touchscreen.getWidth()-57*scale)/2, (touchscreen.getHeight()-88*scale)/2, scale, 1, false);
	}

	public void loop() {
		delay(50);
	}

}
