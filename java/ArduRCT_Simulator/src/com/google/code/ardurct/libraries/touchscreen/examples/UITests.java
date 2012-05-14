package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;
import com.google.code.ardurct.libraries.touchscreen.IUIActionCallback;
import com.google.code.ardurct.libraries.touchscreen.IUIDrawCallback;

public class UITests extends ArduinoFirmware {

	public int listTab;
	public int testsTab;
	public int colorsTab;
	
	public static int POPUP_ID = 99;
	public static int TEST_ID = 100;
	public static int PUSH_1_ID = 101;
	public static int PUSH_2_ID = 102;
	public static int PUSH_3_ID = 103;
	public static int T1_ID = 110;
	public static int T2_ID = 111;
	public static int LABEL_B_ID = 118;
	public static int LABEL_O_ID = 119;
	public static int LABEL_1_ID = 120;
	public static int LABEL_2_ID = 125;
	public static int LABEL_3_ID = 127;
	public static int GAUGE_1_ID = 130;
	public static int SLIDER_1_ID = 140;
	
	public static int SLIDER_R_ID = 200;
	public static int SLIDER_G_ID = 201;
	public static int SLIDER_B_ID = 202;
	public static int LABEL_R_ID = 205;
	public static int AREA_COLOR_ID = 210;
	
	public static int GROUP_1 = 3;

	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.setBacklight(180);
	    
	    touchscreen.setupUI(0, 0, touchscreen.getWidth(), touchscreen.getHeight());
	    listTab = touchscreen.addUITab(stringToArray("    "), null, drawList);
	    testsTab = touchscreen.addUITab(stringToArray("Tests"), doTestsAction);
	    colorsTab = touchscreen.addUITab(stringToArray("Colors"), setColor, drawColor);
	    
	    touchscreen.addUILabel(testsTab, LABEL_B_ID, 5, 10, UI_AUTO_SIZE, UI_AUTO_SIZE, "Button");
	    touchscreen.addUIButton(testsTab, TEST_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_B_ID, UI_SAME_AS + LABEL_B_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, 
	    		"Test");
	    touchscreen.addUIButton(testsTab, POPUP_ID, UI_RIGHT_OF_WITH_MARGIN + TEST_ID, UI_SAME_AS + LABEL_B_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, 
	    	"Popup");
	    
	    touchscreen.addUILabel(testsTab, LABEL_O_ID, 5, UI_BOTTOM_OF_WITH_MARGIN + LABEL_B_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, "Option");
	    touchscreen.addUIPushButton(testsTab, PUSH_1_ID, GROUP_1, UI_RIGHT_OF_WITH_MARGIN + LABEL_O_ID, UI_SAME_AS + LABEL_O_ID, 
	    		UI_AUTO_SIZE, UI_AUTO_SIZE, "30", UI_SELECTED);
	    touchscreen.addUIPushButton(testsTab, PUSH_2_ID, GROUP_1, UI_RIGHT_OF + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, 
	    		UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, "60", UI_UNSELECTED);
	    touchscreen.addUIPushButton(testsTab, PUSH_3_ID, GROUP_1, UI_RIGHT_OF + PUSH_2_ID, UI_SAME_AS + PUSH_1_ID, 
	    		UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, "90", UI_UNSELECTED);

	    touchscreen.addUILabel(testsTab, LABEL_1_ID, 5, UI_BOTTOM_OF_WITH_MARGIN + LABEL_O_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, "Push button");
	    touchscreen.addUIPushButton(testsTab, T1_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + LABEL_1_ID, 
	    		UI_SAME_AS + LABEL_1_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, "T1", UI_SELECTED);
	    touchscreen.addUIPushButton(testsTab, T2_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + T1_ID, UI_SAME_AS + T1_ID, 
	    		UI_SAME_AS + T1_ID, UI_SAME_AS + T1_ID, "T2", UI_UNSELECTED);
	    
	    touchscreen.addUILabel(testsTab, LABEL_2_ID, 5, UI_BOTTOM_OF_WITH_MARGIN + LABEL_1_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, "Slider");
	    touchscreen.addUISlider(testsTab, SLIDER_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_2_ID, UI_SAME_AS + LABEL_2_ID, 150, UI_SAME_AS + LABEL_2_ID, 40, 0, 100);
	    touchscreen.addUILabel(testsTab, LABEL_3_ID, 5, UI_BOTTOM_OF_WITH_MARGIN + LABEL_2_ID, UI_AUTO_SIZE, UI_AUTO_SIZE,  "Gauge");
	    touchscreen.addUIGauge(testsTab, GAUGE_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_2_ID, UI_SAME_AS + LABEL_3_ID, 
	    		UI_SAME_AS + SLIDER_1_ID, UI_SAME_AS + LABEL_2_ID, 40, 0, 100);
	    
	    touchscreen.setUITab(testsTab);
	    
	    touchscreen.addUILabel(colorsTab, LABEL_R_ID, 5, 15, UI_AUTO_SIZE, UI_AUTO_SIZE,"R");
	    touchscreen.addUISlider(colorsTab, SLIDER_R_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 15, 180, UI_SAME_AS + LABEL_R_ID, 0x15, 0, 0x1F);
	    touchscreen.addUILabel(colorsTab, 0, 5, 60, UI_AUTO_SIZE, UI_AUTO_SIZE, "G");
	    touchscreen.addUISlider(colorsTab, SLIDER_G_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 60, 180, UI_SAME_AS + LABEL_R_ID, 0x1E, 0, 0x3F);
	    touchscreen.addUILabel(colorsTab, 0, 5, 105, UI_AUTO_SIZE, UI_AUTO_SIZE, "B");
	    touchscreen.addUISlider(colorsTab, SLIDER_B_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 105, 180, UI_SAME_AS + LABEL_R_ID, 0xD, 0, 0x1F);
	    touchscreen.addUIArea(colorsTab, AREA_COLOR_ID, 50, UI_BOTTOM_OF_WITH_MARGIN + SLIDER_B_ID, 140, 100);
	    touchscreen.setUIElementEditable(AREA_COLOR_ID, false);
	    touchscreen.setUIElementValue(AREA_COLOR_ID, 0xABCD);
	    
	}

	public void loop() {
		touchscreen.handleUI();
	    delay(20);
	}

	private ActionCB doTestsAction = new ActionCB();
	class ActionCB implements IUIActionCallback {
		public void uiActionCallback(int id) {
			if (id == POPUP_ID) touchscreen.showUIPopup("Test", "A small message", BLACK);
			if (id == SLIDER_1_ID) touchscreen.setUIElementValue(GAUGE_1_ID, touchscreen.getUIElementValue(SLIDER_1_ID));
		}
	}
	
	private SetColorCB setColor = new SetColorCB();
	class SetColorCB implements IUIActionCallback {
		public void uiActionCallback(int id) {
			touchscreen.setUIElementValue(AREA_COLOR_ID, 
					(touchscreen.getUIElementValue(SLIDER_R_ID) << 11) + 
					(touchscreen.getUIElementValue(SLIDER_G_ID) << 5) + 
					touchscreen.getUIElementValue(SLIDER_B_ID));
		}
	}
	
	private DrawColorCB drawColor = new DrawColorCB();
	class DrawColorCB implements IUIDrawCallback {
		public void uiDrawCallback(int id, int x, int y, int width, int height, int value) {
			if (id == AREA_COLOR_ID) {
				touchscreen.fillRectangle(x, y, width, height, WHITE);
				touchscreen.fillRectangle(x, y, width, height-30, value);
				touchscreen.drawRectangle(x, y, width, height-30, BLACK, 1);
				touchscreen.setCursor(x+5, y+height-22);
				touchscreen.print("RGB = 0x");
				touchscreen.print(touchscreen.getUIElementValue(SLIDER_R_ID), HEX);
				touchscreen.print(" 0x");
				touchscreen.print(touchscreen.getUIElementValue(SLIDER_G_ID), HEX);
				touchscreen.print(" 0x");
				touchscreen.print(touchscreen.getUIElementValue(SLIDER_B_ID), HEX);
				touchscreen.setCursor(x+5, y+touchscreen.getFontHeight(FONT_SMALL)+4+height-22);
				touchscreen.print("RGB565 = 0x");
				touchscreen.print(value, HEX);
			}
		}
	}
	
	private DrawListCB drawList = new DrawListCB();
	class DrawListCB implements IUIDrawCallback {
		public void uiDrawCallback(int id, int x, int y, int width, int height, int value) {
			if (id == UI_DRAW_CALLBACK_TAB_ID) {
				touchscreen.drawString("List", x+10, y+12, value == UI_SELECTED ? RED : BLUE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
			}
		}
	}

}

