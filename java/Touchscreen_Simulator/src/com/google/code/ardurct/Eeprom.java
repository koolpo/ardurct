package com.google.code.ardurct;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;

public class Eeprom {
	public static final String FILENAME = "eeprom.txt";
	
	public final static int SIZE = 0x1000;
	public final static int PAGE_WIDTH = 32;
	
	private static int eeprom[] = null;
	
	private static void initialize() {
		if (eeprom != null) return;
		eeprom = new int[SIZE];
		if (!(new File(FILENAME)).exists()) {
			for (int i=0; i<SIZE; i++) eeprom[i] = 0xFF;
			try {
				PrintWriter pw = new PrintWriter(FILENAME);
				for (int line=0; line<SIZE/PAGE_WIDTH; line++) {
					for (int column=0; column<PAGE_WIDTH; column++) {
						pw.print("FF ");
					}
					pw.println();
				}
				pw.flush();
				pw.close();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			}
		} else {
			try {
				BufferedReader bf = new BufferedReader(new FileReader(FILENAME));
				String line = null;
				int lineNb = 0;
				while ((line = bf.readLine()) != null) {
					String parts[] = line.split("\\s");
					for (int column=0; column<parts.length; column++) {
						eeprom[lineNb*PAGE_WIDTH+column] = Integer.parseInt(parts[column], 16);
					}
					lineNb ++;
				}
				bf.close();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
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
		try {
			PrintWriter pw = new PrintWriter(FILENAME);
			for (int line=0; line<SIZE/PAGE_WIDTH; line++) {
				for (int column=0; column<PAGE_WIDTH; column++) {
					String hex = Integer.toHexString(eeprom[line*PAGE_WIDTH+column]).toUpperCase();
					if (hex.length() < 2) hex = "0" + hex;
					pw.print(hex + " ");
				}
				pw.println();
			}
			pw.flush();
			pw.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();			
		}
	}
	
	
}
