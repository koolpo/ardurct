#include <TouchScreen.h>

TouchScreen touchscreen;
uint8_t rotation = SCREEN_ROTATION_0;

void setup() {
    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    touchscreen.setBacklight(180);
}

void loop() {
    drawScreen();
    delay(5000);
    rotation ++;
    if (rotation > SCREEN_ROTATION_270) rotation = SCREEN_ROTATION_0;
    touchscreen.setRotation(rotation);
}


void drawScreen() {
    uint32_t start = millis();
    uint16_t height = touchscreen.getHeight()/3;
    uint16_t width = touchscreen.getWidth();
    uint16_t *buffer;
    for (uint8_t band=0; band<3; band++) buffer = drawBand(band, width, height);
    for (uint16_t y=3*height; y<touchscreen.getHeight(); y++) touchscreen.pasteBitmap(buffer, 0, y, width, 1);
    touchscreen.setCursor(width/2-40, 3*height/2);
    touchscreen.print(millis()-start);
    touchscreen.print("ms");
}

uint16_t *drawBand(uint8_t band, uint16_t width, uint16_t height) {
    uint16_t buffer[320];
    uint16_t wd2 = width/2;

    for (int16_t y=0; y<height; y++) {
        int32_t Y = (y << 8)/height;
		// for first half of width, we go from black to color
		// along y, we go from full c1 to null c1 and null c2 to full c2
        for (int16_t x=0; x<=wd2; x++) {
            int32_t X = (x << 8)/wd2;
            int32_t c1 = (0xFF * X * (0xFF-Y)) >> 16;
            int32_t c2 = (0xFF * X * Y) >> 16;
            int32_t c3 = 0x00;
            if (band == 0) buffer[x] = COLOR_565(c1, c2, c3);
            else if (band == 1) buffer[x] = COLOR_565(c3, c1, c2);
            else buffer[x] = COLOR_565(c2, c3, c1);
        }
		// for second half of width, we go from color to white
		// along y, we go from full c1 to null c1 and null c2 to full c2
        for (int16_t x=wd2; x<width; x++) {
            int32_t X = ((x - wd2) << 7)/wd2;                        
            int32_t c1 = (0xFF * (0xFF-Y + X)) >> 8;
            if (c1 > 0xFF) c1 = 0xFF;
            int32_t c2 = (0xFF * (Y + X)) >> 8;
            if (c2 > 0xFF) c2 = 0xFF;
            int32_t c3 = (0xFF * X) >> 8;
            if (band == 0) buffer[x] = COLOR_565(c1, c2, c3);
            else if (band == 1) buffer[x] = COLOR_565(c3, c1, c2);
            else buffer[x] = COLOR_565(c2, c3, c1);
        }
        touchscreen.pasteBitmap(buffer, 0, y+height*band, width, 1);
    }
    return buffer;
}
