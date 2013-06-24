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

#include <ArduRCT_S6D04H0.h>
ArduRCT_S6D04H0 graphics(21, 0xFF, 0xFF, 5);   // graphics(CD, CS, RESET, BACKLIGHT)

//#include <ArduRCT_ST7735.h>
//ArduRCT_ST7735 graphics(10, 9 , 8, 5);         // graphics(CD, CS, RESET, BACKLIGHT)

ArduRCT_GraphicsUIScreen tab1(&graphics);
ArduRCT_GraphicsUIScreen tab2(&graphics);
ArduRCT_GraphicsUIScreen menu(&graphics);
ArduRCT_GraphicsUIScreen popup(&graphics);

#define TAB1 20
#define TAB2 21
#define FOOTER 30
#define MENU 4
#define POPUP 5

void setup() {
    Serial.begin(9600);
    graphics.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);

    // define the tab1 screen
    // show a few examples of elements
    tab1.addElement(new ArduRCT_GraphicsUILabel("Label"), 5, 5, GUI_AS, GUI_AS);	
    tab1.addElement(new ArduRCT_GraphicsUIValue(0, 3, 0, 255, &handleActions, 1, 16), GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
    tab1.addElement(new ArduRCT_GraphicsUIOption(1, "O1", &handleActions, 1), 5, GUI_BOPWM, GUI_AS, GUI_AS);
    tab1.addElement(new ArduRCT_GraphicsUIOption(2, "O2", &handleActions, 1), GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    tab1.addElement(new ArduRCT_GraphicsUIOption(3, "O3", &handleActions, 1), GUI_ROP, GUI_SAP, GUI_SAP, GUI_SAP);
    tab1.setElementValue(1, GRAPHICS_UI_SELECTED);
    tab1.getElement(2)->editable = false;
    tab1.addElement(new ArduRCT_GraphicsUIButton(4, "Menu", &handleActions), 5, GUI_BOPWM, GUI_AS, GUI_AS);
    tab1.addElement(new ArduRCT_GraphicsUIButton(5, "Popup", &handleActions), GUI_ROPWM, GUI_SAP, GUI_AS, GUI_AS);
    tab1.addElement(new ArduRCT_GraphicsUIElement(6), 5, GUI_BOPWM, 118, GUI_SAP);
    tab1.addElement(new ArduRCT_GraphicsUISlider(7, 50, 0, 100, &handleActions, 5), GUI_SAP, GUI_BOPWM, 118, 20);
    tab1.addElement(new ArduRCT_GraphicsUISlider(8, 70, 0, 100, &handleActions, 5), GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);
    tab1.addElement(new ArduRCT_GraphicsUISlider(9, 10, 0, 100, &handleActions, 5), GUI_SAP, GUI_BOPWM, GUI_SAP, GUI_SAP);
    // set up a header with tabs
    tab1.addElement(new ArduRCT_GraphicsUITab(TAB1, "T1", &handleActions, 1), 0, 0, GUI_AS, GUI_AS, GRAPHICS_UI_SECTION_HEADER);
    tab1.setElementValue(TAB1, GRAPHICS_UI_SELECTED);
    tab1.addElement(new ArduRCT_GraphicsUITab(TAB2, "T2", &handleActions, 1), GUI_ROP, 0, GUI_AS, GUI_AS, GRAPHICS_UI_SECTION_HEADER);
    // set a footer
    tab1.addElement(new ArduRCT_GraphicsUIElement(30, drawFooter), 0, 0, GUI_W, 14, GRAPHICS_UI_SECTION_FOOTER);
    tab1.getElement(FOOTER)->editable = false;
    
    // reuse the tabs for another screen: tab2
    tab2.addElement(tab1.getElement(TAB1));
    // reuse the footer for another screen:tab2
    tab2.addElement(tab1.getElement(FOOTER));
    // check other UI elements
    tab2.addElement(new ArduRCT_GraphicsUISlider(40, 10, 0, 20, &handleActions, 1), 10, 5, 20, 90);
    tab2.addElement(new ArduRCT_GraphicsUIGauge(41, tab2.getElementValue(105), 0, 20), GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
    tab2.addElement(new ArduRCT_GraphicsUILabel(42, "Test", FONT_SMALL), GUI_SAP, GUI_BOP, GUI_AS, GUI_AS);
    tab2.getElement(42)->y ++; 
    tab2.getElement(42)->x -= 4;
    
    // define a popup
    popup.addElement(new ArduRCT_GraphicsUILabel("Are you really sureyou want to exit ?", FONT_SMALL), 6, 5, GUI_AS, GUI_AS);
    popup.addElement(new ArduRCT_GraphicsUIButton(50, "Yes", &handleActions), 5, GUI_BOPWM, GUI_AS, GUI_AS);
    popup.addElement(new ArduRCT_GraphicsUIButton(51, "No", &handleActions), GUI_ROPWM, GUI_SAP, GUI_SAP, GUI_SAP);
    popup.pack(GRAPHICS_POSITION_CENTER);
    popup.setBackgroundColor(GRAPHICS_UI_COLOR_POPUP_BACKGROUND);

    // define a menu
    menu.addElement(new ArduRCT_GraphicsUIListItem(60, "Item 01", &handleActions), 0, 0, GUI_AS, GUI_AS);
    menu.addElement(new ArduRCT_GraphicsUIListItem(61, "Item 02", &handleActions), GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    menu.addElement(new ArduRCT_GraphicsUIListItem(62, "Item 03", &handleActions), GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    menu.addElement(new ArduRCT_GraphicsUIListItem(63, "Item 04", &handleActions), GUI_SAP, GUI_BOP, GUI_SAP, GUI_SAP);
    // pack it as small as possible and make the menu popup from the SW of the screen
    menu.pack(GRAPHICS_POSITION_SW);
    
    graphics.setupGraphicsUIHome(&tab1, tab1.getElement(20));
    graphics.setGraphicsUIScreen(&tab1);
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
    if (elementId == TAB2) graphics.setGraphicsUIScreen(&tab2);
    else if (elementId == TAB1) graphics.setGraphicsUIScreen(&tab1);
    else if (elementId == MENU) graphics.setGraphicsUIScreen(&menu);
    else if (elementId == POPUP) graphics.setGraphicsUIScreen(&popup);
    else if ((elementId >= 50) && (elementId <= 63)) graphics.setGraphicsUIScreen(&tab1);
    else if (elementId == 40) tab2.setElementValue(41, tab2.getElementValue(40));
    return false;
}

void drawFooter(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    graphics.fillRectangle(x, y, width, height, VLIGHT_GREY);
    graphics.drawRectangle(x, y, width, height, GREY, 1);
    graphics.setBackgroundColor(VLIGHT_GREY);
    graphics.drawString("UITest", x+3, y-4+graphics.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
    graphics.drawString("v1.0", x+width-27, y-4+graphics.getFontHeight(FONT_SMALL), BLACK, FONT_SMALL, false, false);
}