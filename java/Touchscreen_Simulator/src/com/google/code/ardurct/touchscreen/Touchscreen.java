package com.google.code.ardurct.touchscreen;

public class Touchscreen extends Macros {
	
	private static final long serialVersionUID = 1L;

	public void begin(int foregroundColor, int backgroundColor, int fontSize, boolean fontBold, boolean fontOverlay) {
		initScreen();
		setRotation(0);
		setFont(fontSize, fontBold, fontOverlay);
		setBackgroundColor(backgroundColor);
		setForegroundColor(foregroundColor);
		if (backgroundColor != BLACK) fillRectangle(0, 0, getWidth(), getHeight(), backgroundColor, true);
	}

	void setupBacklight(int backlightPin) { 	
	}

	public void setBacklight(int value) { 
	}

	int getBacklight() {
		return 0;
	}


}
