package com.google.code.ardurct;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.UIManager;

import com.google.code.ardurct.ArduRCSimulator;
import com.google.code.ardurct.RemotuinoSimulator;
import com.google.code.ardurct.XBeeSimulator;

public class ArduRCTSimulator extends JFrame
implements ActionListener {
	
	private static final long serialVersionUID = -8128231225228032474L;
	
	XBeeSimulator xBee1;
	XBeeSimulator xBee2;
	XBeeSimulator nullXBee;
	XBeeSimulator localXBee;
	XBeeSimulator remoteXBee;
	ArduRCSimulator arduRC;
	RemotuinoSimulator remotuino;
	
	
	ArduRCTSimulator(String title) {
		super(title);
		xBee1 = XBeeSimulator.createAndShowInstance("XBee ONE", "ABCD", "A87B43CD67AD0235", 19200);
		xBee2 = XBeeSimulator.createAndShowInstance("XBee TWO", "ABCD", "B79E12F287CE5431", 19200);
		xBee1.hardware.setDHDL("B79E12F287CE5431");
		xBee2.hardware.setDHDL("A87B43CD67AD0235");		
		nullXBee = new XBeeSimulator("XBee Null", "0000", "0000000000000000", 19200);
		xBee1.hardware.start();
		xBee2.hardware.start();
		xBee1.hardware.connect(xBee2.hardware);
		xBee2.hardware.connect(xBee1.hardware);

		arduRC = ArduRCSimulator.createAndShowInstance();
		arduRC.xBee = xBee1;
		remotuino = RemotuinoSimulator.createAndShowInstance();
		remotuino.xBee = xBee2;
	}

	public void organizeWindows() {
		setLocation(0, 0);
		nullXBee.setLocation(0, xBee1.getHeight());
		arduRC.setLocation(xBee1.getX()+xBee1.getWidth(), 0);
		remotuino.setLocation(arduRC.getX()+arduRC.getWidth(), 0);
		xBee2.setLocation(remotuino.getX(), remotuino.getY() + remotuino.getHeight());
		xBee1.setLocation(0, getY() + getHeight());
	}
	
	private void buildUI() {	
		//Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Setup");
		ButtonGroup group = new ButtonGroup();
		JRadioButtonMenuItem rbMenuItem = new JRadioButtonMenuItem("Local = XBee ONE, Remote = XBee TWO");
		group.add(rbMenuItem);
		rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Local = XBee TWO, Remote = XBee ONE");
		group.add(rbMenuItem);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Local = , Remote = ");
		group.add(rbMenuItem);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);

		menuBar.add(menu);		
		this.setJMenuBar(menuBar);
	}
		
	public void actionPerformed(ActionEvent ae) {
		if (ae.getActionCommand().equals("Local = , Remote = ")) {
			arduRC.xBee = nullXBee;
			remotuino.xBee = nullXBee;
		} else if (ae.getActionCommand().equals("Local = XBee ONE, Remote = XBee TWO")) {
			arduRC.xBee = xBee1;
			remotuino.xBee = xBee2;
		} else if (ae.getActionCommand().equals("Local = XBee TWO, Remote = XBee ONE")) {
			arduRC.xBee = xBee2;
			remotuino.xBee = xBee1;
		}
	}
	
	private static ArduRCTSimulator createAndShowInstance() {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		ArduRCTSimulator simulator = new ArduRCTSimulator("Touchscreen");
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);	
				
		return simulator;
    }

	public static void main(final String args[]) {
		ArduRCTSimulator arduRCT = ArduRCTSimulator.createAndShowInstance();
		arduRCT.organizeWindows();
	}

}
