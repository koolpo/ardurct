package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;

public class StringTests extends ArduinoFirmware {
	
	public void setup() {
		touchscreen.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touchscreen.drawString("Test of a long string going for several lines\nwith a carriage return", 
				25, 50, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY, 25);
	}

	public void loop() {
	}
}
