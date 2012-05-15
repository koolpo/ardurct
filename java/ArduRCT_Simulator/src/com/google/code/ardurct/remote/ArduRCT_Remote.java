package com.google.code.ardurct.remote;


public class ArduRCT_Remote extends Radio {

	static final int BAUDRATES[] = { 9600, 19200, 38400, 57600 };

	int serial1BaudrateIndex;
	int loopCounter;
	
	public void setup() {
		serial1BaudrateIndex = 0;
		battery = 255;
		loopCounter = 0;
	    radioSetup(BAUDRATES[serial1BaudrateIndex]);
	}

	public void loop() {
		delay(MAIN_LOOP_DELAY);
		radioProcess();
		handleRadioResponse();
		serialProcess();
		
		switch (loopCounter) {
			case 0: 
				battery --;
				if (battery == 0) battery = 255;
				break;
		}
		loopCounter ++;
		if (loopCounter == 10) loopCounter = 0; 
	}

	public void handleRadioResponse() {
		switch (radioStatus) {
			case RADIO_STATUS_BAUDRATE_ERROR:
				serial1BaudrateIndex ++;
				if (serial1BaudrateIndex == 4) radioStatus = RADIO_STATUS_ERROR;
				else radioSetup(BAUDRATES[serial1BaudrateIndex]);
				break;
			case RADIO_STATUS_FINALIZING:
				radioStatus = RADIO_STATUS_READY;
				break;
		}
	}
	
	public void serialProcess() {
		while (Serial.available() > 0) xBee.print((char) Serial.read());
	}

}
