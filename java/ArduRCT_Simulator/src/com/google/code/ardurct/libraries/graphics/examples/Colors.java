package com.google.code.ardurct.libraries.graphics.examples;

import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.graphics.GraphicsFirmware;

public class Colors extends GraphicsFirmware {

	int rotation = 0;

	ArduRCT_JAVA graphics = new ArduRCT_JAVA();
	
	public void setup() {
	    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    graphics.setBacklight(180);
	    graphics.setForegroundColor(WHITE);
	}

	public void loop() {
		drawScreen();
	    delay(5000);
	    rotation ++;
	    if (rotation > GRAPHICS_ROTATION_270) rotation = GRAPHICS_ROTATION_0;
	    graphics.setRotation(rotation);
	    delay(100);
	}


	void drawScreen() {
	    long start = millis();
	    int height = graphics.getHeight()/3;
	    int width = graphics.getWidth();
	    int buffer[] = null;
	    for (int band=0; band<3; band++) buffer = drawBand(band, width, height);
	    for (int y=3*height; y<graphics.getHeight(); y++) graphics.pasteBitmap(buffer, 0, y, width, 1, false, WHITE);
	    graphics.setCursor(width/2-18, 3*height/2);
	    graphics.print(millis()-start);
	    graphics.print("ms");
	}

	int[] drawBand(int band, int width, int height) {
	    int buffer[] = new int[320];
	    int wd2 = width/2;

	    for (int y=0; y<height; y++) {
	        int Y = (y << 8)/height;
			// for first half of width, we go from black to color
			// along y, we go from full c1 to null c1 and null c2 to full c2
	        for (int x=0; x<=wd2; x++) {
	            int X = (x << 8)/wd2;
	            int c1 = (0xFF * X * (0xFF-Y)) >> 16;
	            int c2 = (0xFF * X * Y) >> 16;
	            int c3 = 0x00;
	            if (band == 0) buffer[x] = COLOR_565(c1, c2, c3);
	            else if (band == 1) buffer[x] = COLOR_565(c3, c1, c2);
	            else buffer[x] = COLOR_565(c2, c3, c1);
	        }
			// for second half of width, we go from color to white
			// along y, we go from full c1 to null c1 and null c2 to full c2
	        for (int x=wd2; x<width; x++) {
	            int X = ((x - wd2) << 7)/wd2;                        
	            int c1 = (0xFF * (0xFF-Y + X)) >> 8;
	            if (c1 > 0xFF) c1 = 0xFF;
	            int c2 = (0xFF * (Y + X)) >> 8;
	            if (c2 > 0xFF) c2 = 0xFF;
	            int c3 = (0xFF * X) >> 8;
	            if (band == 0) buffer[x] = COLOR_565(c1, c2, c3);
	            else if (band == 1) buffer[x] = COLOR_565(c3, c1, c2);
	            else buffer[x] = COLOR_565(c2, c3, c1);
	        }
	        graphics.pasteBitmap(buffer, 0, y+height*band, width, 1, false, WHITE);
	    }
	    return buffer;
	}

	
}
