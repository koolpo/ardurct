#include <TouchScreen.h>

TouchScreen touchscreen;

void setup() {
    touchscreen.begin(BLACK, WHITE);
    touchscreen.setBacklight(180);

    uint16_t height = touchscreen.getHeight()/3;
	uint16_t width = touchscreen.getWidth();
    uint16_t *buffer;
    for (uint8_t band=0; band<3; band++) buffer = drawBand(band, width, height);
    for (uint16_t y=3*height; y<touchscreen.getHeight(); y++) touchscreen.pasteBitmap(buffer, 0, y, width, 1);
}

uint16_t *drawBand(uint8_t band, uint16_t width, uint16_t height) {
    uint16_t buffer[320];
    float fh = height;
    float fw = width;

    for (uint16_t y=0; y<height; y++) {
        float fy = y;
        for (uint16_t x=0; x<=width/2; x++) {
            float fx = x * 2;
            uint16_t r = 0xFF * (fx/fw * (1-fy/fh));
            uint16_t g = 0xFF * (fx/fw * fy/fh);
            uint16_t b = 0x00;
            if (band == 0) buffer[x] = COLOR_565(r, g, b);
            else if (band == 1) buffer[x] = COLOR_565(b, r, g);
            else buffer[x] = COLOR_565(g, b, r);
        }
        for (uint16_t x=width/2; x<width; x++) {
            float fx = ((float)x - fw/2) * 2;                        
            uint16_t r = 0xFF * (1 - fy/fh + fx/fw);
            if (r > 0xFF) r = 0xFF;
            uint16_t g = 0xFF * (fy/fh + fx/fw);
            if (g > 0xFF) g = 0xFF;
            uint16_t b = 0xFF * (fx/fw);
            if (band == 0) buffer[x] = COLOR_565(r, g, b);
            else if (band == 1) buffer[x] = COLOR_565(b, r, g);
            else buffer[x] = COLOR_565(g, b, r);
        }
        touchscreen.pasteBitmap(buffer, 0, y+height*band, width, 1);
    }
	return buffer;
}

void loop() {
    delay(100);
}