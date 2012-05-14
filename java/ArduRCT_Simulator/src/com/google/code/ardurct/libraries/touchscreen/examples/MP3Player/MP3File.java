package com.google.code.ardurct.libraries.touchscreen.examples.MP3Player;

import com.google.code.ardurct.libraries.File;
import com.google.code.ardurct.libraries.SD;

public class MP3File {

	private static final int bitRates[][] = {
	  //  V1L1 V1L2 V1L3 V2L1 V2L2/V2L3
		{  -1,  -1,  -1,  -1,  -1 },	// 0000
		{  32,  32,	 32,  32,   8 },	// 0001
		{  64,  48,  40,  48,  16 },	// 0010
		{  96,	56,  48,  56,  24 },	// 0011
		{ 128,  64,  56,  64,  32 },	// 0100
		{ 160,  80,  64,  80,  40 },	// 0101
		{ 192,  96,  80,  96,  48 },	// 0110
		{ 224, 112,  96, 112,  56 },	// 0111
		{ 256, 128, 112, 128,  64 },	// 1000
		{ 288, 160, 128, 144,  80 },	// 1001
		{ 320, 192, 160, 160,  96 },	// 1010
		{ 352, 224, 192, 176, 112 },	// 1011
		{ 384, 256, 224, 192, 128 },	// 1100
		{ 416, 320, 256, 224, 144 },	// 1101
		{ 448, 384, 320, 256, 160 },	// 1110
		{  -1,  -1,  -1,  -1,  -1 },	// 1111
	};
		
	private static final int samplingFrequencies[][] = {
	   // MPEG1  MPEG2  MPEG2.5
		{ 44100, 22050, 11025 },	// 00	
		{ 48000, 24000, 12000 },	// 01
		{ 32000, 16000,  8000 }		// 10
	};

	int[] filename = new int[30];
	int[] title = new int[30];
	int[] performer = new int[30];
	int[] album = new int[30];
	int bitRate;
	int samplingRate;
	int seconds;
	int headerSize = 0;
	int[] duration = new int[10];
	
	MP3File() {
		filename[0] = 0;
		title[0] = 0;
		performer[0] = 0;
		album[0] = 0;
	}

	public void setFilename(int[] filename) {
		this.filename = filename;
	}

	public void parseHeader() {
		int buffer[] = new int[1024];
		File mp3 = SD.open(filename);
		mp3.read(buffer, 1024);
		int i = 0; 
		headerSize = 0;
		// check if we have an ID3 tag at beginning
		if (bufferMatchesTag(buffer, 0, "ID3")) {
			headerSize = ((buffer[6] & 0x7F) << 21) + ((buffer[7] & 0x7F) << 14) + ((buffer[8] & 0x7F) << 7) + (buffer[9] & 0x7F);
			mp3.seek(headerSize);
			mp3.read(buffer, 1024);
			i = 0;
		}
		// we search for the synchronization 11111111 111xxxxx
		while (true) {
			while (buffer[i] != 0xFF) {
				i++;
				if (i == 1024) { mp3.read(buffer, 1024); i = 0;	}
			}
			if ((buffer[i+1] & 0xE0) == 0xE0) break;
		}
		headerSize += i;
		// we are now in a mp3 frame		
		i++;
		if (i == 1024) { mp3.read(buffer, 2); i = 0;	}
		int version = (buffer[i] & (0x03 << 3)) >> 3;
		int layer = (buffer[i] & (0x03 << 1)) >> 1;
		i++;
		if (i == 1024) { mp3.read(buffer, 1); i = 0;	}
		int bitRateIndex = (buffer[i] & 0xF0) >> 4;
		int samplingFreqIndex = (buffer[i] & (0x03 << 2)) >> 2;
		
		int bitRateColumn = 4;													// V2L2/V2L3
		if ((version == 3) && (layer == 3)) bitRateColumn = 0;					// V1L1
		else if ((version == 3) && (layer == 2)) bitRateColumn = 1;				// V1L2
		else if ((version == 3) && (layer == 1)) bitRateColumn = 2;				// V1L3
		else if (((version & 0x01) == 0) && (layer == 3)) bitRateColumn = 3;	// V2L1
		bitRate = bitRates[bitRateIndex][bitRateColumn];
		
		int samplingFreqColumn = 2;							// MPEG2.5
		if (version == 3) samplingFreqColumn = 0;			// MPEG1
		else if (version == 2) samplingFreqColumn = 1;		// MPEG2
		samplingRate = samplingFrequencies[samplingFreqIndex][samplingFreqColumn];
		
		int KBps = (bitRate * 1000) >> 3;
		int dataSize = mp3.size() - (headerSize >> 3);
		seconds = dataSize / KBps;
		
		i = 0;
		i += numberToString(seconds / 60, duration, i, 0, ' ');
		duration[i++] = ':';
		numberToString(seconds % 60, duration, i, 1, '0');
		mp3.close();
	}

	public void parseTags() {
		int buffer[] = new int[128];
		File mp3 = SD.open(filename);
		mp3.read(buffer, 10);
		// check if we have an ID3 tag at beginning
		if (bufferMatchesTag(buffer, 0, "ID3")) {
			// Search for Album (TALB), Title (TIT2) and Performer (TPE1)
			int i = 10;
			while (i < headerSize) {
				mp3.read(buffer, 10);	// 10 bytes in a header
				i += parseID3v2Tag(mp3, buffer);
				if ((performer[0] != 0) && (title[0] != 0) && (album[0] != 0)) break;
			}			
		} else {
			// tags are at the end
			mp3.seek(mp3.size()-128);
			mp3.read(buffer, 128);
			int i = 0;
			while ((i < 128) && (!bufferMatchesTag(buffer, i, "TAG"))) i++;
			int start = i;
			parseID3v1Field(title, buffer, start+3);
			parseID3v1Field(performer, buffer, start+33);
			parseID3v1Field(album, buffer, start+63);
		}
		mp3.close();
	}

	void printIntArray(int[] array) {
		int i = 0;
		while (array[i] != 0) System.out.print((char)array[i++]);
		System.out.print(" ");
	}
	
	int parseID3v2Tag(File mp3, int[] tag) {
		int size = ((tag[4] & 0x7F) << 21) + ((tag[5] & 0x7F) << 14) + ((tag[6] & 0x7F) << 7) + tag[7];
		int buffer[] = new int[31];
		int encoding = mp3.read();	// discard the encoding
		//printIntArray(tag); System.out.println(encoding + " ");
		size--;
		int kept = Math.min(30, size);
		mp3.read(buffer, kept);
		if (tag[0] == 'T') {
			if (bufferMatchesTag(tag, 1, "ALB")) decodeField(album, buffer, kept, encoding);
			else if (bufferMatchesTag(tag, 1, "IT2")) decodeField(title, buffer, kept, encoding);
			else if (bufferMatchesTag(tag, 1, "PE1")) decodeField(performer, buffer, kept, encoding);
		}
		while (kept < size) {
			mp3.read(buffer, Math.min(30, size-kept));
			kept += 30;
		}
		return size + 1 + 10;
	}
	
	void decodeField(int[] field, int[] buffer, int size, int encoding) {
		if (encoding == 0) {
			for (int i=0; i<size; i++) field[i] = buffer[i];
			field[size] = 0;
		} else if (encoding == 1) {
			int j = 0;
			for (int i=2; i<size; i+=2) field[j++] = buffer[i];
			field[j] = 0;
		}
	}
	
	void parseID3v1Field(int[] field, int[] buffer, int n) {
		int i = 0;
		while (buffer[i+n] != 0) {
			field[i] = buffer[n+i];
			i++;
		}
		field[i] = 0;
	}

	
	boolean bufferMatchesTag(int[] buffer, int n, String tag) {
		return ((buffer[n] == tag.charAt(0)) && (buffer[n+1] == tag.charAt(1)) && (buffer[n+2] == tag.charAt(2)));
	}
	
	public static int numberToString(int number, int[] buffer, int n, int padding, int paddingChr) {
		int i = n;
		if (number > 9999) {
			buffer[i++] = '0' + (number / 10000);
			number -= (number / 10000) * 10000;
			paddingChr = '0';
		} else if (padding > 3) buffer[i++] = paddingChr;
		if (number > 999) {
			buffer[i++] = '0' + (number / 1000);
			number -= (number / 1000) * 1000;
			paddingChr = '0';
		} else if (padding > 2) buffer[i++] = paddingChr;
		if (number > 99) {
			buffer[i++] = '0' + (number / 100);
			number -= (number / 100) * 100;
			paddingChr = '0';
		} else if (padding > 1) buffer[i++] = paddingChr;
		if (number > 9) {
			buffer[i++] = '0' + (number / 10);
			number -= (number / 10) * 10;
		} else if (padding > 0) buffer[i++] = paddingChr;
		buffer[i++] = '0' + number;
		return i-n;
	}

}
