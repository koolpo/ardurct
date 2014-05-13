/*
 * Bmp - Class to draw a *.bmp Bitmap
 *
 * Copyright (c) 2010-2014 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
#ifndef _BMP_H
#define _BMP_H

#include <ArduRCT_Graphics.h>
#include <sdFat.h>

// what is the maximum width an image can have
#define MAX_BMP_WIDTH 320
// how many lines are read at one time from the SdCard
// the higher the value, the faster the display and the more ram used...
#define LINES_TO_READ 16
// size of the uint16_t buffer used to read the values: max 3 bytes per pixel
#define IMG_BUFFER_SIZE (MAX_BMP_WIDTH * LINES_TO_READ * 3 / 2)

class Bmp {

    public:
        Bmp();
        
        uint8_t setFileName(char *fileName);
        
        char *getFileName() { return _fileName; };
        
        uint32_t draw(ArduRCT_Graphics *graphics, int16_t x = -10000, int16_t y = -10000);
        
        uint16_t getWidth() { return _width; };

        uint16_t getHeight() { return _width; };

        uint16_t getCompression() { return _compression; };

        uint8_t getBitsPerPixel() { return _bitsPerPixel; };
        
        char *getErrorMessage();

    private:
        uint8_t _error;
        uint32_t _imageOffset;
        int16_t _width;
        int16_t _height;
        uint16_t _bitsPerPixel;
        uint32_t _compression;
        char *_fileName;
        SdFile _file;
        
        static uint16_t _imgBuffer[IMG_BUFFER_SIZE];
    
        uint16_t _readImageData(uint16_t pixelsToRead, uint32_t *pixelsNotRead);
        uint16_t _readLE16b();
        uint32_t _readLE32b();
};

#endif