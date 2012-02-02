#ifndef MACRO_DRAWING_HPP
#define MACRO_DRAWING_HPP 1

#include <inttypes.h>

// the number of macros defined in eeprom
// each compressed macro can be up to SCREEN_MACRO_MAX_SIZE character long
// the eeprom taken will be SCREEN_MACRO_MAX_NUMBER * SCREEN_MACRO_MAX_SIZE bytes
#define SCREEN_MACRO_MAX_NUMBER 16
// this number can not be bigger than 255
#define SCREEN_MACRO_MAX_SIZE 128

#define SCREEN_MACRO_CMD_NONE						0x00
#define SCREEN_MACRO_CMD_PRESET_FOREGROUND			0x11
#define SCREEN_MACRO_CMD_PRESET_BACKGROUND			0x12
#define SCREEN_MACRO_CMD_PRESET_FONT				0x13
#define SCREEN_MACRO_CMD_PRESET_THICKNESS			0x14
#define SCREEN_MACRO_CMD_PRESET_SCALE				0x15
#define SCREEN_MACRO_CMD_PRESET_ERASE				0x16
#define SCREEN_MACRO_CMD_LINE 						0x20
#define SCREEN_MACRO_CMD_ARC 						0x30
#define SCREEN_MACRO_CMD_ARC_FILLED 				0x31
#define SCREEN_MACRO_CMD_ARC_REVERSED				0x32
#define SCREEN_MACRO_CMD_ARC_FILLED_REVERSED		0x33
#define SCREEN_MACRO_CMD_RECTANGLE					0x40
#define SCREEN_MACRO_CMD_RECTANGLE_FILLED			0x41
#define SCREEN_MACRO_CMD_RECTANGLE_ROUNDED			0x42
#define SCREEN_MACRO_CMD_RECTANGLE_ROUNDED_FILLED	0x43
#define SCREEN_MACRO_CMD_TRIANGLE					0x50
#define SCREEN_MACRO_CMD_TRIANGLE_FILLED			0x51
#define SCREEN_MACRO_CMD_CIRCLE						0x60
#define SCREEN_MACRO_CMD_CIRCLE_FILLED				0x61
#define SCREEN_MACRO_CMD_STRING						0x70
#define SCREEN_MACRO_CMD_WRITE						0xE0
#define SCREEN_MACRO_CMD_EXECUTE					0xF0
#define SCREEN_MACRO_CMD_EXECUTE_WITH_RESET			0xF1

typedef struct {
	uint8_t cmd;
	uint8_t nbParams;
	int16_t param[6];
	uint16_t color;
	uint8_t *text;
	uint8_t textLen;
} macroCommand_t;

#endif