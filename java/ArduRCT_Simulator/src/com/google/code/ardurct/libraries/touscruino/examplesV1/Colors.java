package com.google.code.ardurct.libraries.touscruino.examplesV1;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruino;

public class Colors extends TouscruinoFirmware {

	int FILL_SIZE = 40;
	int BUTTON_FONT = FONT_MEDIUM;
	boolean BUTTON_FONT_WEIGHT = FONT_BOLD;
	
	// ------------------ Java specific ------------------------- //
	private buttonsCB handleButtons = new buttonsCB();
	// ------------------ Java specific ------------------------- //

	ArduRCT_TouScruino Touscruino = new ArduRCT_TouScruino();

	ArduRCT_EventHandler buttonsHandler = new ArduRCT_EventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, handleButtons); 

	int colorPart[] = { 0, 0, 0 };
	int activeColor = 0;
	int buttonWidth;
	
	public void setup() {
	    Touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    Touscruino.setRotation(GRAPHICS_ROTATION_90);  
	    buttonWidth = Touscruino.getStringWidth("Reset", BUTTON_FONT) + 10;
	    drawButton("Red", 0, false);
	    drawButton("Up", 1, false);
	    drawButton("Down", 2, false);
	    drawButton("Reset", 3, false);
	    drawColor(BLACK);
	    
	    Touscruino.registerEventHandler(buttonsHandler);	
	}

	public void loop() {
		Touscruino.manageEvents();
	}

	void drawButton(String label, int nb, boolean active) {
		int height = Touscruino.getHeight()/4; 
		int y = nb*height;
		Touscruino.fillRectangle(Touscruino.getWidth()-buttonWidth, y, buttonWidth, height, active ? GREY : LIGHT_GREY);
		Touscruino.drawRectangle(Touscruino.getWidth()-buttonWidth, y - (nb == 0 ? 0 : 1), buttonWidth, height + (nb == 0 ? 0 : 1), BLACK, 1);
		Touscruino.setBackgroundColor(active ? GREY : LIGHT_GREY);
		int x = Touscruino.getWidth()-buttonWidth+(buttonWidth-Touscruino.getStringWidth(label, BUTTON_FONT))/2;
		y += (height-Touscruino.getFontHeight(BUTTON_FONT))/2;
		Touscruino.drawString(label, x, y, BLACK, BUTTON_FONT, BUTTON_FONT_WEIGHT, NO_OVERLAY);
	}
	
	void drawColor(int color) {
		int y = (Touscruino.getHeight()-FILL_SIZE-24)/2;
	    Touscruino.fillRectangle((Touscruino.getWidth()-FILL_SIZE-buttonWidth)/2, y, FILL_SIZE, FILL_SIZE, color);
	    Touscruino.drawRectangle((Touscruino.getWidth()-FILL_SIZE-buttonWidth)/2, y, FILL_SIZE, FILL_SIZE, BLACK, 1);
	    Touscruino.setBackgroundColor(WHITE);
	    Touscruino.setCursor(8, y + FILL_SIZE + 10);
	    Touscruino.print("R="); 
	    if (colorPart[0] < 0x10) Touscruino.print(0);
	    Touscruino.print(colorPart[0], HEX); 
	    Touscruino.print(" G="); 
	    if (colorPart[1] < 0x10) Touscruino.print(0);
	    Touscruino.print(colorPart[1], HEX); 
	    Touscruino.print(" B="); 
	    if (colorPart[2] < 0x10) Touscruino.print(0);
	    Touscruino.print(colorPart[2], HEX); 
	    Touscruino.setCursor(16, y + FILL_SIZE + 24);	   
	    Touscruino.print("Color=");
	    if (color < 0x1000) Touscruino.print(0);
	    if (color < 0x100) Touscruino.print(0);
	    if (color < 0x10) Touscruino.print(0);
	    Touscruino.print(color, HEX); 
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
