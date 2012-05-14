package com.google.code.ardurct.hardware;

import com.google.code.ardurct.libraries.HardwareSerial;

public class XBeeHardware extends HardwareSerial implements Runnable {

	private static final long serialVersionUID = -2137339099846032519L;

	public static final int XBEE_DEFAULT_GUARD_TIME = 100;
	public static final int XBEE_DEFAULT_BAUDRATE = 19200;

	public static String commandMode = "+++";
	
	public HardwareSerial serial;
	public String shsl;
	public String dhdl; 
	public String id;
	private XBeeHardware remote = null;
	
	public int hardwareBaudrate;
	
	private int processedLength = 0;
	private boolean isCommandMode = false;

	public XBeeHardware(String anId, String aSHSL, int aHardwareBaudrate) {
		super("");
		id = anId;
		shsl = aSHSL;
		dhdl = "000000000000FFFF";
		hardwareBaudrate = aHardwareBaudrate;
		updateConfiguration();
	}
	
	public void start() {
		Thread processor = new Thread(this);
		processor.start();			
	}
	
	public void clear() {
		super.clear();
		processedLength = 0;
		isCommandMode = false;
	}
	
	public void connect(XBeeHardware aRemote) {
		remote = aRemote;
	}
	
	public void disconnect() {
		remote = null;
	}
	
	public void updateConfiguration() {
		setConfiguration(hardwareBaudrate + "b" + " - " + id + " - " + shsl + " > " + dhdl);
	}
	
	public void run() {
		while (true) {
			try {
				if (inboundQueue.getText().length() - processedLength > 0) {
					if (hardwareBaudrate != getBaudrate()) {
						while (available() > 0) {
							read();
							processedLength++;								
						}
					} else {
						String cmdText = inboundQueue.getText().substring(processedLength).toUpperCase();
						if (isCommandMode) processCommandList(cmdText);
						else {
							// look for first char of commandMode
							int indOf = cmdText.indexOf(commandMode.charAt(0));
							if (indOf != -1) {
								// send all chars up to first char of commandMode
								cmdText = cmdText.substring(indOf);
								for (int i=0; i<indOf; i++) {
									char c = (char)read();
									if (remote != null) remote.print(c);
									processedLength++;
								}
								processCommandList(cmdText);
							} else {
								while (available() > 0) {
									char c = (char)read();
									if (remote != null) remote.print(c);
									processedLength++;								
								}
							}
						}
					}
				}
                Thread.sleep(10);
			} catch (Exception e) { 	
			}		
		}
	}

	private void processCommandList(String cmdText) {
		if (cmdText.length() < 3) return;
		if (!isCommandMode) {
			if (cmdText.charAt(0) == commandMode.charAt(0) && cmdText.charAt(1) == commandMode.charAt(1) && cmdText.charAt(2) == commandMode.charAt(2)) {
				isCommandMode = true;
				print("OK\n");
				// we have a commandMode marker
				for (int i=0; i<3; i++) read();
				processedLength += 3;
				// remove the marker
				cmdText = cmdText.substring(3);
			} else {
				// send the first commandMode marker
				char c = (char)read();
				if (remote != null) remote.print(c);
				processedLength++;
				return;
			}
		}
		int indOf = cmdText.indexOf('\n');
		if (indOf == -1) return;
		// absorb the command
		for (int i=0; i<indOf+1; i++) read();
		processedLength += indOf+1;
		// we are in command mode
		cmdText = cmdText.substring(0, indOf);
		if (cmdText.charAt(indOf-1) == '\r') cmdText = cmdText.substring(0, indOf-1);
		if (!cmdText.startsWith("AT")) {
			print("KO\n");
			isCommandMode = false;
			return;
		}
		cmdText = cmdText.substring(2);
		String cmdList[] = cmdText.replace(',', '§').split("§");		
		for (String cmd : cmdList) {
			if (!processCommand(cmd)) print("KO\n");
		}
	}
	
	private boolean processCommand(String cmd) {	
		if (cmd.startsWith("CN")) {
			isCommandMode = false;
			print("OK\n");
		} else if (cmd.startsWith("ID")) {
			if (cmd.length() > 2) {
				id = (cmd.substring(2) + "000").substring(0, 4);
				updateConfiguration();
				print("OK\n");
			} else print(id + "\n"); 
		} else if (cmd.startsWith("DH")) {
			if (cmd.length() > 2) {
				dhdl = (cmd.substring(2) + "000000000000000").substring(0, 8) + dhdl.substring(8, 16);
				print("OK\n");
				updateConfiguration();
			} else print(dhdl.substring(0, 8) + "\n"); 
		} else if (cmd.startsWith("DL")) {
			if (cmd.length() > 2) {
				dhdl = dhdl.substring(0, 8) + (cmd.substring(2) + "000000000000000").substring(8, 16);
				print("OK\n");
				updateConfiguration();
			} else print(dhdl.substring(8, 16) + "\n"); 
		} else if (cmd.startsWith("SH")) {
			if (cmd.length() > 2) return false;
			else print(shsl.substring(0, 8) + "\n"); 
		} else if (cmd.startsWith("SL")) {
			if (cmd.length() > 2) return false;
			else print(shsl.substring(8, 16) + "\n"); 
		} else if (cmd.startsWith("GT")) {
			print("OK\n");
		} else if (cmd.startsWith("MY")) {
			print("OK\n");
		} else if (cmd.startsWith("WR")) {
			print("OK\n");
		} else if (cmd.startsWith("BD")) {
			try {
				hardwareBaudrate = Integer.parseInt(cmd.substring(2), 16);
				baudrate = hardwareBaudrate;
				updateConfiguration();
				print("OK\n");
			} catch (Exception e) {
				return false;
			}
		}
		return true;
	}
}
