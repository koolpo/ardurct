/*
 * BmpDisplay - Display a bmp
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
 *	Some ideas from ladyada/adafruit	
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

#include <SdFat.h>
#include <SPI.h>
#include <ArduRCT_Graphics.h>
#include "Bmp.h"

#define SLIDE_SHOW_WAIT 2
#define SD_CS 4
#define MAX_BMP 50

//ArduRCT_S6D04H0 graphics;
//ArduRCT_SPFD5408 graphics;
//ArduRCT_ST7735 graphics;
ArduRCT_ILI9340 graphics;

SdFat sd;
SdFile file;
Bmp bmp;
char bmpNames[MAX_BMP][13];
uint8_t nbBmp = 0;
uint8_t bmpIndex = 0;
boolean rootHasValidBmp = false;

#define dualPrint(a) { Serial.print(a); graphics.print(a); }
#define dualPrintH(a) { Serial.print(a, HEX); graphics.print(a, HEX); }
#define dualPrintln(a) { Serial.println(a); graphics.println(a); }

void setup() {
    Serial.begin(57600);
    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD);
    graphics.setMargin(5);
        
    dualPrint("Initializing SD... ");
    pinMode(SS, OUTPUT);
    pinMode(SD_CS, OUTPUT);
    if (!sd.begin(SD_CS, SPI_FULL_SPEED)) {
        dualPrintln("Failed!");
        return;
    } else dualPrintln("OK!");

    scanRootForBmp();
    if (nbBmp == 0) {
        dualPrintln("Error: no BMP on card.");
    } else {
        Serial.print("Found ");
        Serial.print(nbBmp);
        Serial.println(" BMP on card");
        Serial.println();
    }
}

void loop() {
    if (nbBmp == 0) return;
    
    if (bmpNames[bmpIndex][0]) {
        Serial.print(bmpNames[bmpIndex]);
        Serial.print(": ");
        uint8_t error = bmp.setFileName(bmpNames[bmpIndex]);
        if (error) {
            Serial.println(bmp.getErrorMessage());
            bmpNames[bmpIndex][0] = 0;
        } else {
            if (rootHasValidBmp) graphics.fillScreen(WHITE);
            rootHasValidBmp = true;
            uint32_t timeToDraw = bmp.draw(&graphics);
            Serial.print(timeToDraw); Serial.println("us");
            delay(SLIDE_SHOW_WAIT * 1000);
        }
    }
    bmpIndex ++;
    if (bmpIndex == nbBmp) {
        if (rootHasValidBmp) bmpIndex = 0;
        else nbBmp = 0;
    }
}

void scanRootForBmp() {
    while (file.openNext(sd.vwd(), O_READ)) {
        file.getFilename(bmpNames[nbBmp]);
        Serial.print(bmpNames[nbBmp]);
        Serial.print(": ");
        if (file.isDir()) {
            Serial.println("dir");
            file.close();
            continue;
        }
        int i = 1;
        while (bmpNames[nbBmp][i] != 0) i++;
        if ((bmpNames[nbBmp][i-3] == 'B') && (bmpNames[nbBmp][i-2] == 'M') && (bmpNames[nbBmp][i-1] == 'P')) {
            nbBmp++;
            Serial.println("ok");
        } else {
            Serial.println("not a BMP");
        }
        file.close();
        if (nbBmp == MAX_BMP) return;
    }
}