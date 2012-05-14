package com.google.code.ardurct.libraries.touchscreen.examples.MP3Player;

import com.google.code.ardurct.libraries.File;
import com.google.code.ardurct.libraries.SD;
import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;

public class MP3Player extends ArduinoFirmware {
	
	private static final int MAX_FILES = 10;
	
	MP3File mp3Files[] = new MP3File[MAX_FILES];
	int nbMP3Files = -1;
	
	public void setup() {
		Serial.begin(9600);
		Serial.print("Initializing SD card...");
		
		// On the Ethernet Shield, CS is pin 4. It's set as an output by default.
		// Note that even if it's not used as the CS pin, the hardware SS pin 
		// (10 on most Arduino boards, 53 on the Mega) must be left as an output 
		// or the SD library functions will not work. 
		pinMode(10, OUTPUT);

  		if (!SD.begin(10)) {
	  		Serial.println("failed!");
    		return;
  		}
  		Serial.println("done.");
  		for (int i=0; i<MAX_FILES; i++) mp3Files[i] = new MP3File();
  		findMP3s();
 		Serial.println("done!");
 		
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    touchscreen.setBacklight(180);
	    touchscreen.setBackgroundColor(COLOR_565(0xEE, 0xEE, 0xCC));
	    for (int i=0; i<nbMP3Files; i++) drawMP3(i, i*45, 45);
	}

	public void loop() {
	  // nothing happens after setup finishes.
	}

	void findMP3s() {
		File root = SD.open("/");
		File entry =  root.openNextFile();
		nbMP3Files = 0;
		Serial.println("Searching for *.MP3 at root...");
		while ((entry != null) && (nbMP3Files < MAX_FILES)) {
			int filename[] = entry.name();
			int i = 0;
			while (filename[i] != '.') i++;
			if ((filename[i+1] == 'M') && (filename[i+2] == 'P') && (filename[i+3] == '3')) {
				Serial.print(" ");
				Serial.println(filename);
				mp3Files[nbMP3Files].setFilename(filename);
				mp3Files[nbMP3Files].parseHeader();
				mp3Files[nbMP3Files].parseTags();
				Serial.print("  Bit rate:       ");	Serial.print(mp3Files[nbMP3Files].bitRate);			Serial.println(" Kb/s");
				Serial.print("  Sampling freq.: ");	Serial.print(mp3Files[nbMP3Files].samplingRate);	Serial.println(" Hz");
				Serial.print("  Duration:       ");	Serial.println(mp3Files[nbMP3Files].duration);
				Serial.print("  Album:          ");	Serial.println(mp3Files[nbMP3Files].album);
				Serial.print("  Title:          ");	Serial.println(mp3Files[nbMP3Files].title);
				Serial.print("  Performer:      ");	Serial.println(mp3Files[nbMP3Files].performer);				
				nbMP3Files++;
			}
			entry =  root.openNextFile();
		}
	}

	void drawMP3(int id, int y, int height) {
		touchscreen.fillRectangle(0, y, touchscreen.getWidth(), height, touchscreen.getBackgroundColor());
		touchscreen.drawLine(0, y, touchscreen.getWidth(), y, touchscreen.getForegroundColor(), 1);
		touchscreen.drawString(mp3Files[id].title, 5, y+5, touchscreen.getForegroundColor(), FONT_MEDIUM, FONT_BOLD, false);
		touchscreen.drawString(mp3Files[id].duration, touchscreen.getWidth()-5-touchscreen.getStringWidth(mp3Files[id].duration, FONT_MEDIUM), 
				y+5, touchscreen.getForegroundColor(), FONT_MEDIUM, FONT_BOLD, false);
		touchscreen.drawString(mp3Files[id].performer, 5, y+22, touchscreen.getForegroundColor(), FONT_SMALL, FONT_PLAIN, false);
		int[] kbps = { ' ', ' ', ' ', 'k', 'b', 'p', 's', 0 };
		MP3File.numberToString(mp3Files[id].bitRate, kbps, 0, 2, ' ');
		touchscreen.drawString(kbps, touchscreen.getWidth()-5-touchscreen.getStringWidth(kbps, FONT_SMALL), 
				y+22, touchscreen.getForegroundColor(), FONT_SMALL, FONT_PLAIN, false);
		touchscreen.drawString(mp3Files[id].album, 5, y+35, touchscreen.getForegroundColor(), FONT_SMALL, FONT_PLAIN, false);
		int[] hz = { ' ', ' ', ' ', ' ', ' ', 'H', 'z', 0 };
		MP3File.numberToString(mp3Files[id].samplingRate, hz, 0, 4, ' ');
		touchscreen.drawString(hz, touchscreen.getWidth()-5-touchscreen.getStringWidth(hz, FONT_SMALL), 
				y+35, touchscreen.getForegroundColor(), FONT_SMALL, FONT_PLAIN, false);
		touchscreen.drawLine(0, y+height, touchscreen.getWidth(), y+height, touchscreen.getForegroundColor(), 1);
	}
}
