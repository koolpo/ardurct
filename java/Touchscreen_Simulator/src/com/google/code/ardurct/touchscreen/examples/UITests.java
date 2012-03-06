package com.google.code.ardurct.touchscreen.examples;

import com.google.code.ardurct.touchscreen.IUIActionCallback;
import com.google.code.ardurct.touchscreen.IUIDrawCallback;


public class UITests extends Example 
implements IUIActionCallback, IUIDrawCallback {

	public int LIST_TAB;
	public int TESTS_TAB;
	public int COLORS_TAB;
	
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
	    LIST_TAB = touchscreen.addUITab(stringToArray("List"));
	    TESTS_TAB = touchscreen.addUITab(stringToArray("Tests"));
	    COLORS_TAB = touchscreen.addUITab(stringToArray("Colors"));
	    
	    touchscreen.addUILabel(TESTS_TAB, LABEL_B_ID, 5, 5, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Button"));
	    touchscreen.addUIButton(TESTS_TAB, TEST_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_B_ID, 5, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Test"), this);
	    touchscreen.addUILabel(TESTS_TAB, LABEL_O_ID, 5, 35, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Option"));
	    touchscreen.addUIPushButton(TESTS_TAB, PUSH_1_ID, GROUP_1, UI_RIGHT_OF_WITH_MARGIN + LABEL_O_ID, 35, 
	    		UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("30"), UI_SELECTED, this);
	    touchscreen.addUIPushButton(TESTS_TAB, PUSH_2_ID, GROUP_1, UI_RIGHT_OF + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, 
	    		UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, stringToArray("60"), UI_UNSELECTED, this);
	    touchscreen.addUIPushButton(TESTS_TAB, PUSH_3_ID, GROUP_1, UI_RIGHT_OF + PUSH_2_ID, UI_SAME_AS + PUSH_1_ID, 
	    		UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, stringToArray("90"), UI_UNSELECTED, this);
	    touchscreen.addUILabel(TESTS_TAB, LABEL_1_ID, 5, 65, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Push button"));
	    touchscreen.addUIPushButton(TESTS_TAB, T1_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + LABEL_1_ID, UI_SAME_AS + LABEL_1_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("T1"), UI_SELECTED, this);
	    touchscreen.addUIPushButton(TESTS_TAB, T2_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + T1_ID, UI_SAME_AS + T1_ID, 
	    		UI_SAME_AS + T1_ID, UI_SAME_AS + T1_ID, stringToArray("T2"), UI_UNSELECTED, this);
	    touchscreen.addUILabel(TESTS_TAB, LABEL_2_ID, 5, 95, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Gauge"));
	    touchscreen.addUIGauge(TESTS_TAB, GAUGE_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_2_ID, UI_SAME_AS + LABEL_2_ID, 
	    		150, UI_SAME_AS + LABEL_2_ID, 40, 0, 100);
	    touchscreen.addUILabel(TESTS_TAB, LABEL_3_ID, 5, 125, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("Slider"));
	    touchscreen.addUISlider(TESTS_TAB, SLIDER_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_3_ID, UI_SAME_AS + LABEL_3_ID, 
	    		150, UI_SAME_AS + LABEL_3_ID, 40, 0, 100, this);
	    touchscreen.addUISlider(TESTS_TAB, SLIDER_2_ID, 15, 150, 20, 120, 2, 0, 10, this);
	    touchscreen.addUIGauge(TESTS_TAB, GAUGE_2_ID, 60, 150, 20, 120, 2, 0, 10);
	    touchscreen.setUITab(TESTS_TAB);
	    
	    touchscreen.addUILabel(COLORS_TAB, LABEL_R_ID, 5, 15, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("R"));
	    touchscreen.addUISlider(COLORS_TAB, SLIDER_R_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 15, 180, UI_SAME_AS + LABEL_R_ID, 0x15, 0, 0x1F, this);
	    touchscreen.addUILabel(COLORS_TAB, 0, 5, 45, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("G"));
	    touchscreen.addUISlider(COLORS_TAB, SLIDER_G_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 45, 180, UI_SAME_AS + LABEL_R_ID, 0x1E, 0, 0x3F, this);
	    touchscreen.addUILabel(COLORS_TAB, 0, 5, 75, UI_AUTO_SIZE, UI_AUTO_SIZE, stringToArray("B"));
	    touchscreen.addUISlider(COLORS_TAB, SLIDER_B_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 75, 180, UI_SAME_AS + LABEL_R_ID, 0xD, 0, 0x1F, this);
	    touchscreen.addUIArea(COLORS_TAB, AREA_COLOR_ID, 50, 130, 140, 140, null, this);
	    touchscreen.setUIElementEditable(AREA_COLOR_ID, false);
	    touchscreen.setUIElementValue(AREA_COLOR_ID, 0xABCD);
	    
	}

	public void loop() {
		touchscreen.handleUI();
	    delay(20);
	}

	public void uiActionCallback(int id) {
		if (id == SLIDER_1_ID) touchscreen.setUIElementValue(GAUGE_1_ID, touchscreen.getUIElementValue(SLIDER_1_ID));
		if (id == SLIDER_2_ID) touchscreen.setUIElementValue(GAUGE_2_ID, touchscreen.getUIElementValue(SLIDER_2_ID));
		if ((id >= SLIDER_R_ID) && (id <= SLIDER_B_ID)) {
			touchscreen.setUIElementValue(AREA_COLOR_ID, 
					(touchscreen.getUIElementValue(SLIDER_R_ID) << 11) + (touchscreen.getUIElementValue(SLIDER_G_ID) << 5) + touchscreen.getUIElementValue(SLIDER_B_ID));
		}
	}

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
