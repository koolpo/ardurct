package com.google.code.ardurct.libraries.graphics.examples;

import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIButton;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIElement;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIGauge;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUILabel;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIListItem;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIOption;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIScreen;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUISlider;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUITab;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIValue;
import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.graphics.GraphicsFirmware;
import com.google.code.ardurct.libraries.graphics.IUIActionCallback;
import com.google.code.ardurct.libraries.graphics.IUIDrawCallback;

public class UITest extends GraphicsFirmware {

	// ------------------ Java specific ------------------------- //
	private buttonsCB handleButtons = new buttonsCB();
	private footerDrawCB footerDrawer = new footerDrawCB();
	// ------------------ Java specific ------------------------- //

	ArduRCT_JAVA graphics = new ArduRCT_JAVA();
	
	ArduRCT_GraphicsUIScreen home = new ArduRCT_GraphicsUIScreen();
	ArduRCT_GraphicsUIScreen screen = new ArduRCT_GraphicsUIScreen();
	ArduRCT_GraphicsUIScreen menu = new ArduRCT_GraphicsUIScreen();
	ArduRCT_GraphicsUIScreen popup = new ArduRCT_GraphicsUIScreen();
	ArduRCT_GraphicsUIScreen currentScreen;

	public void setup() {
		Serial.begin(9600);
		graphics.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);

		graphics.addScreen(home);
		// show a few examples of elements
		home.addElement(new ArduRCT_GraphicsUILabel(array("Label")), 5, 5, GUI_AS, GUI_AS);	
		home.addElement(new ArduRCT_GraphicsUIValue(70, 3, 0, 255, handleButtons, 1, 16), GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
		home.addElement(new ArduRCT_GraphicsUIOption(1, array("O1"), handleButtons, 1), 5, GUI_BOPWM, GUI_AS, GUI_AS);
		home.getElement(1).setValue(GRAPHICS_UI_SELECTED);
		home.addElement(new ArduRCT_GraphicsUIOption(2, array("O2"), handleButtons, 1), GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
		home.getElement(2).editable = false;
		for (int i=3; i<4; i++) 
			home.addElement(new ArduRCT_GraphicsUIOption(i, array("O"+i), handleButtons, 1), GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
		home.addElement(new ArduRCT_GraphicsUIButton(20, array("Menu"), handleButtons), 5, GUI_BOPWM, GUI_AS, GUI_AS);
		home.addElement(new ArduRCT_GraphicsUIButton(21, array("Popup"), handleButtons), GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
		home.addElement(new ArduRCT_GraphicsUIElement(25), 5, GUI_BOPWM, 118, GUI_SAP);
		home.addElement(new ArduRCT_GraphicsUISlider(25, 50, 0, 100, handleButtons, 5), GUI_SAP, GUI_BOPWM, 118, 20);
		home.addElement(new ArduRCT_GraphicsUISlider(29, 70, 0, 100, handleButtons, 5), GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);
		home.addElement(new ArduRCT_GraphicsUISlider(30, 10, 0, 100, handleButtons, 5), GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);

		// set up a header with tabs
		home.addElement(new ArduRCT_GraphicsUITab(42, array("T1"), handleButtons, 1), 0, 0, GUI_AS, GUI_AS, GUI_HEADER);
		home.getElement(42).setValue(GRAPHICS_UI_SELECTED);
		home.addElement(new ArduRCT_GraphicsUITab(43, array("T2"), handleButtons, 1), GUI_ROP, 0, GUI_AS, GUI_AS, GUI_HEADER);
		// set a footer
		home.addElement(new ArduRCT_GraphicsUIElement(90, footerDrawer), 0, 0, GUI_W, 14, GUI_FOOTER);
		home.getElement(90).editable = false;
		
		graphics.addScreen(screen);
		// reuse the tabs for another screen
		screen.addElement(home.getElement(42));
		// reuse the footer for another screen
		screen.addElement(home.getElement(90));
		// check other UI elements
		screen.addElement(new ArduRCT_GraphicsUISlider(105, 10, 0, 20, handleButtons, 1), 10, 5, 20, 90);
		screen.addElement(new ArduRCT_GraphicsUIGauge(106, screen.getElementValue(105), 0, 20), GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
		screen.addElement(new ArduRCT_GraphicsUILabel(107, array("Test"), FONT_SMALL), GUI_SAP, GUI_BOP, GUI_AS, GUI_AS);
		screen.getElement(107).y ++; 
		screen.getElement(107).x -= 4;
		
		// create a popup
		graphics.addScreen(popup);
		popup.addElement(new ArduRCT_GraphicsUILabel(array("Are you really sureyou want to exit ?"), FONT_SMALL), 6, 5, GUI_AS, GUI_AS);
		popup.addElement(new ArduRCT_GraphicsUIButton(70, array("Yes"), handleButtons), 5, GUI_BOPWM, GUI_AS, GUI_AS);
		popup.addElement(new ArduRCT_GraphicsUIButton(71, array("No"), handleButtons), GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
		popup.pack(GRAPHICS_POSITION_CENTER);
		popup.setBackgroundColor(GRAPHICS_UI_COLOR_POPUP_BACKGROUND);

		// define a menu
		graphics.addScreen(menu);
		menu.addElement(new ArduRCT_GraphicsUIListItem(51, array("Item 01"), handleButtons), 0, 0, 90, GUI_AS);
		for (int i=2; i<12; i++) 
			menu.addElement(new ArduRCT_GraphicsUIListItem(50+i, array("Item " + (i<10?"0":"") + i), handleButtons), 0, GUI_BOP, GUI_SAP, GUI_SAP);
		// pack it as small as possible and make the menu popup from the SW of the screen
		menu.pack(GRAPHICS_POSITION_SW);
		
		graphics.setGraphicsUIHome(home, home.getElement(42));
		graphics.setGraphicsUIScreen(home);
	}

	public void loop() {
		delay(5);
		if (Serial.available() == 0) return;
		
		int value = Serial.read();
		int actionId = 0;
		switch (value) {
			case 'm': actionId = GRAPHICS_UI_ACTION_MENU; break;
			case 'e': actionId = GRAPHICS_UI_ACTION_ENTER; break;
			case 'u': actionId = GRAPHICS_UI_ACTION_UP; break;
			case 'd': actionId = GRAPHICS_UI_ACTION_DOWN; break;
		}
		if (actionId != 0) graphics.handleGraphicsUI(actionId, 0, 0);
		if (actionId == GRAPHICS_UI_ACTION_ENTER) graphics.handleGraphicsUI(actionId, 0, 0);
	}

	class buttonsCB implements IUIActionCallback {
		public boolean run(int elementId, int value) {
			if (elementId == 43) graphics.setGraphicsUIScreen(screen);
			else if (elementId == 42) graphics.setGraphicsUIScreen(home);
			else if (elementId == 20) graphics.setGraphicsUIScreen(menu);
			else if (elementId == 21) graphics.setGraphicsUIScreen(popup);
			else if ((elementId >= 70) && (elementId <= 71)) graphics.setGraphicsUIScreen(home);
			else if ((elementId >= 51) && (elementId <= 62)) graphics.setGraphicsUIScreen(home);
			else if (elementId == 105) screen.setElementValue(106, screen.getElementValue(105));
			return false;
		}
	}
	
	class footerDrawCB implements IUIDrawCallback {
		public void draw(int id, int state, int value, int x, int y, int width, int height) {
			graphics.fillRectangle(x, y, width, height, VLIGHT_GREY);
			graphics.drawRectangle(x, y, width, height, GREY, 1);
			graphics.setBackgroundColor(VLIGHT_GREY);
			graphics.drawString("UITest", x+3, y-4+graphics.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
			graphics.drawString("v1.0", x+width-27, y-4+graphics.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
		}
	}
}
