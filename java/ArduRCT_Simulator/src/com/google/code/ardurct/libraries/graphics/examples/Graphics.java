package com.google.code.ardurct.libraries.graphics.examples;

import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.graphics.GraphicsFirmware;

public class Graphics extends GraphicsFirmware {

	ArduRCT_JAVA graphics = new ArduRCT_JAVA();
	
	int wait = 500;
	
	public void setup() {
	    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    graphics.setBacklight(180);
	}

	public void loop() {
	    // Fonts
	    graphics.drawString("SMALL", 5, 4, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawString("BOLD", 5, 14, BLACK, FONT_SMALL, FONT_BOLD, NO_OVERLAY);
	    graphics.drawString("MEDIUM", 5, 30, RED, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawString("BOLD", 5, 44, RED, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    graphics.drawString("BIG", 5, 64, BROWN, FONT_BIG, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawString("BOLD", 5, 84, BROWN, FONT_BIG, FONT_BOLD, NO_OVERLAY);
	    graphics.drawString("HUGE", 5, 110, BLUE, FONT_HUGE, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawString("BOLD", 5, 134, BLUE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
	    delay(wait);
	    // Lines
		graphics.fillScreen(WHITE);
		graphics.drawString("Lines", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    graphics.drawLine(10, 30, 104, 69, RED, 2);
	    graphics.drawLine(12, 92, 81, 81, BLUE, 1);
	    graphics.drawLine(15, 145, 120, 105, GREEN, 5);
	    delay(wait);
	    // Triangles
		graphics.fillScreen(WHITE);
		graphics.drawString("Triangles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    graphics.drawTriangle(7, 31, 71, 30, 20, 63, BLUE, 1);
	    graphics.drawString("Thickness 3", 50, 65, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawTriangle(17, 92, 87, 80, 109, 106, DARK_GREEN, 3);
	    graphics.drawString("Filled", 12, 145, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.fillTriangle(18, 112, 13, 136, 94, 128, ORANGE);
	    delay(wait);
	    // Corners
		graphics.fillScreen(WHITE);
		graphics.drawString("Corners", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		graphics.fillCorner(74, 90, 40, GRAPHICS_CORNER_SE, RED);
	    graphics.drawString("SE", 100, 120, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		graphics.fillCorner(54, 90, 35, GRAPHICS_CORNER_SW, ORANGE);
	    graphics.drawString("SW", 10, 115, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		graphics.fillCorner(74, 70, 45, GRAPHICS_CORNER_NE, BLUE);
	    graphics.drawString("NE", 110, 40, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		graphics.fillCorner(54, 70, 30, GRAPHICS_CORNER_NW, GREY);
	    graphics.drawString("NW", 20, 40, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    delay(wait);
	    // Rectangle
		graphics.fillScreen(WHITE);
		graphics.drawString("Rectangles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    graphics.drawRectangle(12, 28, 40, 50, GREY, 3);
	    graphics.fillRectangle(65, 30, 50, 25, ORANGE);
	    graphics.drawString("Filled", 70, 60, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawRoundedRectangle(10, 97, 55, 32, 6, RED, 2);
	    graphics.drawString("Rounded", 15, 136, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.fillRoundedRectangle(80, 80, 33, 48, 8, BLUE);
	    graphics.drawString("Rounded", 78, 136, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawString("filled", 78, 146, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    delay(wait);
	    // Arcs
		graphics.fillScreen(WHITE);
		graphics.drawString("Arcs", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    graphics.drawArc(55, 60, 20, GRAPHICS_ARC_NWW, RED, 3);
	    graphics.drawString("NWW", 15, 45, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawArc(62, 53, 20, GRAPHICS_ARC_NNW, ORANGE, 3);
	    graphics.drawString("NNW", 30, 27, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawArc(70, 55, 20, GRAPHICS_ARC_NE, BLUE, 3);
	    graphics.drawString("NE", 90, 33, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawArc(64, 65, 20, GRAPHICS_ARC_S, GREY, 3);
	    graphics.drawString("S", 90, 70, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.fillArc(55, 130, 20, GRAPHICS_ARC_NWW, GREEN);
	    graphics.drawString("NWW", 15, 115, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.fillArc(62, 123, 20, GRAPHICS_ARC_NNW, LIGHT_GREY);
	    graphics.drawString("NNW", 30, 97, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.fillArc(70, 125, 20, GRAPHICS_ARC_NE, ORANGE);
	    graphics.drawString("NE", 90, 103, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.fillArc(64, 115, 30, GRAPHICS_ARC_SSW+GRAPHICS_ARC_SSE, BLUE);
	    graphics.drawString("SSW+SSE", 80, 147, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    graphics.drawString("Filled", 10, 150, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    delay(wait);
	    // Circles
		graphics.fillScreen(WHITE);
		graphics.drawString("Circles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    graphics.drawCircle(64, 55, 25, BLUE, 3);
	    graphics.fillCircle(64, 115, 25, BROWN);
	    graphics.drawString("Filled", 50, 145, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    delay(wait);
	    // Big digits
		graphics.fillScreen(WHITE);
		graphics.drawString("Big digits", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		graphics.drawBigDigit(6, 10, 25, 40, 60, BLUE, 6, GRAPHICS_STYLE_NORMAL);
		graphics.drawBigDigit(4, 70, 35, 45, 55, RED, 10, GRAPHICS_STYLE_ADVANCED);
		graphics.drawBigDigit(8, 30, 100, 60, 50, BROWN, 12, GRAPHICS_STYLE_ADVANCED);
	    delay(wait);
	    // Rotations
		graphics.fillScreen(WHITE);
		graphics.drawString("Rotations", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
		graphics.setRotation(GRAPHICS_ROTATION_90);
		graphics.drawString("Rotation 90d", 15, 5, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		graphics.setRotation(GRAPHICS_ROTATION_180);
		graphics.drawString("Rotation 180d", 5, 5, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		graphics.setRotation(GRAPHICS_ROTATION_270);
		graphics.drawString("Rotation 270d", 15, 5, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		graphics.setRotation(GRAPHICS_ROTATION_0);
	    delay(wait);

	    graphics.fillScreen(WHITE);
		wait = 2000;
	}
}
