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
 *  The UI can be navigated through by sending letters through the serial port
 *      m or l      menu or escape
 *      u           up
 *      d           down
 *      e or r      enter
 **/
 
#include <SPI.h>

//#include <ArduRCT_S6D04H0.h>
//ArduRCT_S6D04H0 graphics;

#include <ArduRCT_SPFD5408.h>
ArduRCT_SPFD5408 graphics;

//#include <ArduRCT_ST7735.h>
//ArduRCT_ST7735 graphics;

#define FONT_FACE FONT_PLAIN

#define TAB1 20
#define TAB2 21
#define FOOTER 30
#define MENU 4
#define POPUP 5

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
ArduRCT_GraphicsUIElement element1(6);
ArduRCT_GraphicsUISlider slider1(7, 50, 0, 100, &handleActions, 5);
ArduRCT_GraphicsUISlider slider2(8, 70, 0, 100, &handleActions, 5);
ArduRCT_GraphicsUISlider slider3(9, 10, 0, 100, &handleActions, 5);
ArduRCT_GraphicsUITab tab1(TAB1, "T1", &handleActions, 1);
ArduRCT_GraphicsUITab tab2(TAB2, "T2", &handleActions, 1);
ArduRCT_GraphicsUIElement element2(FOOTER, &drawFooter);
ArduRCT_GraphicsUISlider slider4(40, 10, 0, 20, &handleActions, 1);
ArduRCT_GraphicsUIGauge gauge1(41, 10, 0, 20);
ArduRCT_GraphicsUILabel label2(42, "Test", FONT_SMALL);
ArduRCT_GraphicsUILabel label3("Are you really sure you want to exit ?", FONT_SMALL);
ArduRCT_GraphicsUIButton button3(50, "Yes", &handleActions);
ArduRCT_GraphicsUIButton button4(51, "No", &handleActions);
ArduRCT_GraphicsUIListItem listItem1(60, "Item 01", &handleActions);
ArduRCT_GraphicsUIListItem listItem2(61, "Item 02", &handleActions);
ArduRCT_GraphicsUIListItem listItem3(62, "Item 03", &handleActions);
ArduRCT_GraphicsUIListItem listItem4(63, "Item 04", &handleActions);

void setup() {
    Serial.begin(9600);
    graphics.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);

    // add the screens
    // define screen1
    graphics.addScreen(&screen1);
    // show a few examples of elements
    screen1.addElement(&label1, 5, 5, GUI_AS, GUI_AS);	
    screen1.addElement(&value1, GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
    screen1.addElement(&option1, 5, GUI_BOPWM, GUI_AS, GUI_AS);
    screen1.addElement(&option2, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    screen1.addElement(&option3, GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    screen1.setElementValue(1, GRAPHICS_UI_SELECTED);
    screen1.getElement(2)->editable = false;
    screen1.addElement(&button1, 5, GUI_BOPWM, GUI_AS, GUI_AS);
    screen1.addElement(&button2, GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
    screen1.addElement(&element1, 5, GUI_BOPWM, 118, GUI_SAP);
    screen1.addElement(&slider1, GUI_SAP, GUI_BOPWM, 118, 20);
    screen1.addElement(&slider2, GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);
    screen1.addElement(&slider3, GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);
    // set up a header with tabs
    screen1.addElement(&tab1, 0, 0, GUI_AS, GUI_AS, GRAPHICS_UI_SECTION_HEADER);
    screen1.setElementValue(TAB1, GRAPHICS_UI_SELECTED);
    screen1.addElement(&tab2, GUI_ROP, 0, GUI_AS, GUI_AS, GRAPHICS_UI_SECTION_HEADER);
    // set a footer
    screen1.addElement(&element2, 0, 0, GUI_W, 14, GRAPHICS_UI_SECTION_FOOTER);
    screen1.getElement(FOOTER)->editable = false;
    
    graphics.addScreen(&screen2);
    // reuse the tabs for another screen: screen2
    screen2.addElement(screen1.getElement(TAB1));
    // reuse the footer for another screen:tab2
    screen2.addElement(screen1.getElement(FOOTER));
    // check other UI elements
    screen2.addElement(&slider4, 10, 5, 20, 90);
    screen2.addElement(&gauge1, GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
    screen2.addElement(&label2, GUI_SAP, GUI_BOP, GUI_AS, GUI_AS);
    screen2.getElement(42)->y ++; 
    screen2.getElement(42)->x -= 4;
    
    // define a popup
    graphics.addScreen(&popup);
    popup.addElement(&label3, 6, 5, GUI_AS, GUI_AS);
    popup.addElement(&button3, 5, GUI_BOPWM, GUI_AS, GUI_AS);
    popup.addElement(&button4, GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
    popup.pack(GRAPHICS_POSITION_CENTER);
    popup.setBackgroundColor(GRAPHICS_UI_COLOR_POPUP_BACKGROUND);

    // define a menu
    graphics.addScreen(&menu);
    menu.addElement(&listItem1, 0, 0, GUI_AS, GUI_AS);
    menu.addElement(&listItem2, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    menu.addElement(&listItem3, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    menu.addElement(&listItem4, GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    // pack it as small as possible and make the menu popup from the SW of the screen
    menu.pack(GRAPHICS_POSITION_SW);
    
    graphics.setGraphicsUIHome(&screen1, screen1.getElement(TAB1));
    graphics.setGraphicsUIScreen(&screen1);
}

void loop() {
    delay(5);
    if (Serial.available() == 0) return;
    
    uint8_t value = Serial.read();
    uint8_t actionId = 0;
    switch (value) {
        case 'm': 
        case 'l': 
            actionId = GRAPHICS_UI_ACTION_MENU; 
            break;
        case 'e': 
        case 'r': 
            actionId = GRAPHICS_UI_ACTION_ENTER; 
            break;
        case 'u': 
            actionId = GRAPHICS_UI_ACTION_UP; 
            break;
        case 'd': 
            actionId = GRAPHICS_UI_ACTION_DOWN; 
            break;
    }
    if (actionId != 0) graphics.handleGraphicsUI(actionId, 0, 0);
    if (actionId == GRAPHICS_UI_ACTION_ENTER) graphics.handleGraphicsUI(GRAPHICS_UI_ACTION_RELEASE, 0, 0);
}

bool handleActions(uint8_t elementId, int16_t value) {
    if (elementId == TAB2) graphics.setGraphicsUIScreen(&screen2);
    else if (elementId == TAB1) graphics.setGraphicsUIScreen(&screen1);
    else if (elementId == MENU) graphics.setGraphicsUIScreen(&menu);
    else if (elementId == POPUP) graphics.setGraphicsUIScreen(&popup);
    else if ((elementId >= 50) && (elementId <= 63)) graphics.setGraphicsUIScreen(&screen1);
    else if (elementId == 40) screen2.setElementValue(41, screen2.getElementValue(40));
    return false;
}

void drawFooter(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    graphics.fillRectangle(x, y, width, height, VLIGHT_GREY);
    graphics.drawRectangle(x, y, width, height, GREY, 1);
    graphics.setBackgroundColor(VLIGHT_GREY);
    graphics.drawString("UITest", x+3, y-4+graphics.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
    graphics.drawString("v1.0", x+width-27, y-4+graphics.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
}