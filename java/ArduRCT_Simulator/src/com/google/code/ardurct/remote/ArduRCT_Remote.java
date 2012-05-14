package com.google.code.ardurct.remote;


public class ArduRCT_Remote extends Radio {

	static final int BAUDRATES[] = { 9600, 19200, 38400, 57600 };

	int serial1BaudrateIndex;
	
	public void setup() {
		serial1BaudrateIndex = 0;
	    radioSetup(BAUDRATES[serial1BaudrateIndex]);
	}

	public void loop() {
		radioProcess();
		handleRadioResponse();
	}

	public void handleRadioResponse() {
		switch (radioStatus) {
			case RADIO_STATUS_BAUDRATE_ERROR:
				serial1BaudrateIndex ++;
				if (serial1BaudrateIndex == 4) {
					radioStatus = RADIO_STATUS_ERROR;
					Serial.print("Error: could not communicate with XBEE at any baudrate");
				} else {
					radioSetup(BAUDRATES[serial1BaudrateIndex]);
					Serial.print("Trying ");
					Serial.print(BAUDRATES[serial1BaudrateIndex]);
					Serial.print("\n");
				}
				break;
			case RADIO_STATUS_FINALIZING:
				Serial.println("Ready");
				radioStatus = RADIO_STATUS_READY;
				break;
		}

	}

}
