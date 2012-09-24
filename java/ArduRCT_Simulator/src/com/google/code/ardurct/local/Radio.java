package com.google.code.ardurct.local;

import com.google.code.ardurct.libraries.Eeprom;
import com.google.code.ardurct.libraries.XBee;

public class Radio extends Options {
		
	public static final int MAIN_LOOP_DELAY = 20;

	public static final int RADIO_RETRY_CONNECTION_EVERY = 50;		// 1 second RADIO_RETRY_CONNECTION_EVERY/(1/MAIN_LOOP_DELAY)
	public static final int RADIO_MAX_CONNECTION_RETRIES = 500;		// 10 seconds RADIO_MAX_CONNECTION_RETRIES/(1/MAIN_LOOP_DELAY)

	public static final int UI_RADIO_LOCAL_ADDRESS = 30;
	public static final int UI_RADIO_REMOTE_1_ADDRESS = 31;
	public static final int UI_RADIO_REMOTE_2_ADDRESS = 32;
	public static final int UI_RADIO_REMOTE_3_ADDRESS = 33;

	public static final int RADIO_STATUS_ERROR = -1;
	public static final int RADIO_STATUS_BAUDRATE_ERROR = -2;
	public static final int RADIO_STATUS_FIRST_CONNECTION_ERROR = -3;
	public static final int RADIO_STATUS_CONNECTION_ERROR = -4;
	public static final int RADIO_STATUS_UNDEFINED = 0;
	public static final int RADIO_STATUS_FINALIZING = 110;
	public static final int RADIO_STATUS_READY = 100;
	public static final int RADIO_STATUS_ENTERING_COMMAND_MODE = 1;
	public static final int RADIO_STATUS_GETTING_PAN_ID = 6;
	public static final int RADIO_STATUS_CHECKING_ADDRESS = 10;
	public static final int RADIO_STATUS_GETTING_LOCAL_ADDRESS = 15;
	public static final int RADIO_STATUS_GETTING_REMOTE_ADDRESS = 20;
	public static final int RADIO_STATUS_CONFIGURING_MODULE = 25;
	public static final int RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_REMOTE = 35;
	public static final int RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_LOCAL = 36;
	public static final int RADIO_STATUS_PREPARE_TO_SEND_TEST_TO_REMOTE = 85;
	public static final int RADIO_STATUS_SEND_TEST_TO_REMOTE = 90;
	public static final int RADIO_STATUS_RECEIVE_TEST_FROM_REMOTE = 95;
	
 	static final int RADIO_CONFIG_INIT = 0;
	static final int RADIO_CONFIG_ENTER_CMD_MODE = 10;
	static final int RADIO_CONFIG_GET_SHSL = 20;
	static final int RADIO_CONFIG_SET_BD = 30;
	static final int RADIO_CONFIG_SET_ID = 40;
	static final int RADIO_CONFIG_SET_DHDL = 50;
	static final int RADIO_CONFIG_SET_64B = 60;
	static final int RADIO_CONFIG_WRITE = 70;
	static final int RADIO_CONFIG_EXIT_CMD_MODE = 80;
	
	public static final int RADIO_LOCAL_EEPROM_ADDRESS = EEND-95;
	public static final int RADIO_REMOTE_1_EEPROM_ADDRESS = EEND-103;
	public static final int RADIO_REMOTE_2_EEPROM_ADDRESS = EEND-111;
	public static final int RADIO_REMOTE_3_EEPROM_ADDRESS = EEND-119;
		
	public XBee xBee = new XBee();
	
	public int radioStatus;
	public int radioConfigurationStatus;
	public int[] radioPanId = { '0', '0', '0', '0' };
	public int radioBaudrate;
	public int radioConnectionRetries;
	public int[] radioLocalAddress = new int[8];
	public int[] radioRemote1Address = new int[8];
	public int[] radioRemote2Address = new int[8];
	public int[] radioRemote3Address = new int[8];
	
	public int[] radioFrame = new int[64];
	public int radioFramePtr;

	public void radioSetup(int baudrate) {
		xBee.begin(Serial1, baudrate);
	    radioStatus = RADIO_STATUS_UNDEFINED;
	    radioConfigurationStatus = 0;
	    radioConnectionRetries = 0;
	    radioBaudrate = baudrate;
	    readModuleAddressInEeprom(radioLocalAddress, RADIO_LOCAL_EEPROM_ADDRESS);
	    readModuleAddressInEeprom(radioRemote1Address, RADIO_REMOTE_1_EEPROM_ADDRESS);
	    readModuleAddressInEeprom(radioRemote2Address, RADIO_REMOTE_2_EEPROM_ADDRESS);
	    readModuleAddressInEeprom(radioRemote3Address, RADIO_REMOTE_3_EEPROM_ADDRESS);
	}
	
	public void radioProcess() {	
		int[] buffer = new int[20];
		
		if (radioStatus <= RADIO_STATUS_ERROR) return;

		// process pending commands
	    xBee.processCommand();
	    if (xBee.isProcessingCommand()) return;

	    switch (radioStatus) {
	    		
	    	case RADIO_STATUS_READY:
	    		if (xBee.available() <= 0) return;
	    		while (xBee.available() > 0) {
	    			radioFrame[radioFramePtr++] = xBee.read();
	    			if (radioFrame[radioFramePtr-1] == '\n') break;
	    		}
	    		if (radioFrame[radioFramePtr-1] == '\n') radioProcessFrame();
	    		break;
	    		
	    	case RADIO_STATUS_UNDEFINED:
	    		xBee.enterCommandMode();
	    		radioStatus = RADIO_STATUS_ENTERING_COMMAND_MODE;
	    		break;
	    		
	    	case RADIO_STATUS_ENTERING_COMMAND_MODE:
	    		if (!xBee.isInCommandMode()) {
	    			radioStatus = RADIO_STATUS_BAUDRATE_ERROR;
	    			return;
	    		}
	    		xBee.getId();
	    		radioStatus = RADIO_STATUS_GETTING_PAN_ID;
	    		break;
	    		
	    	case RADIO_STATUS_GETTING_PAN_ID:
	    	    copyArray(xBee.getId(), radioPanId, 4);
	    		setPanIdLabels();
	    		xBee.getSHSL();
	    	    if (!radioAddressIsDefined(radioLocalAddress)){
	    	    	// never configured, get the local address
	    	    	radioStatus = RADIO_STATUS_GETTING_LOCAL_ADDRESS;
	    	    } else if (!radioAddressIsDefined(radioRemote1Address)){
	    	    	// remote radio not known, get its address
	    	    	radioStatus = RADIO_STATUS_GETTING_REMOTE_ADDRESS;
	    	    } else {
	    	    	// we test which radio is plugged in and maybe configure it
	    	    	radioStatus = RADIO_STATUS_CHECKING_ADDRESS;
	    	    }
	    		break;

	    	case RADIO_STATUS_GETTING_LOCAL_ADDRESS:
	    		hexStringToArray(xBee.getSHSL(), radioLocalAddress, 16);
	    		// save it for future use
	    		writeModuleAddressInEeprom(radioLocalAddress, RADIO_LOCAL_EEPROM_ADDRESS);
	    		// ask to switch the module
	    		radioStatus = RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_REMOTE;
	    		break;

	    	case RADIO_STATUS_GETTING_REMOTE_ADDRESS:
	    		hexStringToArray(xBee.getSHSL(), buffer, 16);
	    		if (arrayEqual(buffer, radioLocalAddress, 8)) {
	    			// ask to switch the module, as it is still the same
		    		radioStatus = RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_REMOTE;
		    		return;
	    		} 
    			// save it in the next slot available
	    		if (!radioAddressIsDefined(radioRemote1Address)) {
	    			copyArray(buffer, radioRemote1Address, 8);
	    			writeModuleAddressInEeprom(radioRemote1Address, RADIO_REMOTE_1_EEPROM_ADDRESS);
	    		} else if (!radioAddressIsDefined(radioRemote2Address)) {
	    			copyArray(buffer, radioRemote2Address, 8);
	    			writeModuleAddressInEeprom(radioRemote2Address, RADIO_REMOTE_2_EEPROM_ADDRESS);
	    		} else {
	    			copyArray(buffer, radioRemote3Address, 8);
	    			writeModuleAddressInEeprom(radioRemote3Address, RADIO_REMOTE_3_EEPROM_ADDRESS);	    			
	    		}
	    		// write the configuration
	    		radioStatus = RADIO_STATUS_CONFIGURING_MODULE;
	    		radioConfigurationStatus = RADIO_CONFIG_INIT;
	    		
	    	case RADIO_STATUS_CONFIGURING_MODULE:
	    		radioConfigureModule();
	    		break;
	    		
	    	case RADIO_STATUS_CHECKING_ADDRESS:
	    		hexStringToArray(xBee.getSHSL(), buffer, 16);
	    		if (arrayEqual(buffer, radioLocalAddress, 8)) {
	    			// we are on local XBee, check the configuration
	    			if (xBee.getDHDL() == null) return;
	    			hexStringToArray(xBee.getDHDL(), buffer, 16);
	    			if (arrayEqual(buffer, radioRemote1Address, 8)) {
		    			// if DHDL is remote, test the connection
	    				radioConnectionRetries = 0;
		    			radioStatus = RADIO_STATUS_PREPARE_TO_SEND_TEST_TO_REMOTE;
		    			xBee.exitCommandMode();
		    		} else {
			    		radioStatus = RADIO_STATUS_CONFIGURING_MODULE;
			    		radioConfigurationStatus = RADIO_CONFIG_INIT;		    			
		    		}
	    		} else if (arrayEqual(buffer, radioRemote1Address, 8) || 
	    					arrayEqual(buffer, radioRemote2Address, 8) ||
	    					arrayEqual(buffer, radioRemote3Address, 8)) {
	    			radioStatus = RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_LOCAL;
	    		} else if (!radioAddressIsDefined(radioRemote2Address) || !radioAddressIsDefined(radioRemote3Address)) {
	    			// we are on a new remote, get its details
	    			radioStatus = RADIO_STATUS_GETTING_REMOTE_ADDRESS;
	    		} else {
	    			radioStatus = RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_LOCAL;
	    		}
	    		break;
	    	
	    	case RADIO_STATUS_PREPARE_TO_SEND_TEST_TO_REMOTE:
	    		radioStatus = RADIO_STATUS_SEND_TEST_TO_REMOTE;
	    		break;
	    		
	    	case RADIO_STATUS_SEND_TEST_TO_REMOTE:
	    		if ((radioConnectionRetries % RADIO_RETRY_CONNECTION_EVERY) == 0) xBee.print("$CT\n");
	    		radioStatus = RADIO_STATUS_RECEIVE_TEST_FROM_REMOTE;
	    		break;
	    		
	    	case RADIO_STATUS_RECEIVE_TEST_FROM_REMOTE:
	    		radioConnectionRetries ++;
	    		radioStatus = RADIO_STATUS_CONNECTION_ERROR;
	    		if (radioConnectionRetries == RADIO_RETRY_CONNECTION_EVERY) radioStatus = RADIO_STATUS_FIRST_CONNECTION_ERROR;
	    		if (xBee.available() >= 4) {
	    			if ((xBee.read() == '$') && (xBee.read() == '.') && (xBee.read() == 'C') && (xBee.read() == '\n')) {
	    				radioStatus = RADIO_STATUS_FINALIZING;
	    			}
	    		}
	    		if ((radioStatus == RADIO_STATUS_CONNECTION_ERROR) && (radioConnectionRetries > RADIO_MAX_CONNECTION_RETRIES)) 
	    			radioStatus = RADIO_STATUS_ERROR;
	    		break;

	    	case RADIO_STATUS_FINALIZING:
	    	default:
	    		break;

	    }
	}

	public void radioConfigureModule() {
		int[] buffer = new int[20];
		
		switch (radioConfigurationStatus) {
			case RADIO_CONFIG_INIT:
				if (!xBee.isInCommandMode()) {
					xBee.enterCommandMode();
					return;
				}

			case RADIO_CONFIG_GET_SHSL:
				if (xBee.getSHSL() == null) return;
				radioConfigurationStatus = RADIO_CONFIG_SET_BD;
				xBee.setBaudrate(radioBaudrate);
				break;
				
			case RADIO_CONFIG_SET_BD:
				radioConfigurationStatus = RADIO_CONFIG_SET_ID;
				xBee.setId(radioPanId);
				break;
				
			case RADIO_CONFIG_SET_ID:
				radioConfigurationStatus = RADIO_CONFIG_SET_64B;
				xBee.enable64BitsAdressing();
				break;

			case RADIO_CONFIG_SET_64B:
				radioConfigurationStatus = RADIO_CONFIG_SET_DHDL;
				hexStringToArray(xBee.getSHSL(), buffer, 16);
				if (arrayEqual(buffer, radioLocalAddress, 8)) {
					// we are in local module, we have to set radioRemoteAddress_1 as DHDL
					arrayToHexString(radioRemote1Address, buffer, 8);
				} else {
					// we are in remote module, we have to set radioLocalAddress as DHDL
					arrayToHexString(radioLocalAddress, buffer, 8);					
				}
				xBee.setDHDL(buffer);
				break;

			case RADIO_CONFIG_SET_DHDL:
				radioConfigurationStatus = RADIO_CONFIG_WRITE;
				xBee.commitToXBeeFlash();
				break;

			case RADIO_CONFIG_WRITE:
				radioConfigurationStatus = RADIO_CONFIG_EXIT_CMD_MODE;
				xBee.exitCommandMode();
				break;

			case RADIO_CONFIG_EXIT_CMD_MODE:
				hexStringToArray(xBee.getSHSL(), buffer, 16);
				if (arrayEqual(buffer, radioLocalAddress, 8)) {
    				radioConnectionRetries = 0;
					radioStatus = RADIO_STATUS_PREPARE_TO_SEND_TEST_TO_REMOTE;
				} else {
					radioStatus = RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_LOCAL;
				}
				break;
		}

	}
	
	public void radioResetAddress(int[] address, int eepromAddress) {
		for (int i=0; i<8; i++) address[i] = 0xFF;
		writeModuleAddressInEeprom(address, eepromAddress);
	}
	
	public void radioCommandEngines(boolean stop) {
		if (stop) xBee.print("$MS\n");
		else xBee.print("$MR\n");
	}
	
	public void radioProcessFrame() {	
		int i = 0;
		if ((radioFramePtr > 3) && (radioFrame[radioFramePtr-1] == '\n') && (radioFrame[0] == '$')) {
			boolean absorbFrame = true;
			// we are receiving Remotuino data
			switch (radioFrame[1]) {
				case 'B':	// battery
					remoteBattery = radioFrame[2] - (radioFrame[2] < 'A' ? '0' : 'A'-10);
					remoteBattery = (remoteBattery << 4) + radioFrame[3] - (radioFrame[3] < 'A' ? '0' : 'A'-10);
					remoteBatteryIsMeasured = true;
					break;
				case 'G':	// GPS
					absorbFrame = false;
					break;
				case 'T':	// Telemetry
					i = 0;
					telemetryDataHasChanged = true;
					switch (radioFrame[2]) {
						case 'F':	// Fix
							if (radioFrame[3] == 'Y') telemetryGPSHasLock = true;
							else telemetryGPSHasLock = false;
							break;
						case 'L':	// Longitude
							copyRadioFrameToField(telemetryLongitudeDisplay, true);
							break;
						case 'l':	// latitude
							copyRadioFrameToField(telemetryLatitudeDisplay, true);							
							break;
						case 'A':	// Altitude
							i = copyRadioFrameToField(telemetryAltitudeDisplay, false);
							telemetryAltitudeDisplay[i++] = 'm';
							telemetryAltitudeDisplay[i++] = 0;
							break;
						case 's':	// speed
							i = parseDecimalFromRadioFrame();
							decimalIntToMeterDisplay(i, telemetrySpeedDisplay);
							break;
						case 'D':	// Distance (received as one decimal int)
							telemetryDistance = parseDecimalFromRadioFrame();
							decimalIntToMeterDisplay(telemetryDistance, telemetryDistanceDisplay);
							break;
						case 'H':	// Height (received as one decimal int)
							telemetryHeight = parseDecimalFromRadioFrame();
							decimalIntToMeterDisplay(telemetryHeight, telemetryHeightDisplay);
							break;
						case 'a':	// angle (received as one decimal int)
							telemetryAngle = parseDecimalFromRadioFrame();
							break;
						case 'S':	// Stopped
							break;
						case 'R':	// Run
							break;
					}
					break;
				case 'M':
					if (radioFrame[2] == 'R') {
						if (!enginesAreRunning) enginesStartTime = millis();
						enginesAreRunning = true;
					} else {
						if (enginesAreRunning) {
							long now = millis();
							telemetryRunTime += now - enginesStartTime;
							enginesStartTime = now;
						}
						enginesAreRunning = false;
					}
					break;
				default:
					break;
			}
			if (absorbFrame) radioFramePtr = 0;
		}
		i = 0;
		while (i < radioFramePtr) Serial.print((char)radioFrame[i++]);
		radioFramePtr = 0;
		radioStrength = (int) Math.round(Math.abs(Math.random() * 63));
	}
	
	public int copyRadioFrameToField(int[] field, boolean isDMS) {
		int i = 0;
		int j = 0;
		boolean padding = true;
		while (radioFrame[i+3] != '\n') {
			if (padding && (radioFrame[i+3] == '0') && (radioFrame[i+4] != '.')) padding = true;		// remove leading 0
			else padding = false;
			field[j] = (padding ? ' ' : radioFrame[i+3]);
			j++;
			i++;
			if (isDMS && ((j == 3) || (j == 6) || (j == 9) || (j == 13))) j++;	// keep formating for DMS
		}
		field[j] = 0;
		return j;
	}
	
	public int parseDecimalFromRadioFrame() {
		int value = 0;
		int i = 3;
		while (radioFrame[i] != '\n') value = value * 10 + (radioFrame[i++] - '0');
		return value;
	}
	
	public void decimalIntToMeterDisplay(int dec, int[] display) {
		display[DISPLAY_METER_LENGTH] = 'm';
		int i = DISPLAY_METER_LENGTH - 1;
		if (dec < 1000) {		// up to 100m, we show the decimal
			display[i--] = (dec % 10) + '0';
			display[i--] = '.';
			display[i] = '0';
		}
		dec = dec / 10;
		while (dec > 0) {
			display[i--] = (dec % 10) + '0';
			dec = dec / 10;
		}
		while (i >= 0) display[i--] = ' ';
	}
	
	public void radioMeasureRemoteBattery() {
		xBee.print("$B\n");
	}
	
	
	public void radioCommitChanges() {
		radioConfigurationStatus = RADIO_CONFIG_INIT;
		radioStatus = RADIO_STATUS_CONFIGURING_MODULE;
	}
	
	public void readModuleAddressInEeprom(int[] address, int eepromAddress) {
		for (int i=0; i<8; i++) address[i] = Eeprom.read_uint8_t(eepromAddress + i);
	}
	
	public void writeModuleAddressInEeprom(int[] address, int eepromAddress) {
		for (int i=0; i<8; i++) Eeprom.write_uint8_t(eepromAddress + i, address[i]);
	}
	
	public int[] arrayToHexString(int[] array, int[] hexString, int arraySize) {
		hexString[arraySize*2] = 0;
		for (int i=0; i<arraySize; i++) {
			hexString[i*2] = (array[i] >> 4) + '0';
			if (hexString[i*2] > '9') hexString[i*2] += 'A' - '0' - 10;
			hexString[i*2+1] = (array[i] & 0x0F) + '0';
			if (hexString[i*2+1] > '9') hexString[i*2+1] += 'A' - '0' - 10;
		}
		return hexString;
	}
	
	public void hexStringToArray(int[] hexString, int[] array, int hexSize) {
		for (int i=0; i<hexSize/2; i++) {
			array[i] = hexString[i*2] - (hexString[i*2] > '9' ? 'A'-10 : '0');
			array[i] = (array[i] << 4) + hexString[i*2+1] - (hexString[i*2+1] > '9' ? 'A'-10 : '0');
		}
	}
	
	public boolean arrayEqual(int[] array1, int[] array2, int size) {
		for (int i=0; i<size; i++) {
			if (array1[i] != array2[i]) return false;
		}
		return true;
	}

	public boolean radioAddressIsDefined(int[] address) {
		for (int i=0; i<8; i++) {
			if (address[i] != 0xFF) return true;
		}
		return false;
	}
	
	public boolean copyArray(int[] array1, int[] array2, int size) {
		for (int i=0; i<size; i++) array2[i] = array1[i];
		return true;
	}

	public void setup() {}
	public void loop() {}
	public void setPanIdLabels() {}

}
