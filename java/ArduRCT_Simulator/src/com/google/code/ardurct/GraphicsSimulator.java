package com.google.code.ardurct;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import javax.imageio.ImageIO;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.UIManager;

import com.google.code.ardurct.hardware.RockerSwitch;
import com.google.code.ardurct.hardware.Switch;
import com.google.code.ardurct.hardware.TFTTouchPanel;
import com.google.code.ardurct.libraries.ArduinoFirmware;
import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.libraries.touscruino.ITouscruinoDefines;
import com.google.code.ardurct.utils.Designer;

public class GraphicsSimulator extends JFrame
implements ActionListener, Runnable, ITouscruinoDefines {
	
	private static final long serialVersionUID = -1L;
	
	public static int TFT_VERSION_1 = 1;
	public static int TFT_VERSION_2 = 2;
	public static boolean LANDSCAPE_ORIENTATION = TFTTouchPanel.LANDSCAPE;
	public static boolean PORTRAIT_ORIENTATION = TFTTouchPanel.PORTRAIT;
	public static boolean HAS_ROCKER_SWITCH = true;
	public static boolean HAS_SWITCHES = true;

	public HardwareSerial serial;
	public ArduinoFirmware example;	
	public TFTTouchPanel graphicsPanel;
	public Designer designer;
	
	public int imgIndex = 0;
	
	List<String> exampleClassNames = null;
	String examplePath;
	String exampleName;
	
	GraphicsSimulator() {
	}
	
	GraphicsSimulator(String title, String examplePath) {
		super(title);
		this.examplePath = examplePath;
		exampleClassNames = getExampleClasses();
		
	}

	private void buildUI(int tftVersion, boolean tftIsLandscape, boolean hasSwitches, boolean hasRockerSwitch) {	
		//Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("File");
		JMenuItem menuItem = new JMenuItem("Sortie png");
		menuItem.addActionListener(this);
		menu.add(menuItem);
		menuBar.add(menu);		
		menu = new JMenu("Examples");
		for (String className : exampleClassNames) {
			menuItem = new JMenuItem(className);
			menuItem.addActionListener(this);
			menu.add(menuItem);
		}
		menu.addSeparator();
		menuItem = new JMenuItem("Designer");
		menuItem.addActionListener(this);
		menu.add(menuItem);
		menuBar.add(menu);		
		this.setJMenuBar(menuBar);

		TFTTouchPanel.setVersion(tftVersion);
		TFTTouchPanel.setOrientation(tftIsLandscape);
		graphicsPanel = new TFTTouchPanel();
		serial = new HardwareSerial("Serial");
		
		JPanel switchPanel = new JPanel();
		if (hasRockerSwitch) {
			switchPanel.setLayout(new BorderLayout(2, 2));
			switchPanel.add(new RockerSwitch(TOUSCRUINO_UP, TOUSCRUINO_MENU, TOUSCRUINO_ENTER, TOUSCRUINO_RIGHT, TOUSCRUINO_DOWN),BorderLayout.CENTER);
		} else if (hasSwitches && tftIsLandscape) {
			switchPanel.setLayout(new BoxLayout(switchPanel, BoxLayout.Y_AXIS));
			switchPanel.add(new Switch(TOUSCRUINO_ENTER, "Enter"));
			switchPanel.add(new Switch(TOUSCRUINO_UP, "Up"));
			switchPanel.add(new Switch(TOUSCRUINO_DOWN, "Down"));
			switchPanel.add(new Switch(TOUSCRUINO_MENU, "Menu"));
		} else if (hasSwitches) {
			switchPanel.setLayout(new BoxLayout(switchPanel, BoxLayout.X_AXIS));
			switchPanel.add(new Switch(TOUSCRUINO_MENU, "Menu"));
			switchPanel.add(new Switch(TOUSCRUINO_DOWN, "Down"));
			switchPanel.add(new Switch(TOUSCRUINO_UP, "Up"));
			switchPanel.add(new Switch(TOUSCRUINO_ENTER, "Enter"));
		}
		this.setLayout(new BorderLayout(2, 2));
		JPanel center = new JPanel();
		center.setLayout(new BorderLayout(3, 3));
		if (tftIsLandscape) center.add(switchPanel, BorderLayout.EAST);
		else center.add(switchPanel, BorderLayout.SOUTH);
		center.add(graphicsPanel, BorderLayout.CENTER);
		this.add(center, BorderLayout.CENTER);
		this.add(serial, BorderLayout.SOUTH);
		
		designer = Designer.createAndShowInstance(getTitle(), graphicsPanel);
	}
		
	private List<String> getExampleClasses() {
		ClassLoader classLoader = GraphicsSimulator.class.getClassLoader();
		assert classLoader != null;
		String packageName = GraphicsSimulator.class.getPackage().getName() + examplePath;
		String path = packageName.replace('.', '/');
		Enumeration<URL> resources;
		try {
			resources = classLoader.getResources(path);
			List<String> files = new ArrayList<String>();
			if (resources.hasMoreElements()) {
			    URL resource = resources.nextElement();
		        File[] fileNames = (new File(resource.getFile())).listFiles();
		        for (int i=0; i<fileNames.length; i++) {
		        	if (fileNames[i].isDirectory() && !fileNames[i].getName().startsWith(".")) files.add(fileNames[i].getName());
		        	// only take classes and not inner classes
		        	else if (fileNames[i].getName().endsWith(".class") && fileNames[i].getName().indexOf('$') == -1) {
		        		files.add(fileNames[i].getName().substring(0, fileNames[i].getName().length() - 6));
		        	}
		        }
			}
			return files;
		} catch (IOException e) {
			return null;
		}
	}

	public void actionPerformed(ActionEvent ae) {
		if (ae.getActionCommand().equals("Sortie png")) {
			try {
				ImageIO.write(graphicsPanel.getContentImage(), "png", new File(exampleName + "." + imgIndex +".png"));
				imgIndex ++;
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else if (ae.getActionCommand().equals("Designer")) {
			if (example != null) example.stop();
			example = null;
			designer.setLocation(getWidth(), 0);
			designer.setVisible(true);
			serial.begin(HardwareSerial.DEFAULT_BAUD_RATE);
			serial.println("Select an item in the list and\nclick on the screen to place it");
			designer.start();
		} else {
			if (example != null) example.stop();
			startExample(ae.getActionCommand());
		}
	}
	
	public void startExample(String name) {
		exampleName = name;
		ClassLoader classLoader = GraphicsSimulator.class.getClassLoader();
		// try with the filename as a class
		try {
			String className = GraphicsSimulator.class.getPackage().getName() + examplePath + "." + exampleName;
			example = (ArduinoFirmware)classLoader.loadClass(className).newInstance();
		} catch (Exception e) {		
			example = null;
		}
		// try with the directory name
		if (example == null) {
			try {
				String className = GraphicsSimulator.class.getPackage().getName() + examplePath + "." + exampleName + "." + exampleName;
				example = (ArduinoFirmware)classLoader.loadClass(className).newInstance();
			} catch (Exception e) {		
				example = null;
			}			
		}
		serial.begin(HardwareSerial.DEFAULT_BAUD_RATE);
		example.setHardwareSerial(serial);
		example.start();
	}

	public void run() {
		
	}
	
	protected static GraphicsSimulator createAndShowInstance(String title, String examplePath, int tftVersion, boolean tftIsLandscape, 
			boolean hasSwitches, boolean hasRockerSwitch) {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		GraphicsSimulator simulator = new GraphicsSimulator(title, examplePath);
		simulator.buildUI(tftVersion, tftIsLandscape, hasSwitches, hasRockerSwitch);
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);
		simulator.graphicsPanel.start();
		return simulator;
    }

	public static void main(final String[] args) {
		GraphicsSimulator simulator = createAndShowInstance("graphics", ".libraries.graphics.examples", 
				TFT_VERSION_2, PORTRAIT_ORIENTATION, !HAS_SWITCHES, !HAS_ROCKER_SWITCH);
		simulator.startExample("BasicExample");
	}

}
