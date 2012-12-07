package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;


public class Graphics extends ArduinoFirmware {

	int wait = 3000;
	
	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    touchscreen.setBacklight(180);
	}

	public void loop() {
	    // Fonts
	    touchscreen.drawString("SMALL", 5, 4, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawString("BOLD", 5, 14, BLACK, FONT_SMALL, FONT_BOLD, NO_OVERLAY);
	    touchscreen.drawString("MEDIUM", 5, 30, RED, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawString("BOLD", 5, 44, RED, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    touchscreen.drawString("BIG", 5, 64, BROWN, FONT_BIG, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawString("BOLD", 5, 84, BROWN, FONT_BIG, FONT_BOLD, NO_OVERLAY);
	    touchscreen.drawString("HUGE", 5, 110, BLUE, FONT_HUGE, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawString("BOLD", 5, 134, BLUE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
	    delay(wait);
	    // Lines
		touchscreen.fillScreen(WHITE);
		touchscreen.drawString("Lines", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    touchscreen.drawLine(10, 30, 104, 69, RED, 2);
	    touchscreen.drawLine(12, 92, 81, 81, BLUE, 1);
	    touchscreen.drawLine(15, 145, 120, 105, GREEN, 5);
	    delay(wait);
	    // Triangles
		touchscreen.fillScreen(WHITE);
		touchscreen.drawString("Triangles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    touchscreen.drawTriangle(7, 31, 71, 30, 20, 63, BLUE, 1);
	    touchscreen.drawString("Thickness 3", 50, 65, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawTriangle(17, 92, 87, 80, 109, 106, DARK_GREEN, 3);
	    touchscreen.drawString("Filled", 12, 145, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.fillTriangle(18, 112, 13, 136, 94, 128, ORANGE);
	    delay(wait);
	    // Corners
		touchscreen.fillScreen(WHITE);
		touchscreen.drawString("Corners", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		touchscreen.fillCorner(74, 90, 40, GRAPHICS_CORNER_SE, RED);
	    touchscreen.drawString("SE", 100, 120, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touchscreen.fillCorner(54, 90, 35, GRAPHICS_CORNER_SW, ORANGE);
	    touchscreen.drawString("SW", 10, 115, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touchscreen.fillCorner(74, 70, 45, GRAPHICS_CORNER_NE, BLUE);
	    touchscreen.drawString("NE", 110, 40, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touchscreen.fillCorner(54, 70, 30, GRAPHICS_CORNER_NW, GREY);
	    touchscreen.drawString("NW", 20, 40, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    delay(wait);
	    // Rectangle
		touchscreen.fillScreen(WHITE);
		touchscreen.drawString("Rectangles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    touchscreen.drawRectangle(12, 28, 40, 50, GREY, 3);
	    touchscreen.fillRectangle(65, 30, 50, 25, ORANGE);
	    touchscreen.drawString("Filled", 70, 60, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawRoundedRectangle(10, 97, 55, 32, 6, RED, 2);
	    touchscreen.drawString("Rounded", 15, 136, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.fillRoundedRectangle(80, 80, 33, 48, 8, BLUE);
	    touchscreen.drawString("Rounded", 78, 136, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawString("filled", 78, 146, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    delay(wait);
	    // Arcs
		touchscreen.fillScreen(WHITE);
		touchscreen.drawString("Arcs", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    touchscreen.drawArc(55, 60, 20, GRAPHICS_ARC_NWW, RED, 3);
	    touchscreen.drawString("NWW", 15, 45, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawArc(62, 53, 20, GRAPHICS_ARC_NNW, ORANGE, 3);
	    touchscreen.drawString("NNW", 30, 27, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawArc(70, 55, 20, GRAPHICS_ARC_NE, BLUE, 3);
	    touchscreen.drawString("NE", 90, 33, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawArc(64, 65, 20, GRAPHICS_ARC_S, GREY, 3);
	    touchscreen.drawString("S", 90, 70, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.fillArc(55, 130, 20, GRAPHICS_ARC_NWW, GREEN);
	    touchscreen.drawString("NWW", 15, 115, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.fillArc(62, 123, 20, GRAPHICS_ARC_NNW, LIGHT_GREY);
	    touchscreen.drawString("NNW", 30, 97, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.fillArc(70, 125, 20, GRAPHICS_ARC_NE, ORANGE);
	    touchscreen.drawString("NE", 90, 103, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.fillArc(64, 115, 30, GRAPHICS_ARC_SSW+GRAPHICS_ARC_SSE, BLUE);
	    touchscreen.drawString("SSW+SSE", 80, 147, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.drawString("Filled", 10, 150, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    delay(wait);
	    // Circles
		touchscreen.fillScreen(WHITE);
		touchscreen.drawString("Circles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    touchscreen.drawCircle(64, 55, 25, BLUE, 3);
	    touchscreen.fillCircle(64, 115, 25, BROWN);
	    touchscreen.drawString("Filled", 50, 145, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    delay(wait);
	    // Big digits
		touchscreen.fillScreen(WHITE);
		touchscreen.drawString("Big digits", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		touchscreen.drawBigDigit(6, 10, 25, 40, 60, BLUE, 6, GRAPHICS_STYLE_NORMAL);
		touchscreen.drawBigDigit(4, 70, 35, 45, 55, RED, 10, GRAPHICS_STYLE_ADVANCED);
		touchscreen.drawBigDigit(8, 30, 100, 60, 50, BROWN, 12, GRAPHICS_STYLE_ADVANCED);
	    delay(wait);
	    // Rotations
		touchscreen.fillScreen(WHITE);
		touchscreen.drawString("Rotations", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    /*
	    touchscreen.setRotation(SCREEN_ROTATION_90);
	    touchscreen.drawString("Rotation 90d", 15, 5, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.setRotation(SCREEN_ROTATION_180);
	    touchscreen.drawString("Rotation 180d", 5, 5, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.setRotation(SCREEN_ROTATION_270);
	    touchscreen.drawString("Rotation 270d", 15, 5, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
	    */
	    delay(wait);
		touchscreen.fillScreen(WHITE);
		wait = 2000;
	}
}
