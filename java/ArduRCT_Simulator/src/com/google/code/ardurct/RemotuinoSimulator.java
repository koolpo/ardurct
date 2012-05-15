package com.google.code.ardurct;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.UIManager;

import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.remote.ArduRCT_Remote;

public class RemotuinoSimulator extends JFrame 
implements ActionListener {
	
	private static final long serialVersionUID = -8128231225228032474L;
	
	public HardwareSerial serial;
	public XBeeSimulator xBee = new XBeeSimulator(null, null, null, 0);
	public ArduRCT_Remote application;
	
	RemotuinoSimulator(String title) {
		super(title);
		serial = new HardwareSerial("Serial");		
	}
	
	private void buildUI() {	
		//Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Actions");
		
		ButtonGroup group2 = new ButtonGroup();
		JRadioButtonMenuItem rbMenuItem = new JRadioButtonMenuItem("Start");
		group2.add(rbMenuItem);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Stop");
		group2.add(rbMenuItem);
		rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);

		menuBar.add(menu);		
		this.setJMenuBar(menuBar);
		
		setLayout(new BorderLayout(2, 2));
		setPreferredSize(new Dimension(350, 300));
		add(serial, BorderLayout.CENTER);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		pack();
		setVisible(true);
		this.add(serial, BorderLayout.CENTER);
	}
		
	public void actionPerformed(ActionEvent ae) {
		if (ae.getActionCommand().equals("Start")) {
			application = new ArduRCT_Remote();
			application.setHardwareSerial(serial);
			application.setHardwareSerial1(xBee.hardware);
			application.start();
		} else if (ae.getActionCommand().equals("Stop")) {
			xBee.hardware.clear();
			serial.clear();
			if (application != null) application.stop();
		}
	}
	
	public static RemotuinoSimulator createAndShowInstance() {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		RemotuinoSimulator simulator = new RemotuinoSimulator("Remotuino");
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);	
				
		return simulator;
    }

	public static void main(final String args[]) {
		RemotuinoSimulator.createAndShowInstance();
	}

}
