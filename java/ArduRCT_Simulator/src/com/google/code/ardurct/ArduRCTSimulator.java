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
import com.google.code.ardurct.libraries.touchscreen.IDefines;
import com.google.code.ardurct.libraries.touchscreen.TouchScreen;
import com.google.code.ardurct.local.ArduRCT_Local;
import com.google.code.ardurct.remote.ArduRCT_Remote;

public class ArduRCTSimulator extends JFrame
implements ActionListener {
	
	private static final long serialVersionUID = -8128231225228032474L;
	
	public final static String EXAMPLES_PATH = ".libraries.touchscreen.examples"; 

	public TouchScreen touchscreen;
	public HardwareSerial serial;
	XBeeSimulator xBee1;
	XBeeSimulator xBee2;
	XBeeSimulator nullXBee;
	XBeeSimulator localXBee;
	XBeeSimulator remoteXBee;
	public HardwareSerial remoteSerial;
	JFrame remote;
	
	public ArduRCT_Local localApplication;
	public ArduRCT_Remote remoteApplication;
	
	ArduRCTSimulator(String title) {
		super(title);
		xBee1 = XBeeSimulator.createAndShowInstance("XBee ONE", "ABCD", "A87B43CD67AD0235", 19200);
		xBee2 = XBeeSimulator.createAndShowInstance("XBee TWO", "ABCD", "B79E12F287CE5431", 19200);
		nullXBee = XBeeSimulator.createAndShowInstance("XBee Null", "0000", "0000000000000000", 19200);
		xBee1.hardware.start();
		xBee2.hardware.start();
		xBee1.hardware.connect(xBee2.hardware);
		xBee2.hardware.connect(xBee1.hardware);
		localXBee = nullXBee;
		remoteXBee = nullXBee;
		
		touchscreen = new TouchScreen();
		serial = new HardwareSerial("Serial");		
		
		remoteSerial = new HardwareSerial("Serial");
		remote = new JFrame("Remote");
		remote.setLayout(new BorderLayout(2, 2));
		remote.setPreferredSize(new Dimension(350, 240));
		remote.add(remoteSerial, BorderLayout.CENTER);
		remote.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		remote.pack();
		remote.setVisible(true);	
	}

	public void organizeWindows() {
		nullXBee.setLocation(0, 0);
		xBee1.setLocation(0, nullXBee.getHeight());
		setLocation(nullXBee.getWidth(), 0);
		touchscreen.fillScreen(IDefines.BLACK);
		touchscreen.repaint();
		remote.setLocation(getX()+getWidth(), 0);
		xBee2.setLocation(remote.getX(), remote.getY() + remote.getHeight());
	}
	
	private void buildUI() {	
		//Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Actions");
		JMenuItem menuItem = new JMenuItem("Sortie png");
		menuItem.addActionListener(this);
		menu.add(menuItem);
		menu.addSeparator();
		ButtonGroup group = new ButtonGroup();
		JRadioButtonMenuItem rbMenuItem = new JRadioButtonMenuItem("Local = XBee ONE, Remote = XBee TWO");
		group.add(rbMenuItem);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Local = XBee TWO, Remote = XBee ONE");
		group.add(rbMenuItem);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Local = , Remote = ");
		group.add(rbMenuItem);
		rbMenuItem.setSelected(true);
		menuItem.addActionListener(this);
		menu.add(rbMenuItem);
		
		menu.addSeparator();
		ButtonGroup group2 = new ButtonGroup();
		rbMenuItem = new JRadioButtonMenuItem("Start Local");
		group2.add(rbMenuItem);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Stop Local");
		group2.add(rbMenuItem);
		rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);

		menu.addSeparator();
		ButtonGroup group3 = new ButtonGroup();
		rbMenuItem = new JRadioButtonMenuItem("Start Remote");
		group3.add(rbMenuItem);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Stop Remote");
		group3.add(rbMenuItem);
		rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);

		menuBar.add(menu);		
		this.setJMenuBar(menuBar);

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
		} else if (ae.getActionCommand().equals("Local = , Remote = ")) {
			localXBee = nullXBee;
			remoteXBee = nullXBee;
		} else if (ae.getActionCommand().equals("Local = XBee ONE, Remote = XBee TWO")) {
			localXBee = xBee1;
			remoteXBee = xBee2;
		} else if (ae.getActionCommand().equals("Local = XBee TWO, Remote = XBee ONE")) {
			localXBee = xBee2;
			remoteXBee = xBee1;
		} else if (ae.getActionCommand().equals("Start Local")) {
			localApplication = new ArduRCT_Local(); 
			localApplication.setHardwareSerial(serial);
			localApplication.setTouchscreen(touchscreen);
			localApplication.setHardwareSerial1(localXBee.hardware);
			localApplication.start();
		} else if (ae.getActionCommand().equals("Start Remote")) {
			remoteApplication = new ArduRCT_Remote();
			remoteApplication.setHardwareSerial(remoteSerial);
			remoteApplication.setHardwareSerial1(remoteXBee.hardware);
			remoteApplication.start();
		} else if (ae.getActionCommand().equals("Stop Local")) {
			localXBee.hardware.clear();
			nullXBee.hardware.clear();
			serial.clear();
			if (localApplication != null) localApplication.stop();
			touchscreen.fillScreen(IDefines.BLACK);			
		} else if (ae.getActionCommand().equals("Stop Remote")) {
			remoteXBee.hardware.clear();
			remoteSerial.clear();
			if (remoteApplication != null) remoteApplication.stop();
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
