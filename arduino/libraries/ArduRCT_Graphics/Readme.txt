Notes
-----
1- Review ArduRCT_Graphics_Configuration.h to adapt to your setup
2- In the pypical use below, select the chipset you have

Typical use
-----------

#include <SPI.h>
#include <ArduRCT_Graphics.h>

ArduRCT_XXXXX graphics;

void setup() {
    // start the screen
    graphics.begin(WHITE, BLACK, FONT_MEDIUM, FONT_BOLD);
}

void loop() {
}

