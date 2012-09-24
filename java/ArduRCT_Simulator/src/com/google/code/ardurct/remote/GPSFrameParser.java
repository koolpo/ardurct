package com.google.code.ardurct.remote;

import java.util.Date;

public class GPSFrameParser extends Radio {
	
	boolean gpsIsInitialized = false; 
	boolean gpsHasChanged = false;
	float gpsInitialLatitude;
	float gpsInitialLongitude;
	float gpsInitialAltitude;
	
	float gpsDistance;
	float gpsSpeed;
	float gpsAltitude;
	float gpsLongitude;
	float gpsLatitude;
	float gpsHeight;
	boolean gpsHasFix = false;

	/*	
	 * 	$GPGGA,time[hhmmss.sss],latitude[ddmm.mmmmmm],N,longitude[dddmm.mmmmmm],E,fix[n],satellites[n],
	 *		hdop[nn.nn],altitude[nn.nnn],M,geoidal,M,separation,station_id*hh<CR><LF>
	 *
	 *	$GPVTG,cog[nn.nn],T,,M,x.xxx,N,nn.nnn,K,A*hh<CR><LF>			
	 */
	public void gpsParseData(int[] frame) {
		int i = 0;
		int tokens = 0;
		if (frame[2] == 'G') {	// GGA frame
			// check if the frame is valid
			while (frame[i] != '\n') {
				if (frame[i] == ',') tokens ++;
				if (tokens == 6) break;
				i++;
			}
			if (frame[i+1] == '0') return;	// no fix
			gpsParseGGAFrame(frame);
			if (!gpsIsInitialized) {
				gpsInitialLatitude = gpsLatitude;
				gpsInitialLongitude = gpsLongitude;
				gpsInitialAltitude = gpsAltitude;
			}
		} else if (frame[2] == 'V') {
			// check if the frame is valid
			while (frame[i] != '\n') {
				if (frame[i] == ',') tokens ++;
				if (tokens == 7) break;
				i++;
			}
			if (frame[i+1] != 'A') return;	// no fix
			gpsParseVTGFrame(frame);
		}
		gpsHeight = gpsAltitude - gpsInitialAltitude;
	}

	/*	
	 * 	$GPGGA,time[hhmmss.sss],latitude[ddmm.mmmmmm],N,longitude[dddmm.mmmmmm],E,fix[n],satellites[n],
	 *		hdop[nn.nn],altitude[nn.nnn],M,geoidal,M,separation,station_id*hh<CR><LF>
	 */
	public void gpsParseGGAFrame(int[] frame) {
		int i = 0;
		int tokens = 0;
		while (frame[i] != '\n') {
			if (frame[i] == ',') tokens ++;
			if (tokens == 1) gpsLatitude = 1;
		}		
	}

	/*	
	 *	$GPVTG,cog[nn.nn],T,,M,x.xxx,N,nn.nnn,K,A*hh<CR><LF>			
	 */
	public void gpsParseVTGFrame(int[] frame) {
		
	}

	public float gpsParseFloat(int[] frame, int n) {
		float value = 0;
		float under;
		while (frame[n] != ',') {
			
		}
		return value;
	}
}
