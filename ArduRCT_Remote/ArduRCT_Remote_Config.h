#ifndef ARDURCT_REMOTE_CONFIG_H
#define ARDURCT_REMOTE_CONFIG_H

#include "XBee_Config.h"

/* --- Common to Local and Remote --- */

#define NB_CHANNELS 10

#define RADIO_FRAME_START '#'
#define RADIO_REPEAT_FRAME_START 2

#define XBEE_BAUDRATE 57600

/* ---------------------------------- */

// Serial
#define SERIAL_BAUDRATE 57600

// Servos
#define FIRST_SERVO 4

// Radio
#define XBEE_RX 3
#define XBEE_TX 2
#define XBEE_GUARD_TIME 50


#endif
