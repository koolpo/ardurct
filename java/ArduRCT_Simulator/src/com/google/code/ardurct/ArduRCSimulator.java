package com.google.code.ardurct;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.imageio.ImageIO;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.UIManager;

import com.google.code.ardurct.hardware.RotaryEncoder;
import com.google.code.ardurct.hardware.Joystick;
import com.google.code.ardurct.hardware.RockerSwitch;
import com.google.code.ardurct.hardware.Switch;
import com.google.code.ardurct.hardware.TFTTouchPanel;
import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.libraries.touscruino.ITouscruinoDefines;
import com.google.code.ardurct.local.ArduRCT_LocalV1;

public class ArduRCSimulator extends JFrame
implements ActionListener, ITouscruinoDefines, IArduRCTDefines {

	public static final boolean HAS_ROCKER_SWITCH = true;

	private static final long serialVersionUID = -8128231225228032474L;
	
	TFTTouchPanel graphicsPanel;
	HardwareSerial serial;
	XBeeSimulator xBee = new XBeeSimulator(null, null, null, 9600);
	
	ArduRCT_LocalV1 application;

	int imgIndex = 0;
	
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

		serial = new HardwareSerial("Serial");		
		graphicsPanel = new TFTTouchPanel();

		this.setLayout(new BorderLayout(2, 2));
		JPanel switchPanel = new JPanel();
		if (HAS_ROCKER_SWITCH) {
			switchPanel.setLayout(new BorderLayout(2, 2));
			switchPanel.add(new RockerSwitch(TOUSCRUINO_UP, TOUSCRUINO_MENU, TOUSCRUINO_ENTER, TOUSCRUINO_RIGHT, TOUSCRUINO_DOWN),BorderLayout.CENTER);
		} else {
			switchPanel.setLayout(new BoxLayout(switchPanel, BoxLayout.X_AXIS));
			switchPanel.add(new Switch(TOUSCRUINO_MENU, "Menu"));
			switchPanel.add(new Switch(TOUSCRUINO_DOWN, "Down"));
			switchPanel.add(new Switch(TOUSCRUINO_UP, "Up"));
			switchPanel.add(new Switch(TOUSCRUINO_ENTER, "Enter"));
		}

		JPanel centerCenterPanel = new JPanel();
		centerCenterPanel.setLayout(new BorderLayout(3, 3));
		centerCenterPanel.add(graphicsPanel, BorderLayout.CENTER);
		centerCenterPanel.add(switchPanel, BorderLayout.SOUTH);

		JPanel centerLeft = new JPanel();
		centerLeft.setLayout(new BorderLayout(3, 3));
		centerLeft.add(new RotaryEncoder(ENCODER_A, ENCODER_B), BorderLayout.NORTH);
		centerLeft.add(new Joystick(ROLL, SPEED, "H:Yaw V:Speed"), BorderLayout.CENTER);

		JPanel centerRight = new JPanel();
		centerRight.setLayout(new BorderLayout(3, 3));
		JPanel centerRightTop = new JPanel();
		centerRightTop.setPreferredSize(new Dimension(RotaryEncoder.ENCODER_SIZE+RotaryEncoder.ENCODER_MARGIN, RotaryEncoder.ENCODER_SIZE+RotaryEncoder.ENCODER_MARGIN));
		centerRight.add(centerRightTop, BorderLayout.NORTH);
		centerRight.add(new Joystick(YAW, PITCH, "H:Roll V:Pitch"), BorderLayout.CENTER);
		
		JPanel center = new JPanel();
		center.setLayout(new BorderLayout(3, 3));
		center.add(centerRight, BorderLayout.EAST);
		center.add(centerLeft, BorderLayout.WEST);
		center.add(centerCenterPanel, BorderLayout.CENTER);
		
		this.add(center, BorderLayout.CENTER);
		this.add(serial, BorderLayout.SOUTH);
	}
		
	public void actionPerformed(ActionEvent ae) {
		if (ae.getActionCommand().equals("Sortie png")) {
			try {
				ImageIO.write(graphicsPanel.getContentImage(), "png", new File("ArduRCT." + imgIndex +".png"));
				imgIndex  ++;
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else if (ae.getActionCommand().equals("Start")) {
			application = new ArduRCT_LocalV1(); 
			application.setHardwareSerial(serial);
			application.setHardwareSerial1(xBee.serial);
			application.start();
		} else if (ae.getActionCommand().equals("Stop")) {
			xBee.serial.clear();
			serial.clear();
			if (application != null) application.stop();
			TFTTouchPanel.initPanel();			
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
		simulator.graphicsPanel.start();		
		return simulator;
    }

	public static void main(final String args[]) {
		ArduRCSimulator.createAndShowInstance();
	}

}
