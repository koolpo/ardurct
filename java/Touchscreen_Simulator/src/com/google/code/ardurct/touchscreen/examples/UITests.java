package com.google.code.ardurct.touchscreen.examples;

import com.google.code.ardurct.touchscreen.IUIActionCallback;
import com.google.code.ardurct.touchscreen.IUIDrawCallback;

public class UITests extends Example {

	public int listTab;
	public int testsTab;
	public int colorsTab;
	
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
	public static int GAUGE_2_ID = 150;
	public static int SLIDER_2_ID = 160;
	
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
	    listTab = touchscreen.addUITab(stringToArray("List"));
	    testsTab = touchscreen.addUITab(stringToArray("Tests"), doTestsAction);
	    colorsTab = touchscreen.addUITab(stringToArray("Colors"), setColor, drawColor);
	    
	    touchscreen.addUILabel(testsTab, LABEL_B_ID, 5, 5, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Button"));
	    touchscreen.addUIButton(testsTab, TEST_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_B_ID, 5, UI_AUTO_SIZE, UI_AUTO_SIZE, 
	    		stringToArray("Test"));
	    touchscreen.addUILabel(testsTab, LABEL_O_ID, 5, 35, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Option"));
	    touchscreen.addUIPushButton(testsTab, PUSH_1_ID, GROUP_1, UI_RIGHT_OF_WITH_MARGIN + LABEL_O_ID, 35, 
	    		UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("30"), UI_SELECTED);
	    touchscreen.addUIPushButton(testsTab, PUSH_2_ID, GROUP_1, UI_RIGHT_OF + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, 
	    		UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, stringToArray("60"), UI_UNSELECTED);
	    touchscreen.addUIPushButton(testsTab, PUSH_3_ID, GROUP_1, UI_RIGHT_OF + PUSH_2_ID, UI_SAME_AS + PUSH_1_ID, 
	    		UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, stringToArray("90"), UI_UNSELECTED);
	    touchscreen.addUILabel(testsTab, LABEL_1_ID, 5, 65, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Push button"));
	    touchscreen.addUIPushButton(testsTab, T1_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + LABEL_1_ID, 
	    		UI_SAME_AS + LABEL_1_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("T1"), UI_SELECTED);
	    touchscreen.addUIPushButton(testsTab, T2_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + T1_ID, UI_SAME_AS + T1_ID, 
	    		UI_SAME_AS + T1_ID, UI_SAME_AS + T1_ID, stringToArray("T2"), UI_UNSELECTED);
	    touchscreen.addUILabel(testsTab, LABEL_2_ID, 5, 95, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Gauge"));
	    touchscreen.addUIGauge(testsTab, GAUGE_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_2_ID, UI_SAME_AS + LABEL_2_ID, 
	    		150, UI_SAME_AS + LABEL_2_ID, 40, 0, 100);
	    touchscreen.addUILabel(testsTab, LABEL_3_ID, 5, 125, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Slider"));
	    touchscreen.addUISlider(testsTab, SLIDER_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_3_ID, UI_SAME_AS + LABEL_3_ID, 
	    		150, UI_SAME_AS + LABEL_3_ID, 40, 0, 100);
	    touchscreen.addUISlider(testsTab, SLIDER_2_ID, 15, 150, 20, 120, 2, 0, 10);
	    touchscreen.addUIGauge(testsTab, GAUGE_2_ID, 60, 150, 20, 120, 2, 0, 10);
	    touchscreen.setUITab(testsTab);
	    
	    touchscreen.addUILabel(colorsTab, LABEL_R_ID, 5, 15, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("R"));
	    touchscreen.addUISlider(colorsTab, SLIDER_R_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 15, 180, 
	    		UI_SAME_AS + LABEL_R_ID, 0x15, 0, 0x1F);
	    touchscreen.addUILabel(colorsTab, 0, 5, 45, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("G"));
	    touchscreen.addUISlider(colorsTab, SLIDER_G_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 45, 180, 
	    		UI_SAME_AS + LABEL_R_ID, 0x1E, 0, 0x3F);
	    touchscreen.addUILabel(colorsTab, 0, 5, 75, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("B"));
	    touchscreen.addUISlider(colorsTab, SLIDER_B_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 75, 180, 
	    		UI_SAME_AS + LABEL_R_ID, 0xD, 0, 0x1F);
	    touchscreen.addUIArea(colorsTab, AREA_COLOR_ID, 50, 130, 140, 140);
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
			if (id == SLIDER_1_ID) touchscreen.setUIElementValue(GAUGE_1_ID, touchscreen.getUIElementValue(SLIDER_1_ID));
			if (id == SLIDER_2_ID) touchscreen.setUIElementValue(GAUGE_2_ID, touchscreen.getUIElementValue(SLIDER_2_ID));
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
				touchscreen.fillRectangle(x, y, width, 40, WHITE);
				touchscreen.setCursor(x+5, y);
				touchscreen.print("RGB = 0x");
				touchscreen.print(touchscreen.getUIElementValue(SLIDER_R_ID), HEX);
				touchscreen.print(" 0x");
				touchscreen.print(touchscreen.getUIElementValue(SLIDER_G_ID), HEX);
				touchscreen.print(" 0x");
				touchscreen.print(touchscreen.getUIElementValue(SLIDER_B_ID), HEX);
				touchscreen.setCursor(x+5, y+touchscreen.getFontHeight(FONT_SMALL)+4);
				touchscreen.print("RGB565 = 0x");
				touchscreen.print(value, HEX);
				touchscreen.fillRectangle(x, y+30, width, height-30, value);
				touchscreen.drawRectangle(x, y+30, width, height-30, BLACK, 1);
			}
		}
	}
	

}

