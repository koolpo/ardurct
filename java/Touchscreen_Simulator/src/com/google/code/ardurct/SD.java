package com.google.code.ardurct;

public class SD {
	
	public static String LOCATION = "SD";
	
	public static boolean begin(int pin) {
		return true;
	}
	
	public static File open(String filename) {
		return new File(LOCATION + "/" + filename);
	}

	public static File open(int filename[]) {
		byte bFilename[] = new byte[filename.length];
		for (int i=0; i<filename.length; i++) bFilename[i] = (byte)filename[i];
		return new File(LOCATION + "/" + new String(bFilename, 0, filename.length));
	}

}
