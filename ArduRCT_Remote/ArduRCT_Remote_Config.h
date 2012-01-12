#ifndef ARDURCT_REMOTE_CONFIG_H
#define ARDURCT_REMOTE_CONFIG_H

/* --- Common to Local and Remote --- */

#define NB_CHANNELS 10

#define RADIO_FRAME_START '#'
#define RADIO_REPEAT_FRAME_START 2

#define XBEE_BAUDRATE 19200

/* ---------------------------------- */

// Serial
#define SERIAL_BAUDRATE 57600

// Servos
#define FIRST_SERVO 7
extern Servo servo[NB_CHANNELS];

#endif
