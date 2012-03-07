/*
 * UITests - Tests for the Touchscreen UI
 *
 * Copyright (c) 2012 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
/**
 * To help draw the UI, the coordinates of the UI elements can be expressed relative to other UI elements
 *   UI_AUTO_SIZE                               for with and height: will surround the text with UI_ELEMENT_TOP_MARGIN or UI_ELEMENT_LEFT_MARGIN
 *   UI_SAME_AS + uiElementId                   will return the corresponding coordinate value from uiElementId
 *   UI_LEFT_OF + uiElementId                   will return the left coordinate of uiElementId
 *   UI_RIGHT_OF    + uiElementId               will return the right coordinate of uiElementId
 *   UI_RIGHT_OF_WITH_MARGIN    + uiElementId   will return the right coordinate of uiElementId plus UI_ELEMENT_LEFT_MARGIN
 *   UI_TOP_OF + uiElementId                    will return the top coordinate of uiElementId
 *   UI_BOTTOM_OF + uiElementId                 will return the bottom coordinate of uiElementId
 *   UI_BOTTOM_OF_WITH_MARGIN + uiElementId     will return the bottom coordinate of uiElementId plus UI_ELEMENT_TOP_MARGIN
 **/ 
 
#include <TouchScreen.h>

#define TEST_ID       100
#define PUSH_1_ID     101
#define PUSH_2_ID     102
#define PUSH_3_ID     103
#define T1_ID         110
#define T2_ID         111
#define LABEL_1_ID    120
#define LABEL_2_ID    125
#define LABEL_3_ID    127
#define GAUGE_1_ID    130
#define SLIDER_1_ID   140
#define GAUGE_2_ID    150
#define SLIDER_2_ID   160

#define SLIDER_R_ID   200
#define SLIDER_G_ID   201
#define SLIDER_B_ID   202
#define LABEL_R_ID    205
#define AREA_COLOR_ID 210

#define GROUP_1 3

TouchScreen touchscreen;

uint8_t listTab;
uint8_t testsTab;
uint8_t colorsTab;

void setup() {
    Serial.begin(57600);
	
    // initialize the screen
    touchscreen.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touchscreen.setBacklight(180);
    
    // setup the UI to take the whole screen
    touchscreen.setupUI(0, 0, touchscreen.getWidth(), touchscreen.getHeight());
    
    // add tabs at the top
    listTab = touchscreen.addUITab("List");
    testsTab = touchscreen.addUITab("Tests", &doTestsAction);
    colorsTab = touchscreen.addUITab("Colors", &setColor, &drawColor);
    
    // populate the tests tab
    touchscreen.addUIButton(testsTab, TEST_ID, 5, 5, UI_AUTO_SIZE, UI_AUTO_SIZE, "Test");
    touchscreen.addUIPushButton(testsTab, PUSH_1_ID, GROUP_1, 5, 35, UI_AUTO_SIZE, UI_AUTO_SIZE, "30", UI_SELECTED);
    touchscreen.addUIPushButton(testsTab, PUSH_2_ID, GROUP_1, UI_RIGHT_OF + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, "60", UI_UNSELECTED);
    touchscreen.addUIPushButton(testsTab, PUSH_3_ID, GROUP_1, UI_RIGHT_OF + PUSH_2_ID, UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, "90", UI_UNSELECTED);
    touchscreen.addUILabel(testsTab, LABEL_1_ID, 5, 65, UI_AUTO_SIZE, UI_AUTO_SIZE, "Push");
    touchscreen.addUIPushButton(testsTab, T1_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + LABEL_1_ID, UI_SAME_AS + LABEL_1_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, "Test 1", UI_SELECTED);
    touchscreen.addUIPushButton(testsTab, T2_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + T1_ID, UI_SAME_AS + T1_ID, UI_SAME_AS + T1_ID, UI_SAME_AS + T1_ID, "Test 2", UI_UNSELECTED);
    touchscreen.addUILabel(testsTab, LABEL_2_ID, 5, 95, UI_AUTO_SIZE, UI_AUTO_SIZE, "Gauge");
    touchscreen.addUIGauge(testsTab, GAUGE_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_2_ID, UI_SAME_AS + LABEL_2_ID, 150, UI_SAME_AS + LABEL_2_ID, 40, 0, 100);
    touchscreen.addUILabel(testsTab, LABEL_3_ID, 5, 125, UI_AUTO_SIZE, UI_AUTO_SIZE, "Slider");
    touchscreen.addUISlider(testsTab, SLIDER_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_3_ID, UI_SAME_AS + LABEL_3_ID, 150, UI_SAME_AS + LABEL_3_ID, 40, 0, 100);
    touchscreen.addUISlider(testsTab, SLIDER_2_ID, 15, 150, 20, 120, 2, 0, 10);
    touchscreen.addUIGauge(testsTab, GAUGE_2_ID, 60, 150, 20, 120, 2, 0, 10);
    
    // populate the colors tab
    touchscreen.addUILabel(colorsTab, LABEL_R_ID, 5, 15, UI_AUTO_SIZE, UI_AUTO_SIZE, "R");
    touchscreen.addUISlider(colorsTab, SLIDER_R_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 15, 180, UI_SAME_AS + LABEL_R_ID, 0x15, 0, 0x1F);
    touchscreen.addUILabel(colorsTab, 0, 5, 45, UI_AUTO_SIZE, UI_AUTO_SIZE, "G");
    touchscreen.addUISlider(colorsTab, SLIDER_G_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 45, 180, UI_SAME_AS + LABEL_R_ID, 0x1E, 0, 0x3F);
    touchscreen.addUILabel(colorsTab, 0, 5, 75, UI_AUTO_SIZE, UI_AUTO_SIZE, "B");
    touchscreen.addUISlider(colorsTab, SLIDER_B_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 75, 180, UI_SAME_AS + LABEL_R_ID, 0xD, 0, 0x1F);
    touchscreen.addUIArea(colorsTab, AREA_COLOR_ID, 50, 130, 140, 140);
    touchscreen.setUIElementValue(AREA_COLOR_ID, 0xABCD);

    // start the UI on the tests tab
    touchscreen.setUITab(testsTab);
}

void loop() {
    // we have to call this function as often as possible to get a good responsivity of the UI
    touchscreen.handleUI();
    delay(5);
}

void doTestsAction(uint8_t id) {
    if (id == SLIDER_1_ID) touchscreen.setUIElementValue(GAUGE_1_ID, touchscreen.getUIElementValue(SLIDER_1_ID));
    else if (id == SLIDER_2_ID) touchscreen.setUIElementValue(GAUGE_2_ID, touchscreen.getUIElementValue(SLIDER_2_ID));
}

void setColor(uint8_t id) {
    int16_t r = touchscreen.getUIElementValue(SLIDER_R_ID);
    int16_t g = touchscreen.getUIElementValue(SLIDER_G_ID);
    int16_t b = touchscreen.getUIElementValue(SLIDER_B_ID);
    touchscreen.setUIElementValue(AREA_COLOR_ID, (r << 11) + (g << 5) + b);
}

void drawColor(uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value) {
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
