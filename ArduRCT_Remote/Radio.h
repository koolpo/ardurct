#ifndef RADIO_H
#define RADIO_H

/*
 * We expect to intercept X*Y.... where
 *    X is RADIO_REPEAT_FRAME_START repeats of RADIO_FRAME_START
 *    Y is one of the following
 */
#define RADIO_IDLE 0
#define RADIO_IN_FRAME 1
#define RADIO_COMMAND 0xFF

#define RADIO_SERVO 'S'
#define RADIO_MESURE 'M'
#define RADIO_GET_GPS 'G'
#define RADIO_OUTPUT 'O'
#define RADIO_DB 'D'

#endif