package com.google.code.ardurct.lcd_pcd8544.examples;

import com.google.code.ardurct.lcd_pcd8544.LCDTest;

public class Clock extends LCDTest {
	
	// These will become #define
	int CLOCK_CENTER_X = LCD_PCD8544_WIDTH / 2;
	int CLOCK_CENTER_Y = LCD_PCD8544_HEIGHT / 2;
	int CLOCK_RADIUS = ((LCD_PCD8544_HEIGHT / 2) - 2);
	
	int CLOCK_SKEW = 4;
	
	float CLOCK_HAND_S = (CLOCK_RADIUS - 5);
	float CLOCK_HAND_M = (CLOCK_HAND_S * 0.9f);
	float CLOCK_HAND_H = (CLOCK_HAND_S * 0.7f);
	
	// These will become 
	int seconds = 0;
	int minutes = 0;
	int hours = 0;
	int days = 0;
	
	void drawHands(float angleH, float angleM, float angleS, boolean on) {
	    lcdG.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, 
	    		CLOCK_CENTER_X + (int)Math.round((CLOCK_HAND_H+CLOCK_SKEW) * Math.cos(angleH)), 
	    		CLOCK_CENTER_Y + (int)Math.round(CLOCK_HAND_H * Math.sin(angleH)), on, 3);
	    lcdG.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, 
	    		CLOCK_CENTER_X + (int)Math.round((CLOCK_HAND_M+CLOCK_SKEW) * Math.cos(angleM)), 
	    		CLOCK_CENTER_Y + (int)Math.round(CLOCK_HAND_M * Math.sin(angleM)), on, 2);
	    lcdG.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, 
	    		CLOCK_CENTER_X + (int)Math.round((CLOCK_HAND_S+CLOCK_SKEW) * Math.cos(angleS)), 
	    		CLOCK_CENTER_Y + (int)Math.round(CLOCK_HAND_S * Math.sin(angleS)), on, 1);
	}
	
	public void setup() {
		lcdG.begin();
		
	    // draw the clock frame, compensate for the display dots not being square
		lcdG.fillEllipse(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_RADIUS+CLOCK_SKEW, CLOCK_RADIUS, true);
		lcdG.fillEllipse(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_RADIUS+CLOCK_SKEW-2, CLOCK_RADIUS-2, false);

		// draw the hour lines
	    for (int i=0; i<12; i++) {
	        float angle = (-90.0f + i * 30.0f) * 3.1415f / 180.0f;
	        double ca = Math.cos(angle);
	        double sa = Math.sin(angle);
	        lcdG.drawLine(CLOCK_CENTER_X + (int)Math.round((CLOCK_RADIUS+CLOCK_SKEW-4) * ca), 
	        		CLOCK_CENTER_Y+ (int)Math.round((CLOCK_RADIUS-4) * sa), 
	        		CLOCK_CENTER_X + (int)Math.round((CLOCK_RADIUS+CLOCK_SKEW) * ca), 
	        		CLOCK_CENTER_Y+ (int)Math.round(CLOCK_RADIUS * sa), true, 1);
	    }
		
		// update the screen with all the changes
	    lcdG.updateDisplay();
	}

	public void loop() {
	    // calculate the angle in radians for seconds
	    float angleS = (-90.0f + seconds * 6.0f) * 3.1415f / 180.0f;
	    // calculate the angle in radians for minutes
	    float angleM = (-90.0f + minutes * 6.0f) * 3.1415f / 180.0f;
	    // calculate the angle in radians for hours (11h59 = 720mn)
	    float angleH = (-90.0f + (60*(hours%12) + minutes) * 0.5f) * 3.1415f / 180.0f;
		
	    // draw hands
	    drawHands(angleH, angleM, angleS, true);
	    lcdG.updateDisplay();
	        
	    // wait for 1 second
	    delay(1000);
	    // erase the previous lines, to prepare for next drawing
	    drawHands(angleH, angleM, angleS, false);

	    // increase the seconds, minutes, hours and days
	    seconds ++;
	    if (seconds > 59) {
	        seconds = 0;
	        minutes ++;
	        if (minutes > 59) {
	            minutes = 0;
	            hours ++;
	            if (hours > 24) {
	                hours = 0;
	                days ++;
	            }
	        }
	    }   
		
	}
}
