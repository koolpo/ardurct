package com.google.code.ardurct.libraries.graphics.examples;

import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.graphics.GraphicsFirmware;

public class BasicExample extends GraphicsFirmware {
	
	ArduRCT_JAVA graphics = new ArduRCT_JAVA();
	
	public void setup() {
		graphics.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		graphics.setMargin(2);
		graphics.setCursor(0, 0);
		graphics.print("Select an item from the Examples menu");
	}

	public void loop() {
	}

}
