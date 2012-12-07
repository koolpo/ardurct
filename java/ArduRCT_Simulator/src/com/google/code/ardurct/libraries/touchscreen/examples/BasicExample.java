package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;

public class BasicExample extends ArduinoFirmware {
	
	public void setup() {
		touchscreen.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touchscreen.setMargin(2);
		touchscreen.setCursor(0, 0);
		touchscreen.print("Select an example from the Examples menu");
	}

	public void loop() {
	}
}
