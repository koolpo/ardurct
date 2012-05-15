package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;


public class Graphics extends ArduinoFirmware {

	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    touchscreen.setBacklight(180);

	    // Fonts
	    touchscreen.drawString("SMALL", 5, 5, BLACK, FONT_SMALL, false, false);
	    touchscreen.drawString("BOLD", 100, 5, BLACK, FONT_SMALL, FONT_BOLD, false);
	    touchscreen.drawString("MEDIUM", 5, 20, BLACK, FONT_MEDIUM, false, false);
	    touchscreen.drawString("BOLD", 100, 20, BLACK, FONT_MEDIUM, FONT_BOLD, false);
	    touchscreen.drawString("BIG", 5, 40, BLACK, FONT_BIG, false, false);
	    touchscreen.drawString("BOLD", 100, 40, BLACK, FONT_BIG, FONT_BOLD, false);
	    touchscreen.drawString("HUGE", 5, 65, BLACK, FONT_HUGE, false, false);
	    touchscreen.drawString("BOLD", 100, 65, BLACK, FONT_HUGE, FONT_BOLD, false);
	    // Lines
	    touchscreen.drawLine(10, 95, 60, 95, RED, 2);
	    touchscreen.drawLine(80, 95, 80, 105, RED, 3);
	    touchscreen.drawLine(100, 95, 130, 105, RED, 1);
	    // Triangles
	    touchscreen.drawTriangle(20, 140, 40, 120, 50, 135, BLUE, 2);
	    touchscreen.fillTriangle(120, 130, 140, 120, 150, 135, BLUE);
	    // Rectangle
	    touchscreen.drawRectangle(30, 150, 40, 20, MAGENTA, 3);
	    touchscreen.fillRectangle(130, 150, 40, 20, MAGENTA);
	    // Rounded rectangle
	    touchscreen.drawRoundedRectangle(30, 180, 40, 20, 6, GREEN, 2);
	    touchscreen.fillRoundedRectangle(130, 180, 40, 20, 6, GREEN);
	    // Arcs
	    touchscreen.drawArc(40, 210, 30, SCREEN_ARC_SE, RED, 4);
	    touchscreen.drawArc(80, 240, 30, SCREEN_ARC_NEE, RED, 3);
	    touchscreen.fillArc(140, 210, 20, SCREEN_ARC_SW, RED);
	    // Circles
	    touchscreen.drawCircle(50, 270, 20, BLUE, 1);
	    touchscreen.drawCircle(100, 270, 20, BLUE, 3);
	    touchscreen.fillCircle(150, 270, 20, BLUE);
	    // Rotations
	    /*
	    touchscreen.setRotation(SCREEN_ROTATION_90);
	    touchscreen.drawString("Rotation 90deg", 5, 5, BLACK, FONT_MEDIUM, FONT_PLAIN, false);
	    touchscreen.setRotation(SCREEN_ROTATION_180);
	    touchscreen.drawString("Rotation 180deg", 5, 5, BLACK, FONT_MEDIUM, FONT_PLAIN, false);
	    touchscreen.setRotation(SCREEN_ROTATION_270);
	    touchscreen.drawString("Rotation 270deg", 5, 5, BLACK, FONT_MEDIUM, FONT_PLAIN, false);
	    */
	}

	public void loop() {
	    delay(100);
	}
}
