#ifndef RADIO_H
#define RADIO_H

/*
 * We expect to intercept X*Y.... where
 *    X is RADIO_REPEAT_FRAME_START repeats of RADIO_FRAME_START
 *    Y is one of the following
 */
#define RADIO_IDLE 0
#define RADIO_COMMAND 1
#define RADIO_SERVO 'S'
#define RADIO_ID_PROPOSE 'i'
#define RADIO_ID_VERIFY 'j'
#define RADIO_ID_CHANGE 'I'
#define RADIO_MESURE 'M'
#define RADIO_GET_GPS 'G'
#define RADIO_OUTPUT 'O'

#endif