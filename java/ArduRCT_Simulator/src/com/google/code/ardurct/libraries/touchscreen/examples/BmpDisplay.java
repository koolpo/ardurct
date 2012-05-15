package com.google.code.ardurct.libraries.touchscreen.examples;

import com.google.code.ardurct.libraries.File;
import com.google.code.ardurct.libraries.SD;
import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;

public class BmpDisplay extends ArduinoFirmware {
	
	public static final String FILE_NAME = "monk.bmp";
	public final static int MAX_BMP_WIDTH = 320;
	public final static int SD_CS = 4;
	public final static int SS = 4;


	public void setup() {
	    Serial.begin(57600);
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    touchscreen.setMargin(5);
	    touchscreen.setBacklight(180);

	    dualPrint("Initializing SD... ");
	    pinMode(SS, OUTPUT);

	    if (!SD.begin(SD_CS)) {
	        dualPrintln("Failed!");
	        while (true);
	    } else dualPrintln("OK!");

	    int width[] = new int[1];
	    int height[] = new int[1];  
	    File bmpFile = SD.open(FILE_NAME);
	    if (bmpFile == null) {
	        dualPrintln("Can not find image.");
	        while (true);
	    }
	    if (!bmpReadHeader(bmpFile, width, height)) { 
	        bmpFile.close();
	        while (true);
	    }  
	    bmpDraw(bmpFile, 0, 0, width, height);
	    bmpFile.close();

	    delay(1000);
	}


	public void loop() {
	  delay(1000);
	}

	private void dualPrint(String a) {
		Serial.print(a); 
		touchscreen.print(a); 
	}

	private void dualPrint(int a) {
		Serial.print(a); 
		touchscreen.print(a); 
	}

	private void dualPrint(long a) {
		Serial.print(a); 
		touchscreen.print(a); 
	}

	private void dualPrintH(int a) { 
		Serial.print(a, HEX); 
		touchscreen.print(a, HEX); 
	}
	
	private void dualPrintln(String a) {
		Serial.print(a + "\n"); 
		touchscreen.print(a + "\n"); 
	}

	private void dualPrintln(int a) {
		Serial.print(a + "\n"); 
		touchscreen.print(a + "\n"); 
	}

	void bmpDraw(File f, int x, int y, int width[], int height[]) {
	    long time = millis();
	  
	    int bmpBuffer[] = new int[3 * MAX_BMP_WIDTH];  // 3 * pixels to buffer
	    int imgBuffer[] = new int[MAX_BMP_WIDTH];
	    
	    for (int i=0; i<height[0]; i++) {
	        // read one line
	        f.read(bmpBuffer, 3*width[0]);    
	        int ptr = 0;
	        for (int j=0; j<width[0]; j++) {
	            // convert pixel from 888 to 565
	            int b = bmpBuffer[ptr++];  // blue
	            int g = bmpBuffer[ptr++];  // green
	            int r = bmpBuffer[ptr++];  // red
	            int color = r >> 3;
	            color <<= 6;
	            g >>= 2;
	            color |= g;
	            color <<= 5;
	            b >>= 3;
	            color |= b;
	            imgBuffer[j] = color;
	        }
	        // bitmaps are stored with the BOTTOM line first so we have to start from the bottom
	        touchscreen.pasteBitmap(imgBuffer, x, y + height[0]-1 - i, width[0], 1, false, WHITE);
	    }
		touchscreen.setCursor(0, 0);
	    dualPrint(millis() - time);
	    dualPrintln("ms");
	}

	boolean bmpReadHeader(File f, int width[], int height[]) {
	    int tmp = readLE16b(f);
	    if (tmp != 0x4D42) {
	        dualPrint("Start: "); dualPrintH(tmp);
	        dualPrintln(" => Not supported.");
	        // magic bytes missing
	        return false;
	    }
	    // read file size
	    tmp = readLE32b(f);  
	    dualPrint("File size: "); dualPrintln(tmp);
	    // read and ignore creator bytes
	    readLE32b(f);
	    // read offset
	    int bmpImageOffset = readLE32b(f);  
	    // read DIB header
	    tmp = readLE32b(f);
	    width[0] = readLE32b(f);
	    height[0] = readLE32b(f);
	    dualPrint("Image size: "); 
	    dualPrint(width[0]);
	    dualPrint("x");
	    dualPrintln(height[0]); 
	    
	    if (readLE16b(f) != 1)  return false;
	    
	    // read bit depth
	    int bmpDepth = readLE16b(f);
	    dualPrint("Bit depth: "); dualPrint(bmpDepth);
	    if (bmpDepth != 24) {
	        dualPrintln(" => Not supported.");
	        return false;
	    } else dualPrintln("");
	    
	    // read compression
	    int compression = readLE32b(f);
	    dualPrint("Compression: "); dualPrint(compression);
	    if (compression != 0) {
	        dualPrintln(" => Not supported.");
	        return false;
	    } else dualPrintln("");

	    f.seek(bmpImageOffset);
	    return true;
	}


	// Read 16b as Little Endian
	int readLE16b(File f) {
	    int l = f.read();
	    int h = f.read();
	    h <<= 8;
	    h |= l;
	    return h;
	}


	// Read 32b as Little Endian
	int readLE32b(File f) {
	    int l = readLE16b(f);
	    int h = readLE16b(f);
	    h <<= 16;
	    h |= l;
	    return h;
	}


}
