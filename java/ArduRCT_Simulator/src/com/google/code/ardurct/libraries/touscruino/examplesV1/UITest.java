package com.google.code.ardurct.libraries.touscruino.examplesV1;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
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
import com.google.code.ardurct.libraries.graphics.IUIActionCallback;
import com.google.code.ardurct.libraries.graphics.IUIDrawCallback;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruino;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class UITest extends TouscruinoFirmware {

	// ------------------ Java specific ------------------------- //
	private buttonsCB handleButtons = new buttonsCB();
	private footerDrawCB footerDrawer = new footerDrawCB();
	private tickerDrawCB tickerDrawer = new tickerDrawCB();
	private timeHandlerCB refreshTime = new timeHandlerCB();
	// ------------------ Java specific ------------------------- //

	ArduRCT_TouScruino Touscruino = new ArduRCT_TouScruino();
	
	ArduRCT_GraphicsUIScreen home = new ArduRCT_GraphicsUIScreen();
	ArduRCT_GraphicsUIScreen screen = new ArduRCT_GraphicsUIScreen();
	ArduRCT_GraphicsUIScreen menu = new ArduRCT_GraphicsUIScreen();
	ArduRCT_GraphicsUIScreen popup = new ArduRCT_GraphicsUIScreen();
	
	public void setup() {
		Serial.begin(9600);
		Touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		//touscruino.setRotation(GRAPHICS_ROTATION_90);

		Touscruino.addScreen(home);
		// show a few examples of elements
		home.addElement(new ArduRCT_GraphicsUILabel(array("Label")), 5, 0, GUI_AS, GUI_AS);	
		home.addElement(new ArduRCT_GraphicsUIValue(70, 3, 0, 255, handleButtons, 1, 16), GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
		home.addElement(new ArduRCT_GraphicsUIOption(1, array("O1"), handleButtons, 1), 5, GUI_BOPWM, GUI_AS, GUI_AS);
		home.setElementValue(1, GRAPHICS_UI_PRESSED);
		home.addElement(new ArduRCT_GraphicsUIOption(2, array("O2"), handleButtons, 1), GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
		home.getElement(2).editable = false;
		for (int i=3; i<4; i++) 
			home.addElement(new ArduRCT_GraphicsUIOption(i, array("O"+i), handleButtons, 1), GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
		home.addElement(new ArduRCT_GraphicsUIButton(20, array("Menu"), handleButtons), 5, GUI_BOPWM, GUI_AS, GUI_AS);
		home.addElement(new ArduRCT_GraphicsUIButton(21, array("Popup"), handleButtons), GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
		home.addElement(new ArduRCT_GraphicsUISlider(27, 50, 0, 100, handleButtons, 5),5, GUI_BOPWM, 118, 20);
		home.addElement(new ArduRCT_GraphicsUISlider(29, 70, 0, 100, handleButtons, 5), GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);
		home.addElement(new ArduRCT_GraphicsUISlider(30, 10, 0, 100, handleButtons, 5), GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);
		home.addElement(new ArduRCT_GraphicsUIElement(25, tickerDrawer), 5, GUI_BOPWM, 118, GUI_SAP);

		// set up a header with tabs
		home.addElement(new ArduRCT_GraphicsUITab(42, array("T1"), handleButtons, 1), 0, 0, GUI_AS, GUI_AS, GUI_HEADER);
		home.getElement(42).setValue(GRAPHICS_UI_PRESSED);
		home.addElement(new ArduRCT_GraphicsUITab(43, array("T2"), handleButtons, 1), GUI_ROP, 0, GUI_AS, GUI_AS, GUI_HEADER);
		// set a footer
		home.addElement(new ArduRCT_GraphicsUIElement(90, footerDrawer), 0, 0, GUI_W, 14, GUI_FOOTER);
		home.getElement(90).editable = false;
		
		Touscruino.addScreen(screen);
		// reuse the tabs for screen
		screen.addElement(home.getElement(42));
		// reuse the footer for screen
		screen.addElement(home.getElement(90));
		// check other UI elements
		screen.addElement(new ArduRCT_GraphicsUISlider(105, 10, 0, 20, handleButtons, 1), 10, 5, 20, 90);
		screen.addElement(new ArduRCT_GraphicsUIGauge(106, screen.getElementValue(105), 0, 20), GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
		screen.addElement(new ArduRCT_GraphicsUILabel(107, array("Test"), FONT_SMALL), GUI_SAP, GUI_BOP, GUI_AS, GUI_AS);
		screen.getElement(107).y ++; 
		screen.getElement(107).x -= 4;
		
		Touscruino.addScreen(popup);
		// create a popup
		popup.addElement(new ArduRCT_GraphicsUILabel(array("Are you really sureyou want to exit ?"), FONT_SMALL), 6, 5, GUI_AS, GUI_AS);
		popup.addElement(new ArduRCT_GraphicsUIButton(70, array("Yes"), handleButtons), 5, GUI_BOPWM, GUI_AS, GUI_AS);
		popup.addElement(new ArduRCT_GraphicsUIButton(71, array("No"), handleButtons), GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
		popup.pack(GRAPHICS_POSITION_CENTER);
		popup.setBackgroundColor(GRAPHICS_UI_COLOR_POPUP_BACKGROUND);
		
		Touscruino.addScreen(menu);
		// define a menu
		menu.addElement(new ArduRCT_GraphicsUIListItem(51, array("Item 01"), handleButtons), 0, 0, 90, GUI_AS);
		for (int i=2; i<12; i++) 
			menu.addElement(new ArduRCT_GraphicsUIListItem(50+i, array("Item " + (i<10?"0":"") + i), handleButtons), 0, GUI_BOP, GUI_SAP, GUI_SAP);
		// pack it as small as possible and make the menu popup from the SW of the screen
		menu.pack(GRAPHICS_POSITION_SW);
		
		// let touscruino manage the buttons 
		Touscruino.setGraphicsUIHome(home, home.getElement(42));
		Touscruino.setGraphicsUIScreen(home);
		//Serial.print(Integer.toHexString(COLOR_565(0xB0, 0xDB, 0xE3)));
		
		Touscruino.registerEventHandler(new ArduRCT_EventHandler(EVENT_TIME, refreshTime));
	}

	public void loop() {
		delay(5);
		Touscruino.manageEvents();
	}

	class buttonsCB implements IUIActionCallback {
		public boolean run(int elementId, int state, int value) {
			if (state == GRAPHICS_UI_RELEASED) {
				if (elementId == 43) Touscruino.setGraphicsUIScreen(screen);
				else if (elementId == 42) Touscruino.setGraphicsUIScreen(home);
				else if (elementId == 20) Touscruino.setGraphicsUIScreen(menu);
				else if (elementId == 21) Touscruino.setGraphicsUIScreen(popup);
				else if ((elementId >= 70) && (elementId <= 71)) Touscruino.setGraphicsUIScreen(home);
				else if ((elementId >= 51) && (elementId <= 63)) Touscruino.setGraphicsUIScreen(home);
			} 
			if (elementId == 105) screen.setElementValue(106, screen.getElementValue(105));
			return false;
		}
	}
	
	class footerDrawCB implements IUIDrawCallback {
		public void draw(int id, int state, int value, int x, int y, int width, int height) {
			Touscruino.fillRectangle(x, y, width, height, VLIGHT_GREY);
			Touscruino.drawRectangle(x, y, width, height, GREY, 1);
			Touscruino.setBackgroundColor(VLIGHT_GREY);
			Touscruino.drawString("UITest", x+3, y-4+Touscruino.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
			Touscruino.drawString("v1.0", x+width-27, y-4+Touscruino.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
		}
	}
	
	class tickerDrawCB implements IUIDrawCallback {
		public void draw(int id, int state, int value, int x, int y, int width, int height) {
			Touscruino.fillRectangle(x, y, width, height, YELLOW);
			Touscruino.setBackgroundColor(YELLOW);
			int textX = (width - Touscruino.getStringWidth(Touscruino.getRTC().getTimeAsString(RTC_WITH_SECONDS), FONT_MEDIUM))/2;
			Touscruino.drawString(Touscruino.getRTC().getTimeAsString(RTC_WITH_SECONDS), 
					x+textX, y+(height-Touscruino.getFontHeight(FONT_MEDIUM))/2, BLACK, FONT_MEDIUM, FONT_BOLD, false);
			Touscruino.drawRectangle(x, y, width, height, GREY, 1);
		}
	}
	
	class timeHandlerCB implements IEventCallback {
		public int handle(int eventType) {
			// redraw the element
			home.setElementValue(25, 0);
			return EVENT_HANDLING_DONE;
		}
		
		public int handle(int type, int value, int x, int y) { return EVENT_HANDLING_VOID; }
		public int handle(int type, int value) { return EVENT_HANDLING_VOID; }
	}

}
