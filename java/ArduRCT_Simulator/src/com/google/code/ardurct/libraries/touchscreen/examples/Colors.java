package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;


public class Colors extends ArduinoFirmware {

	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    touchscreen.setBacklight(180);
	    touchscreen.setForegroundColor(WHITE);
	    drawScreen();
	}

	public void loop() {
	    delay(5000);
	}


	void drawScreen() {
	    long start = millis();
	    int height = touchscreen.getHeight()/3;
	    int width = touchscreen.getWidth();
	    int buffer[] = null;
	    for (int band=0; band<3; band++) buffer = drawBand(band, width, height);
	    for (int y=3*height; y<touchscreen.getHeight(); y++) touchscreen.pasteBitmap(buffer, 0, y, width, 1, false, WHITE);
	    touchscreen.setCursor(width/2-40, 3*height/2);
	    touchscreen.print(millis()-start);
	    touchscreen.print("ms");
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
	        touchscreen.pasteBitmap(buffer, 0, y+height*band, width, 1, false, WHITE);
	    }
	    return buffer;
	}

	
}
