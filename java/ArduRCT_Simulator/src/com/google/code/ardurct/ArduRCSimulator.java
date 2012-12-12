package com.google.code.ardurct;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;
import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.UIManager;

import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.libraries.touscruino.IDefines;
import com.google.code.ardurct.libraries.touscruino.TouchScreen;
import com.google.code.ardurct.local.ArduRCT_Local;

public class ArduRCSimulator extends JFrame
implements ActionListener {
	
	private static final long serialVersionUID = -8128231225228032474L;
	
	public TouchScreen touchscreen;
	public HardwareSerial serial;
	XBeeSimulator xBee = new XBeeSimulator(null, null, null, 9600);
	
	public ArduRCT_Local application;
	
	ArduRCSimulator(String title) {
		super(title);
	}
	
	private void buildUI() {	
		//Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Actions");
		JMenuItem menuItem = new JMenuItem("Sortie png");
		menuItem.addActionListener(this);
		menu.add(menuItem);
		
		menu.addSeparator();
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

		touchscreen = new TouchScreen();
		serial = new HardwareSerial("Serial");				
		touchscreen.addMouseListener(touchscreen);
		touchscreen.addMouseMotionListener(touchscreen);

		this.setLayout(new BorderLayout(2, 2));
		JPanel center = new JPanel();
		center.setPreferredSize(new Dimension(280, 380));
		center.setLayout(new BorderLayout(3, 3));
		center.add(new JLabel(" "), BorderLayout.NORTH);
		center.add(new JLabel(" "), BorderLayout.SOUTH);
		center.add(new JLabel("   "), BorderLayout.EAST);
		center.add(new JLabel("   "), BorderLayout.WEST);
		center.add(touchscreen, BorderLayout.CENTER);
		this.add(center, BorderLayout.CENTER);
		this.add(serial, BorderLayout.SOUTH);
	}
		
	public void actionPerformed(ActionEvent ae) {
		if (ae.getActionCommand().equals("Sortie png")) {
			try {
				ImageIO.write((BufferedImage)touchscreen.getScreenPhoto(), "png", new File("ArduRCT_Local.png"));	
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else if (ae.getActionCommand().equals("Start")) {
			application = new ArduRCT_Local(); 
			application.setHardwareSerial(serial);
			application.setTouchscreen(touchscreen);
			application.setHardwareSerial1(xBee.hardware);
			application.start();
		} else if (ae.getActionCommand().equals("Stop")) {
			xBee.hardware.clear();
			serial.clear();
			if (application != null) application.stop();
			touchscreen.fillScreen(IDefines.BLACK);			
		}
	}
	
	public static ArduRCSimulator createAndShowInstance() {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		ArduRCSimulator simulator = new ArduRCSimulator("ArduRC");
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);	
				
		return simulator;
    }

	public static void main(final String args[]) {
		ArduRCSimulator.createAndShowInstance();
	}

}
