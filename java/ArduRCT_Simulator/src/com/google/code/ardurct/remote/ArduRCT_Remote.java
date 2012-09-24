package com.google.code.ardurct.remote;


public class ArduRCT_Remote extends GPSFrameParser {

	static final int MAX_SERIAL_FRAME_LENGTH = 128;
	
	static final int MAIN_LOOP_REPEATS_BEFORE_RESTART = 500;	// 10 seconds MAIN_LOOP_REPEATS_BEFORE_RESTART/(1/MAIN_LOOP_DELAY) 
	
	static final int BAUDRATES[] = { 9600, 19200, 38400, 57600 };
	
	int serial1BaudrateIndex;
	int loopCounter;
	int[] serialFrame = new int[MAX_SERIAL_FRAME_LENGTH];
	int serialFramePtr;
	float initialAltitude;
	
	public void setup() {
		serial1BaudrateIndex = 0;
		battery = 255;
		loopCounter = 0;
	    radioSetup(BAUDRATES[serial1BaudrateIndex]);
	    monitorSetup();
	    serialFramePtr = 0;
	}

	public void loop() {
		delay(MAIN_LOOP_DELAY);
		radioProcess();
		handleRadioResponse();
		serialProcess();
		
		loopCounter ++;
		if (loopCounter == MAIN_LOOP_REPEATS_BEFORE_RESTART) loopCounter = 0;
		
		if ((loopCounter % (1000/MAIN_LOOP_DELAY)) == 0) sendTelemetry();
		if (loopCounter == 0) {
			if (enginesAreRunning) {
				battery --;
				if (battery == 0) battery = 255;
			}
		}
	}

	public void handleRadioResponse() {
		switch (radioStatus) {
			case RADIO_STATUS_BAUDRATE_ERROR:
				serial1BaudrateIndex ++;
				if (serial1BaudrateIndex == 4) {
					radioStatus = RADIO_STATUS_ERROR;
					Serial.print("$E: No XBee connected\n");
				}
				else radioSetup(BAUDRATES[serial1BaudrateIndex]);
				break;
			case RADIO_STATUS_FINALIZING:
				radioStatus = RADIO_STATUS_READY;
				break;
		}
	}
	
	public void serialProcess() {
		if (Serial.available() <= 0) return;
		while (Serial.available() > 0) {
			serialFrame[serialFramePtr++] = Serial.read();
			if (serialFrame[serialFramePtr-1] == '\n') break;
		}
		if (serialFrame[serialFramePtr-1] == '\n') serialProcessFrame();
	}

	public void serialProcessFrame() {
		if ((serialFramePtr > 2) && (serialFrame[0] == '$')) {
			boolean absorbFrame = true;
			// we are receiving commands
			switch (serialFrame[1]) {
				case 'G':	// GPS
					if (sendGPSFrames) absorbFrame = false;
					gpsParseData(serialFrame);
					break;
				default:
					monitorProcessFrame(serialFrame, true);
					break;
			}
			if (absorbFrame) serialFramePtr = 0;
		}
		int i = 0;
		while (i < serialFramePtr) xBee.print((char)serialFrame[i++]);
		serialFramePtr = 0;
	}
	
	public void sendTelemetry() {
		if (!sendTelemetryFrames || !gpsHasChanged) return;
		
	}
}
