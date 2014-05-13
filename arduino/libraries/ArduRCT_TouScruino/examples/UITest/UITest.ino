/*
 * UITest - Testing the different controls of the ArduRCT_GraphicsUI
 *
 * Copyright (c) 2013 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software", to deal
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
 
 **/
 
#include <Wire.h>
#include <SPI.h>
#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>

// Change to your version: the following include will automatically create the proper 'Touscruino' object
#define TOUSCRUINO_VERSION 2
#include <ArduRCT_TouScruino.h>

#define FONT_FACE FONT_PLAIN

#define TAB1 20
#define TAB2 21
#define FOOTER 30
#define MENU 4
#define POPUP 5
#define TIMER 6

ArduRCT_GraphicsUIScreen screen1;
ArduRCT_GraphicsUIScreen screen2;
ArduRCT_GraphicsUIScreen menu;
ArduRCT_GraphicsUIScreen popup;

ArduRCT_GraphicsUILabel label1("Label");
ArduRCT_GraphicsUIValue value1(0, 3, 0, 255, &handleActions, 1, 16);
ArduRCT_GraphicsUIOption option1(1, "O1", &handleActions, 1);
ArduRCT_GraphicsUIOption option2(2, "O2", &handleActions, 1);
ArduRCT_GraphicsUIOption option3(3, "O3", &handleActions, 1);
ArduRCT_GraphicsUIButton button1(4, "Menu", &handleActions);
ArduRCT_GraphicsUIButton button2(5, "Popup", &handleActions);
ArduRCT_GraphicsUIElement timer(TIMER, &drawTimer);
ArduRCT_GraphicsUISlider slider1(7, 50, 0, 100, &handleActions, 2);
ArduRCT_GraphicsUISlider slider2(8, 70, 0, 100, &handleActions, 10);
ArduRCT_GraphicsUITab tab1(TAB1, "Tab 1", &handleActions, 2);
ArduRCT_GraphicsUITab tab2(TAB2, "Tab 2", &handleActions, 2);
ArduRCT_GraphicsUIElement footer(FOOTER, &drawFooter);
ArduRCT_GraphicsUISlider slider3(40, 10, 0, 20, &handleActions, 1);
ArduRCT_GraphicsUIGauge gauge1(41, 10, 0, 20);
ArduRCT_GraphicsUILabel label2(42, "Test", FONT_SMALL);
ArduRCT_GraphicsUILabel label3("Are you really sure you\nwant to exit ?", FONT_MEDIUM);
ArduRCT_GraphicsUIButton button3(50, "Yes", &handleActions);
ArduRCT_GraphicsUIButton button4(51, "No", &handleActions);
ArduRCT_GraphicsUIListItem listItem1(60, "Item 01", &handleActions);
ArduRCT_GraphicsUIListItem listItem2(61, "Item 02", &handleActions);
ArduRCT_GraphicsUIListItem listItem3(62, "Item 03", &handleActions);
ArduRCT_GraphicsUIListItem listItem4(63, "Item 04", &handleActions);

ArduRCT_EventHandler timeHandler(EVENT_TIME_SECOND, &showTime);

bool screenChanged = true;

void setup() {
    Serial.begin(57600);
    // prepare the screen
    Touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    // enable the GraphicsUI
    Touscruino.enableGraphicsUI();

    // add the screens
    // define screen1
    Touscruino.addScreen(&screen1);
    // show a few examples of elements
    screen1.addElement(&label1, 5, 5, GUI_AS, GUI_AS);	
    screen1.addElement(&value1, GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
    screen1.addElement(&option1, 5, GUI_BOPWM, GUI_AS, GUI_AS);
    screen1.addElement(&option2, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    screen1.addElement(&option3, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    screen1.setElementValue(1, GRAPHICS_UI_SELECTED);
    option2.editable = false;
    screen1.addElement(&button1, 5, GUI_BOPWM, GUI_AS, GUI_AS);
    screen1.addElement(&button2, GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
    screen1.addElement(&timer, 5, GUI_BOPWM, 118, GUI_SAP);
    timer.editable = false;
    screen1.addElement(&slider1, GUI_SAP, GUI_BOPWM, 200, 20);
    screen1.addElement(&slider2, GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);
    
    // set up a header with tabs
    screen1.addElement(&tab1, 0, 0, GUI_AS, GUI_AS, GRAPHICS_UI_SECTION_HEADER);
    screen1.setElementValue(TAB1, GRAPHICS_UI_SELECTED);
    screen1.addElement(&tab2, GUI_ROP, 0, GUI_AS, GUI_AS, GRAPHICS_UI_SECTION_HEADER);
    // set a footer
    screen1.addElement(&footer, 0, 0, GUI_W, 16, GRAPHICS_UI_SECTION_FOOTER);
    footer.editable = false;
    
    Touscruino.addScreen(&screen2);
    // reuse the tabs for another screen: screen2
    screen2.addElement(&tab1);
    // reuse the footer for another screen:tab2
    screen2.addElement(&footer);
    // check other UI elements
    screen2.addElement(&slider3, 10, 5, 20, 200);
    screen2.addElement(&gauge1, GUI_ROPWM, GUI_SAP, 35, GUI_SAP);
    screen2.addElement(&label2, GUI_SAP, GUI_BOP, GUI_AS, GUI_AS);
    
    // define a popup
    Touscruino.addScreen(&popup);
    popup.addElement(&label3, 6, 5, GUI_AS, GUI_AS);
    popup.addElement(&button3, 5, GUI_BOPWM, GUI_AS, GUI_AS);
    popup.addElement(&button4, GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
    popup.pack(GRAPHICS_POSITION_CENTER);
    popup.setBackgroundColor(GRAPHICS_UI_COLOR_POPUP_BACKGROUND);

    // define a menu
    Touscruino.addScreen(&menu);
    menu.addElement(&listItem1, 0, 0, GUI_AS, GUI_AS);
    menu.addElement(&listItem2, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    menu.addElement(&listItem3, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    menu.addElement(&listItem4, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    // pack it as small as possible and make the menu popup from the SW of the screen
    menu.pack(GRAPHICS_POSITION_SW);
    
    // start the UI on screen1
    Touscruino.setGraphicsUIScreen(&screen1);
    
    // register a handler for the time
    Touscruino.registerEventHandler(&timeHandler);
}

void loop() {
    Touscruino.manageEvents();
}

// handle the actions created by the UI
bool handleActions(uint8_t elementId, uint8_t state, int16_t value) {
    if (state == GRAPHICS_UI_RELEASED) {
        if (elementId == TAB2) Touscruino.setGraphicsUIScreen(&screen2);
        else if (elementId == TAB1) Touscruino.setGraphicsUIScreen(&screen1);
        else if (elementId == MENU) Touscruino.setGraphicsUIScreen(&menu);
        else if (elementId == POPUP) Touscruino.setGraphicsUIScreen(&popup);
        else if ((elementId >= 50) && (elementId <= 63)) Touscruino.setGraphicsUIScreen(&screen1);
    }
    if (elementId == 40) screen2.setElementValue(41, value);
    screenChanged = true;
    return false;
}

// draw the footer UI element
void drawFooter(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    Touscruino.fillRectangle(x, y, width, height, VLIGHT_GREY);
    Touscruino.drawRectangle(x, y, width, height, GREY, 1);
    Touscruino.setBackgroundColor(VLIGHT_GREY);
    Touscruino.drawString("UITest", x+3, y-4+Touscruino.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
    Touscruino.drawString("v1.0", x+width-27, y-4+Touscruino.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
}

// draw the time in the timer UI element
// a little complex to avoid flickering
void drawTimer(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    char *time = Touscruino.getRTC()->getTimeAsString(RTC_WITH_SECONDS);
    int textX = (width - Touscruino.getStringWidth(time, FONT_MEDIUM))/2;
    Touscruino.setBackgroundColor(YELLOW);
    if ((screenChanged) || (Touscruino.getRTC()->getSecond() == 0)) {
        Touscruino.fillRectangle(x, y, width, height, YELLOW);
        Touscruino.drawRectangle(x, y, width, height, GREY, 1);
        Touscruino.drawString(time, x+textX, y+(height-Touscruino.getFontHeight(FONT_MEDIUM))/2, BLACK, FONT_MEDIUM, FONT_BOLD, false);
        screenChanged = false;
    } else {
        Touscruino.drawString(&time[6], x+textX+6*Touscruino.getFontCharWidth(FONT_MEDIUM), y+(height-Touscruino.getFontHeight(FONT_MEDIUM))/2, BLACK, FONT_MEDIUM, FONT_BOLD, false);
    }
}

int8_t showTime(uint8_t eventType) {
    screen1.setElementValue(TIMER, 0);
    return EVENT_HANDLING_DONE;
}