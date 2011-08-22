#include <NewSoftSerial.h>

#include "ArduRCT_Local.h"

NewSoftSerial radio(RADIO_RX, RADIO_TX);

void radioSetup() {
    radio.begin(RADIO_BAUDRATE);
}

void radioTransmitChannels() {
    for (uint8_t i=0; i<RADIO_REPEAT_FRAME_START; i++) radio.print(RADIO_REPEAT_FRAME_START);
    for (uint8_t i=0; i<NB_CHANNELS; i++) {
        if (channel[i] < 16) radio.print("0");
        radio.print(channel[i], HEX);
    }
    radio.println();
}

void radioProcessReceive() {
    // pass through to the USB port
    while (radio.available()) Serial.print((char)radio.read());
}
