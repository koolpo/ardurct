package com.google.code.ardurct.remote;

import com.google.code.ardurct.libraries.Servo;
import com.google.code.ardurct.libraries.XBee;
import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;


public class Monitor extends ArduinoFirmware {

	static final int MAX_SERVOS = 8;
	static final int FIRST_SERVO = 7;

	public XBee xBee = new XBee();
	Servo servo[] = new Servo[MAX_SERVOS];

	int battery;
	boolean enginesAreRunning;
	boolean sendGPSFrames;
	boolean sendTelemetryFrames;
	int nbServos;
	int firstServo;
	
	public void monitorSetup() {
		sendGPSFrames = false;
		sendTelemetryFrames = false;
	    enginesAreRunning = false;
	    nbServos = 0;
	    firstServo = 0;
	    for (int i=0; i<MAX_SERVOS; i++) {
	    	servo[i] = new Servo();	// line not required in Arduino
	    	servo[i].attach(FIRST_SERVO + i);
	    }
	}
	
	public void monitorProcessFrame(int[] frame, boolean isSerial) {
		int number, value;
		int i = 0;
		switch (frame[1]) {
			case 'B':	// battery
				monitorPrint("$B", isSerial);
				value = (battery >> 4) + '0';
				monitorPrint(value > '9' ? value + 'A' - '0' - 10 : value, isSerial);
				value = (battery & 0x0F) + '0';					
				monitorPrint(value > '9' ? value + 'A' - '0' - 10 : value, isSerial);
				monitorPrint('\n', isSerial);
				break;
			case 'M':	// motors or engines
				if (frame[2] == 'R') {
					enginesAreRunning = true;
					monitorPrint("$MR\n", isSerial);
				} else {
					enginesAreRunning = false;
					monitorPrint("$MS\n", isSerial);
				}
				break;
			case 'T':
				if (frame[2] == 'G') {				// gps telemetry
					if (frame[3] == 'R') {			// run
						sendGPSFrames = true;
						monitorPrint("$TGR\n", isSerial);						
					} else if (frame[3] == 'S') {	// stop
						sendGPSFrames = false;
						monitorPrint("$TGS\n", isSerial);						
					}
				} else if (frame[2] == 'R') {		// run telemetry
					sendTelemetryFrames = true;
					monitorPrint("$TR\n", isSerial);
				} else if (frame[2] == 'S') {		// stop telemetry
					sendGPSFrames = false;
					sendTelemetryFrames = false;
					monitorPrint("$TS\n", isSerial);
				} else {							// send telemetry data to the other end
					while (frame[i] != '\n') monitorPrint(frame[i++], !isSerial);
					monitorPrint('\n', !isSerial);
				}
				break;
			case 'S':	// servos
				if ((frame[2] == 'N') || (frame[2] == 'S') || (frame[2] == 'V')) {
					// we receive a decimal encoded number, 1 digit: 0 to 9, A to Z (36 servos)
					number = frame[3] - (frame[3] > '9' ? 'A'-10 : '0');
					if (number > MAX_SERVOS) number = MAX_SERVOS;
					if (frame[2] == 'N') {
						// number of servos
						nbServos = number;
					} else if (frame[2] == 'S') {
						// servo to start with
						firstServo = number-1;
					} else {
						// set single servo: we receive a decimal encoded number, 2 digits
						value = frame[4] - (frame[4] > '9' ? 'A'-10 : '0');
						value = (value << 4) + frame[5] - (frame[5] > '9' ? 'A'-10 : '0');
						servo[number-1].write(value);
					}
				} else if ((frame[2] >= '0') && ((frame[2] <= '9') || (frame[2] >= 'A')) && (frame[2] <= 'F')) {
					// we receive a frame of values for the servos in order
					for (i=0; i<nbServos; i++) {
						value = frame[2+i*2] - (frame[2+i*2] > '9' ? 'A'-10 : '0');
						value = (value << 4) + frame[3+i*2] - (frame[3+i*2] > '9' ? 'A'-10 : '0');
						if (firstServo + i < MAX_SERVOS) servo[firstServo+i].write(value);
					}
				} else {
					monitorPrint("$E\n", isSerial);
					return;
				}
				monitorPrint("$S", isSerial);
				monitorPrint((char)(nbServos > 9 ? nbServos + 'A'-10 : nbServos + '0'), isSerial);
				monitorPrint('\n', isSerial);
				break;
			default:
				monitorPrint("$E\n", isSerial);
				break;
		}
	}
	
	public void monitorPrint(int[] s, boolean isSerial) {		
		if (isSerial) Serial.print(s);
		else xBee.print(s);
	}

	public void monitorPrint(String s, boolean isSerial) {		
		if (isSerial) Serial.print(s);
		else xBee.print(s);
	}

	public void monitorPrint(int c, boolean isSerial) {		
		if (isSerial) Serial.print((char)c);
		else xBee.print((char)c);
	}

	public void monitorPrint(char c, boolean isSerial) {		
		if (isSerial) Serial.print(c);
		else xBee.print(c);
	}

	public void setup() {}
	public void loop() {}
}
