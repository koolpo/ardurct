#include <NewSoftSerial.h>

#include "ArduRCT_Remote.h"

NewSoftSerial radio(RADIO_RX, RADIO_TX);

int8_t radioFrame = -1 * RADIO_REPEAT_FRAME_START;

void radioSetup() {
    radio.begin(RADIO_BAUDRATE);
}

/*
 * We expect ##aabbccdd
 *    where aa bb cc... are the position in percentage of the servos
 */
void radioProcessReceive() {
    // wait to have something to do
    if (radio.available() > 0) {
        uint8_t val = radio.read();
        // echo the character on the radio and on Serial
        Serial.print((char)val);
        radio.print((char)val);        
        // detect the start of frame
        if (radioFrame < 0) {
            // if right char, we progress in the frame
            if (val == RADIO_FRAME_START) radioFrame ++;
            // else, we start over
            else radioFrame = -1 * RADIO_REPEAT_FRAME_START;
        } else {
            // convert the 2 bytes ascii value into a uint8_t 
            val -= '0';
            if (val > 10) val -= 'A' - '0';
            if ((radioFrame % 2) == 0) servoPosition[radioFrame/2] = val;
            else servoPosition[radioFrame/2] = servoPosition[radioFrame/2] * 16 + val;
            // continue the frame
            radioFrame ++;
        }
        // if the frame is finished, we update the ServoManager
        if (radioFrame/2 == NB_CHANNELS) {
            radioFrame = -1 * RADIO_REPEAT_FRAME_START;
            for (int i=0; i<NB_CHANNELS; i++) ServoManager.mapSet(i, servoPosition[i], 0, 100);
        }
    }
} 

void radioProcessTransmit() {
    // Send all the characters received by Serial to the radio
    while (Serial.available()) radio.print((char)Serial.read());
}
