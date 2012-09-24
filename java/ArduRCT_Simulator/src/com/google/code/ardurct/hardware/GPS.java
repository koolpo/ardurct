package com.google.code.ardurct.hardware;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.google.code.ardurct.libraries.HardwareSerial;

public class GPS implements Runnable {

	public DateFormat timeFormater = new SimpleDateFormat("HHmmss.SSS");
	
	public int baudrate = 9600;
	public int framesPerSecond = 1;
	
	double latitude;
	double longitude;
	double altitude;
	double speed;
	boolean hasFix;
	
	HardwareSerial serial;
	int loopCounter;
	boolean running;
	boolean moving;
	
	Thread t = null;
	
	public GPS(HardwareSerial aSerial) {
		serial = aSerial;
		t = new Thread(this);
		t.start();
		running = true;
		moving = false;
		latitude = 45*Math.random();
		longitude = 90*Math.random();;
		altitude = 20*Math.random();
		speed = 10*Math.random();
		hasFix = false;
		loopCounter = 0;
	}
	
	public void setMoving(boolean isMoving) {
		moving = isMoving;
	}
	
	public void stop() {
		running = false;
		hasFix = false;
		if (t != null) t.interrupt();
	}
	
	public void run() {
		while (running) {
			try {
				Thread.sleep(1000/framesPerSecond);
				if (serial != null) {
					sendGPGGA();
					sendGPVTG();
				}
				loopCounter ++;
				if (((loopCounter % 3) == 0) && moving) adjustValues();
				if (loopCounter == framesPerSecond * 20) {
					hasFix = true;
					loopCounter = 0;
				}
			} catch (InterruptedException e) {
				return;
			}
		}
	}


	/*	
	 * 	$GPGGA,time[hhmmss.sss],latitude[ddmm.mmmmmm],N,longitude[dddmm.mmmmmm],E,fix[n],satellites[n],
	 *		hdop[nn.nn],altitude[nn.nnn],M,geoidal,M,separation,station_id*hh<CR><LF>
	 */
	public void sendGPGGA() {
		double latitudeMin = (latitude-Math.floor(latitude))*60.0;
		double longitudeMin = (longitude-Math.floor(longitude))*60.0;
		serial.toSendQueue.append("$GPGGA," + timeFormater.format(new Date()) + "," + 
				((int)Math.floor(latitude) + (latitudeMin < 10 ? "0" : "") + latitudeMin) + ",N," +
				((int)Math.floor(longitude) + (longitudeMin < 10 ? "0" : "") + longitudeMin) + ",E," + (hasFix ? "1" : "0") + 
				",12,00.00," + altitude + ",M,0,M,0,0*DE\r\n");
	}
	
	/*
	 *	$GPVTG,cog[nn.nn],T,,M,x.xxx,N,nn.nnn,K,A*hh<CR><LF>			
	 */
	public void sendGPVTG() {
		serial.toSendQueue.append("$GPVTG,0.0,T,0,M,0.00,N," + speed + "K," + (hasFix ? "A" : "N") + "*FE\r\n");
	}
	
	public void adjustValues() {
		double adjustment = Math.random() / 100;
		if (latitude + adjustment > 85) latitude -= adjustment;
		else latitude += adjustment;
		adjustment = Math.random() / 100;
		if (longitude + adjustment > 170) longitude -= adjustment;
		else longitude += adjustment;
		adjustment = Math.random() / 10;
		if (altitude + adjustment > 99) altitude -= adjustment;
		else altitude += adjustment;
		adjustment = Math.random() / 10;
		if (speed + adjustment > 99) speed -= adjustment;
		else speed += adjustment;
	}
	
}
