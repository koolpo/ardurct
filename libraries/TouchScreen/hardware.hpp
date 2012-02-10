#ifndef HARDWARE_HPP
#define HARDWARE_HPP

// choose one of the driver chips for your screen
#define HARDWARE_S6D04H0 1
//#define HARDWARE_ILI932X 1

#define HARDWARE_WIDTH 	240
#define HARDWARE_HEIGHT 320

#define HARDWARE_PORT 	PB
#define HARDWARE_CD 	21
#define HARDWARE_WR 	22
#define HARDWARE_RD 	23
#define HARDWARE_CS 	0xFF
#define HARDWARE_RESET 	0xFF

#define HARDWARE_BACKLIGHT 5

#define HARDWARE_XM	A6
#define HARDWARE_XP	15
#define HARDWARE_YM	14
#define HARDWARE_YP	A7

#define HARDWARE_PRESSURE_THRESHOLD 10
#define HARDWARE_PRESSURE_MAX 1000

#define HARDWARE_DISTANCE_EQUAL 20

#define HARDWARE_X_PLANE_RESISTANCE 300

#define HARDWARE_BUS_IS_SHARED_WITH_SPI 1

// Max space that a pattern or a bitmap that needs to be overlaid can occupy
// minimum is FONT_MAX_SPACE (from font.hpp)
// RAM taken can be up to 5 times this, so beware! 
#define HARDWARE_MAX_BITMAP_SPACE (32*32)		

#endif