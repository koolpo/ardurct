/*
 * BmpDisplay - Display a bmp
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
 *    Some ideas from ladyada/adafruit    
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

#include <SD.h>

// Change the version to adapt to your screen
#define TOUSCRUINO_VERSION 1

#if (TOUSCRUINO_VERSION == 1)

#define TFT_CS 10
#define TFT_CD 9
#define TFT_RST 8

#include <SPI.h>
#include <ArduRCT_TouScruinoV1.h>

ArduRCT_TouScruinoV1 tft(TFT_CD, TFT_CS, TFT_RST);

#elif (TOUSCRUINO_VERSION == 2)

#define TFT_PORT 2 // PortB
#define TFT_CD     21
#define TFT_WR     22
#define TFT_RD     23
#define TFT_CS     0xFF
#define TFT_RST 0xFF
#define TFT_BL 5

#include <ArduRCT_TouScruinoV2.h>

ArduRCT_TouScruinoV2 tft(TFT_PORT, TFT_CD, TFT_WR, TFT_RD, TFT_CS, TFT_RST, TFT_BL);

#endif

#define FILE_NAME "woof.bmp"
#define MAX_BMP_WIDTH 320
#define SD_CS 4

#define dualPrint(a) { Serial.print(a); tft.print(a); }
#define dualPrintH(a) { Serial.print(a, HEX); tft.print(a, HEX); }
#define dualPrintln(a) { Serial.println(a); tft.println(a); }


void setup() {
    Serial.begin(57600);
    tft.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    tft.setMargin(5);
    tft.setBacklight(180);

    dualPrint("Initializing SD... ");
    pinMode(SS, OUTPUT);

    if (!SD.begin(SD_CS)) {
        dualPrintln("Failed!");
        while (1);
    } else dualPrintln("OK!");

    File bmpFile = SD.open(FILE_NAME);
    if (!bmpFile) {
        dualPrintln("Can not find image.");
        while (1);
    }
    uint16_t width, height;  
    if (!bmpReadHeader(bmpFile, &width, &height)) { 
        bmpFile.close();
        while (1);
    }  
    bmpDraw(bmpFile, 0, 0, width, height);
    bmpFile.close();

    delay(1000);
}


void loop() {
  delay(1000);
}

/**
 *	If the image is too big, will take the center of it for display.
 *	If too small, will center it from x and y
 **/
void bmpDraw(File f, int16_t x, int16_t y, uint16_t bmpWidth, uint16_t bmpHeight) {
    uint32_t time = millis();
  
    uint8_t bmpBuffer[3 * MAX_BMP_WIDTH];  // 3 * pixels to buffer
    uint16_t imgBuffer[MAX_BMP_WIDTH];
    
    uint16_t width = tft.getWidth();
    uint16_t height = tft.getHeight();
    uint16_t drawWidth = min(width-x, bmpWidth);
    uint16_t drawHeight = min(height-y, bmpHeight);
    int16_t offsetX = min(0, (drawWidth-bmpWidth)/2);
    int16_t offsetY = min(0, (drawHeight-bmpHeight)/2);

    for (uint16_t i=0; i<bmpHeight; i++) {
        // read one line
        f.read(bmpBuffer, 3*bmpWidth);
        // only draw if we are in the tft boundaries
        if (i < offsetY+y) continue;
        // finish if we are outside the tft boundaries
        if (i+offsetY > drawHeight) break;
        
        uint16_t ptr = (offsetX < 0 ? -offsetX : 0) * 3;
        for (uint16_t j=0; j<drawWidth; j++) {
            // convert pixel from 888 to 565
            uint8_t b = bmpBuffer[ptr++];     // blue
            uint8_t g = bmpBuffer[ptr++];     // green
            uint16_t color = bmpBuffer[ptr++]; // red
            color >>= 3;
            color <<= 6;
            g >>= 2;
            color |= g;
            color <<= 5;
            b >>= 3;
            color |= b;
            imgBuffer[j] = color;
        }
        // bitmaps are stored with the BOTTOM line first so we have to start from the bottom
        tft.pasteBitmap(imgBuffer, offsetX < 0 ? x : x+offsetX, y+offsetY + drawHeight-1 - i, drawWidth, 1);
    }
    tft.setCursor(0, 0);
    dualPrint(millis() - time);
    dualPrintln("ms");
}

boolean bmpReadHeader(File f, uint16_t *width, uint16_t *height) {
    uint32_t tmp = readLE16b(f);
    if (tmp != 0x4D42) {
        dualPrint("Start: "); dualPrintH(tmp);
        dualPrintln(" => Not supported.");
        // magic bytes missing
        return false;
    }
    // read file size
    tmp = readLE32b(f);  
    dualPrint("File size: "); dualPrintln(tmp);
    // read and ignore creator bytes
    readLE32b(f);
    // read offset
    uint32_t bmpImageOffset = readLE32b(f);  
    // read DIB header
    tmp = readLE32b(f);
    *width = readLE32b(f);
    *height = readLE32b(f);
    dualPrint("Image size: "); 
    dualPrint(*width);
    dualPrint("x");
    dualPrintln(*height); 
    
    if (readLE16b(f) != 1)  return false;
    
    // read bit depth
    uint16_t bmpDepth = readLE16b(f);
    dualPrint("Bit depth: "); dualPrint(bmpDepth);
    if (bmpDepth != 24) {
        dualPrintln(" => Not supported.");
        return false;
    } else dualPrintln("");
    
    // read compression
    uint32_t compression = readLE32b(f);
    dualPrint("Compression: "); dualPrint(compression);
    if (compression != 0) {
        dualPrintln(" => Not supported.");
        return false;
    } else dualPrintln("");

    f.seek(bmpImageOffset);
    return true;
}


// Read 16b as Little Endian
uint16_t readLE16b(File f) {
    uint8_t l = f.read();
    uint16_t h = f.read();
    h <<= 8;
    h |= l;
    return h;
}


// Read 32b as Little Endian
uint32_t readLE32b(File f) {
    uint16_t l = readLE16b(f);
    uint32_t h = readLE16b(f);
    h <<= 16;
    h |= l;
    return h;
}

