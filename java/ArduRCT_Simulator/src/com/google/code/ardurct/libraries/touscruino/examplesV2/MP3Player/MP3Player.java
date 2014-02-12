package com.google.code.ardurct.libraries.touscruino.examplesV2.MP3Player;

import com.google.code.ardurct.libraries.File;
import com.google.code.ardurct.libraries.SD;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruino;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class MP3Player extends TouscruinoFirmware {
	
	private static final int MAX_FILES = 10;
	
	MP3File mp3Files[] = new MP3File[MAX_FILES];
	int nbMP3Files = -1;
	
	ArduRCT_TouScruino touscruino = new ArduRCT_TouScruino();

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
 		
	    touscruino.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
	    touscruino.setBacklight(180);
	    touscruino.setBackgroundColor(COLOR_565(0xEE, 0xEE, 0xCC));
		touscruino.setRotation(GRAPHICS_ROTATION_90);
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
		touscruino.fillRectangle(0, y, touscruino.getWidth(), height, touscruino.getBackgroundColor());
		touscruino.drawLine(0, y, touscruino.getWidth(), y, touscruino.getForegroundColor(), 1);
		touscruino.drawString(mp3Files[id].title, 5, y+5, touscruino.getForegroundColor(), FONT_MEDIUM, FONT_BOLD, false);
		touscruino.drawString(mp3Files[id].duration, touscruino.getWidth()-5-touscruino.getStringWidth(mp3Files[id].duration, FONT_MEDIUM), 
				y+5, touscruino.getForegroundColor(), FONT_MEDIUM, FONT_BOLD, false);
		touscruino.drawString(mp3Files[id].performer, 5, y+22, touscruino.getForegroundColor(), FONT_SMALL, FONT_PLAIN, false);
		int[] kbps = { ' ', ' ', ' ', 'k', 'b', 'p', 's', 0 };
		MP3File.numberToString(mp3Files[id].bitRate, kbps, 0, 2, ' ');
		touscruino.drawString(kbps, touscruino.getWidth()-5-touscruino.getStringWidth(kbps, FONT_SMALL), 
				y+22, touscruino.getForegroundColor(), FONT_SMALL, FONT_PLAIN, false);
		touscruino.drawString(mp3Files[id].album, 5, y+35, touscruino.getForegroundColor(), FONT_SMALL, FONT_PLAIN, false);
		int[] hz = { ' ', ' ', ' ', ' ', ' ', 'H', 'z', 0 };
		MP3File.numberToString(mp3Files[id].samplingRate, hz, 0, 4, ' ');
		touscruino.drawString(hz, touscruino.getWidth()-5-touscruino.getStringWidth(hz, FONT_SMALL), 
				y+35, touscruino.getForegroundColor(), FONT_SMALL, FONT_PLAIN, false);
		touscruino.drawLine(0, y+height, touscruino.getWidth(), y+height, touscruino.getForegroundColor(), 1);
	}
}
