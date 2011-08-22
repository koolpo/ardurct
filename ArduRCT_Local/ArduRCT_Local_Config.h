#ifndef ARDURCT_LOCAL_CONFIG_H
#define ARDURCT_LOCAL_CONFIG_H

/* --- Common to Local and Remote --- */

#define NB_CHANNELS 8

#define RADIO_FRAME_START '#'
#define RADIO_REPEAT_FRAME_START 2
#define RADIO_BAUDRATE 19200

/* ---------------------------------- */

// Serial
#define SERIAL_BAUDRATE 57600

// Backlight
#define LCD_BACKLIGHT_PIN 10

// Displays
#define LCD_SCLK_PIN 5
#define LCD_SDIN_PIN 6
#define LCD_DC_PIN 13
#define LCD_RESET_PIN 9
#define LCD_SCE_GRAPHIC_PIN 11
#define LCD_SCE_TEXT_PIN 12

// Joysticks and potentiometers
#define JOYSTICK_THROTTLE_PIN 2
#define JOYSTICK_YAW_PIN 3
#define JOYSTICK_PITCH_PIN 0
#define JOYSTICK_ROLL_PIN 1

#define ADJUST_PIN 6

#define BATTERY_PIN 7

// Buttons
#define DOWN_PIN 2
#define UP_PIN 3
#define ENTER_PIN 4

// Radio
#define RADIO_RX 7
#define RADIO_TX 8

#endif
