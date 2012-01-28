#include <TouchScreen.h>
#include <SD.h>

TouchScreen touchscreen;

#define FILE_NAME "woof.bmp"
#define MAX_BMP_WIDTH 320
#define SD_CS 4

#define dualPrint(a) { Serial.print(a); touchscreen.print(a); }
#define dualPrintH(a) { Serial.print(a, HEX); touchscreen.print(a, HEX); }
#define dualPrintln(a) { Serial.println(a); touchscreen.println(a); }


void setup() {
    Serial.begin(57600);
    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    touchscreen.setMargin(5);
    touchscreen.setBacklight(180);

    dualPrint("Initializing SD... ");
    pinMode(SS, OUTPUT);

    if (!SD.begin(SD_CS)) {
        dualPrintln("Failed!");
        while (1);
    } else dualPrintln("OK!");

    uint16_t width, height;  
    File bmpFile = SD.open(FILE_NAME);
    if (!bmpFile) {
        dualPrintln("Can not find image.");
        while (1);
    }
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


void bmpDraw(File f, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    uint32_t time = millis();
  
    uint8_t bmpBuffer[3 * MAX_BMP_WIDTH];  // 3 * pixels to buffer
    uint16_t imgBuffer[MAX_BMP_WIDTH];
    
    for (uint16_t i=0; i<height; i++) {
        // read one line
        f.read(bmpBuffer, 3*width);    
        uint16_t ptr = 0;
        for (uint16_t j=0; j<width; j++) {
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
        touchscreen.pasteBitmap(imgBuffer, x, y + height-1 - i, width, 1);
    }
	touchscreen.setCursor(0, 0);
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

