#include <NewSoftSerial.h>

#include "ArduRCT_Local.h"

NewSoftSerial radio(RADIO_TX, RADIO_RX);

uint8_t radioLastSentChannel[NB_CHANNELS];

void radioSetup() {
    radio.begin(RADIO_BAUDRATE);
}

void radioProcessReceive() {
    // pass through to the USB port
    while (radio.available()) Serial.print(radio.read());
}

void radioTransmitChannels() {
    // check if we have to send the channels
    uint8_t i;
    for (i=0; i<NB_CHANNELS; i++) {
        if (channel[i] != radioLastSentChannel[i]) break;
    }
    // if we reach the end of the loop, we can return, no channel have changed
    if (i==NB_CHANNELS) return;
    // send the frame start and the channels
    for (uint8_t i=0; i<RADIO_REPEAT_FRAME_START; i++) radio.print(RADIO_FRAME_START);
    radio.print("S");
    for (uint8_t i=0; i<NB_CHANNELS; i++) {
        if (channel[i] < 16) radio.print("0");
        radio.print(channel[i], HEX);
        radioLastSentChannel[i] = channel[i];
    }
}

