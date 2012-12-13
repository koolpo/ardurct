package com.google.code.ardurct.libraries.graphics.examples;

import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.graphics.GraphicsFirmware;

public class StringTests extends GraphicsFirmware {
	
	ArduRCT_JAVA graphics = new ArduRCT_JAVA();
	
	public void setup() {
		graphics.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		graphics.drawString("Test of a long string going for several lines\nwith a carriage return", 
				25, 50, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY, 25);
	}

	public void loop() {
	}
}
