package com.google.code.ardurct;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.UIManager;

import com.google.code.ardurct.hardware.XBeeRadio;

public class XBeeSimulator extends JFrame {
	
	private static final long serialVersionUID = -8128231225228032474L;
	
	public XBeeRadio serial;
	
	public XBeeSimulator(String title, String id, String shsl, int aBaudrate) {
		super(title);
		serial = new XBeeRadio(id, shsl, aBaudrate);
		serial.begin(aBaudrate);
	}

	private void buildUI() {	
		this.setLayout(new BorderLayout(2, 2));
		serial.setPreferredSize(new Dimension(340, 200));
		this.add(serial, BorderLayout.CENTER);
	}
		

	public static XBeeSimulator createAndShowInstance(String title, String id, String shsl, int baudrate) {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		XBeeSimulator simulator = new XBeeSimulator(title, id, shsl, baudrate);
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);
		return simulator;
    }

	public static void main(final String[] args) {
		XBeeSimulator localXBee = XBeeSimulator.createAndShowInstance("XBee - Local", "ABCD", "A87B43CD67AD0235", 9600);
		localXBee.serial.setDHDL("B79E12F287CE5431");
		XBeeSimulator remoteXBee = XBeeSimulator.createAndShowInstance("XBee - Remote", "ABCD", "B79E12F287CE5431", 9600);
		remoteXBee.serial.setDHDL("A87B43CD67AD0235");		
		remoteXBee.setLocation(localXBee.getWidth()+5, 0);
		localXBee.serial.start();
		remoteXBee.serial.start();
		localXBee.serial.connect(remoteXBee.serial);
		remoteXBee.serial.connect(localXBee.serial);
	}

}
