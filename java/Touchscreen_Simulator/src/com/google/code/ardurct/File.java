package com.google.code.ardurct;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class File {

	String name;
	FileInputStream fis = null;
	long filePtr = -1;
	String dirEntries[] = null;
	int dirEntriesPtr = -1;
	
	public File(String name) {
		this.name = name;
	}
	
	public int[] name() {
		String filename = name.substring(SD.LOCATION.length() + 1);
		int buffer[] = new int[filename.length() + 1];
		buffer[filename.length()] = 0;
		for (int i=0; i<filename.length(); i++) buffer[i] = filename.charAt(i);
		return buffer;
	}
	
	private void open() {
		try {
			fis = new FileInputStream(name);
			filePtr = 0;
		} catch (FileNotFoundException e) {
			fis = null;
		}		
	}
	
	public void close() {
		filePtr = -1;
		if (fis == null) return;
		try {
			fis.close();
		} catch (IOException e) {
		}
	}
	
	public boolean isDirectory() {
		return (new java.io.File(name)).isDirectory();
	}
	
	public File openNextFile() {
		java.io.File dir = new java.io.File(name); 
		if (!dir.isDirectory()) return null;
		if (dirEntriesPtr == -1) {
			dirEntries = dir.list();
			dirEntriesPtr = 0;
		}
		if (dirEntriesPtr < dirEntries.length) return new File(SD.LOCATION + "/" + dirEntries[dirEntriesPtr++]);
		return null;
	}
	
	public int read() {
		if (filePtr == -1) open();
		if (fis == null) return -1;
		try {
			filePtr ++;
			return ((int)fis.read()) & 0x0FF;
		} catch (IOException e) {
			return -1;
		}
	}

	public void read(int[] buffer, int len) {
		if (filePtr == -1) open();
		if (fis == null) return;
		try {
			byte read[] = new byte[len];
			fis.read(read, 0, len);
			for (int i=0; i<len; i++) buffer[i] = ((int)read[i]) & 0x0FF;
			filePtr += len;
		} catch (IOException e) {
			return;
		}
	}
	
	public void seek(int position) {
		if (fis == null) return;
		if (filePtr > position) {
			close();
			open();
		}
		byte dummy[] = new byte[2048];
		while (position - filePtr > 2048) {
			try {
				fis.read(dummy, 0, 2048);
				filePtr += 2048;
			} catch (IOException e) {
				return;
			}
		}
		while (filePtr != position) read(); 
	}
	
	public int size() {
		java.io.File f = new java.io.File(name);
		return (int)(f.length()); 
	}
}
