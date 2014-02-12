package com.google.code.ardurct.libraries;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class Eeprom {
	public static final String FILENAME = "eeprom.txt";
	
	public final static int SIZE = 0x1000;
	public final static int PAGE_WIDTH = 32;
	
	private static int eeprom[] = null;
	private static byte eepromImage[] = null;
	
	private static void initialize() {
		if (eeprom != null) return;
		eeprom = new int[SIZE];
		eepromImage = new byte[SIZE*3];
		if (!(new File(FILENAME)).exists()) {
			for (int i=0; i<SIZE; i++) {
				eeprom[i] = 0xFF;
				eepromImage[i*3] = 'F';
				eepromImage[i*3+1] = 'F';
				eepromImage[i*3+2] = (byte) (((i % PAGE_WIDTH) == PAGE_WIDTH-1) ? '\n' : ' ');
			}
			try {
				FileOutputStream fos = new FileOutputStream(FILENAME);
				fos.write(eepromImage);
				fos.flush();
				fos.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else {
			try {
				FileInputStream fis = new FileInputStream(FILENAME);
				fis.read(eepromImage);
				fis.close();
				for (int i=0; i<SIZE; i++) {
					eeprom[i] = eepromImage[i*3] - (eepromImage[i*3] > '9' ? 'A'-10 : '0');
					eeprom[i] = (eeprom[i] << 4) + eepromImage[i*3+1] - (eepromImage[i*3+1] > '9' ? 'A'-10 : '0');
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public static int read_uint8_t(int address) {
		initialize();
		return eeprom[address];
	}

	public static void write_uint8_t(int address, int value) {
		initialize();
		eeprom[address] = value & 0x0FF;
		String sValue = Integer.toHexString(value).toUpperCase();
		eepromImage[address*3] = (byte) (sValue.length() == 1 ? '0' : sValue.charAt(0));
		eepromImage[address*3+1] = (byte) (sValue.length() == 1 ? sValue.charAt(0) : sValue.charAt(1));
		try {
			FileOutputStream fos = new FileOutputStream(FILENAME);
			fos.write(eepromImage);
			fos.flush();
			fos.close();
		} catch (Exception e) {
			e.printStackTrace();			
		}
	}
	
	
}
