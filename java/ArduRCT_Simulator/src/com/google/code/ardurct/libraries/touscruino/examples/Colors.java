package com.google.code.ardurct.libraries.touscruino.examples;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruinoV1;

public class Colors extends TouscruinoFirmware {

	int FILL_SIZE = 40;
	int BUTTON_FONT = FONT_MEDIUM;
	boolean BUTTON_FONT_WEIGHT = FONT_BOLD;
	
	// ------------------ Java specific ------------------------- //
	private buttonsCB handleButtons = new buttonsCB();
	// ------------------ Java specific ------------------------- //

	ArduRCT_TouScruinoV1 touscruino = new ArduRCT_TouScruinoV1();

	ArduRCT_EventHandler buttonsHandler = new ArduRCT_EventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, handleButtons); 

	int colorPart[] = { 0, 0, 0 };
	int activeColor = 0;
	int buttonWidth;
	
	public void setup() {
	    touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touscruino.setRotation(GRAPHICS_ROTATION_90);  
	    buttonWidth = touscruino.getStringWidth("Reset", BUTTON_FONT) + 10;
	    drawButton("Red", 0, false);
	    drawButton("Up", 1, false);
	    drawButton("Down", 2, false);
	    drawButton("Reset", 3, false);
	    drawColor(BLACK);
	    
	    touscruino.registerEventHandler(buttonsHandler);	
	}

	public void loop() {
		touscruino.update();
	}

	void drawButton(String label, int nb, boolean active) {
		int height = touscruino.getHeight()/4; 
		int y = nb*height;
		touscruino.fillRectangle(touscruino.getWidth()-buttonWidth, y, buttonWidth, height, active ? GREY : LIGHT_GREY);
		touscruino.drawRectangle(touscruino.getWidth()-buttonWidth, y - (nb == 0 ? 0 : 1), buttonWidth, height + (nb == 0 ? 0 : 1), BLACK, 1);
		touscruino.setBackgroundColor(active ? GREY : LIGHT_GREY);
		int x = touscruino.getWidth()-buttonWidth+(buttonWidth-touscruino.getStringWidth(label, BUTTON_FONT))/2;
		y += (height-touscruino.getFontHeight(BUTTON_FONT))/2;
		touscruino.drawString(label, x, y, BLACK, BUTTON_FONT, BUTTON_FONT_WEIGHT, NO_OVERLAY);
	}
	
	void drawColor(int color) {
		int y = (touscruino.getHeight()-FILL_SIZE-24)/2;
	    touscruino.fillRectangle((touscruino.getWidth()-FILL_SIZE-buttonWidth)/2, y, FILL_SIZE, FILL_SIZE, color);
	    touscruino.drawRectangle((touscruino.getWidth()-FILL_SIZE-buttonWidth)/2, y, FILL_SIZE, FILL_SIZE, BLACK, 1);
	    touscruino.setBackgroundColor(WHITE);
	    touscruino.setCursor(8, y + FILL_SIZE + 10);
	    touscruino.print("R="); 
	    if (colorPart[0] < 0x10) touscruino.print(0);
	    touscruino.print(colorPart[0], HEX); 
	    touscruino.print(" G="); 
	    if (colorPart[1] < 0x10) touscruino.print(0);
	    touscruino.print(colorPart[1], HEX); 
	    touscruino.print(" B="); 
	    if (colorPart[2] < 0x10) touscruino.print(0);
	    touscruino.print(colorPart[2], HEX); 
	    touscruino.setCursor(16, y + FILL_SIZE + 24);	   
	    touscruino.print("Color=");
	    if (color < 0x1000) touscruino.print(0);
	    if (color < 0x100) touscruino.print(0);
	    if (color < 0x10) touscruino.print(0);
	    touscruino.print(color, HEX); 
	}
	
	class buttonsCB implements IEventCallback {
		public int handle(int eventType, int buttonId) {
		    if (eventType == EVENT_SWITCH_PRESSED) {
		        if (buttonId == TOUSCRUINO_MENU) {
		        	drawButton("Reset", 3, true);
		            for (int i=0; i< 3; i++) colorPart[i] = 0;
		        } else if (buttonId == TOUSCRUINO_ENTER) {
		        	activeColor ++;
		        	if (activeColor > 2) activeColor = 0;
		        	if (activeColor == 0) drawButton("Red", 0, true);
		    		else if (activeColor == 1) drawButton("Green", 0, true);
		    		else  drawButton("Blue", 0, true);
		        } else if (buttonId == TOUSCRUINO_UP) {
		        	colorPart[activeColor] += (activeColor == 1 ? 0x04 : 0x08);
		        	if (colorPart[activeColor] > 0xFF) colorPart[activeColor] = 0xFF;
		        	drawButton("Up", 1, true);
		        } else if (buttonId == TOUSCRUINO_DOWN) {
		        	colorPart[activeColor] -= (activeColor == 1 ? 0x04 : 0x08);
		        	if (colorPart[activeColor] < 0) colorPart[activeColor] = 0;
		        	drawButton("Down", 2, true);
		        }
		    } else if (eventType == EVENT_SWITCH_REPEATING) {
		    	if (buttonId == TOUSCRUINO_UP) {
		        	colorPart[activeColor] += (activeColor == 1 ? 0x04 : 0x08);
		        	if (colorPart[activeColor] > 0xFF) colorPart[activeColor] = 0xFF;
		        } else if (buttonId == TOUSCRUINO_DOWN) {
		        	colorPart[activeColor] -= (activeColor == 1 ? 0x04 : 0x08);
		        	if (colorPart[activeColor] < 0) colorPart[activeColor] = 0;
		        }
		    } else if (eventType == EVENT_SWITCH_RELEASED) {
		    	if (buttonId == TOUSCRUINO_MENU) drawButton("Reset", 3, false);
		    	else if (buttonId == TOUSCRUINO_UP) drawButton("Up", 1, false);
		    	else if (buttonId == TOUSCRUINO_DOWN) drawButton("Down", 2, false);
		    	else if (buttonId == TOUSCRUINO_ENTER) {
		    		if (activeColor == 0) drawButton("Red", 0, false);
		    		else if (activeColor == 1) drawButton("Green", 0, false);
		    		else  drawButton("Blue", 0, false);
		    	}
		    }
		    if (eventType != EVENT_SWITCH_RELEASED) {
			    int color = COLOR_565(colorPart[0], colorPart[1], colorPart[2]);
			    drawColor(color);
		    }
			return EVENT_HANDLING_DONE;
		}
		public int handle(int eventType, int eventValue, int eventX, int eventY) { return EVENT_HANDLING_VOID; }
		public int handle(int eventType) { return EVENT_HANDLING_VOID; }
	}

	
}
