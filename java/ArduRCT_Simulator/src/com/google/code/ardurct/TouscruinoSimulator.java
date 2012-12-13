package com.google.code.ardurct;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.UIManager;

import com.google.code.ardurct.hardware.Switch;
import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;
import com.google.code.ardurct.libraries.touscruino.examples.BasicExample;

public class TouscruinoSimulator extends JFrame
implements ActionListener, Runnable {
	
	private static final long serialVersionUID = -8128231225228032474L;
	
	public final static String EXAMPLES_PATH = ".libraries.touscruino.examples"; 

	public HardwareSerial serial;
	public TouscruinoFirmware example;	
	public GraphicsPanel graphicsPanel;
	
	List<String> exampleClassNames = null;
	String exampleName = "BasicExample";
	
	TouscruinoSimulator(String title) {
		super(title);
		exampleClassNames = getExampleClasses();
	}

	private void buildUI() {	
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
		menuBar.add(menu);		
		this.setJMenuBar(menuBar);

		graphicsPanel = new GraphicsPanel();
		serial = new HardwareSerial("Serial");
		
		JPanel centerSouth = new JPanel();
		centerSouth.add(new Switch(14, "Menu"),BorderLayout.NORTH);
		centerSouth.add(new Switch(15, "Up"),BorderLayout.WEST);
		centerSouth.add(new Switch(16, "Down"),BorderLayout.CENTER);
		centerSouth.add(new Switch(17, "Right"),BorderLayout.EAST);
		centerSouth.add(new Switch(18, "Enter"),BorderLayout.SOUTH);
		this.setLayout(new BorderLayout(2, 2));
		JPanel center = new JPanel();
		center.setPreferredSize(new Dimension(300, 400));
		center.setLayout(new BorderLayout(3, 3));
		center.add(new JLabel(" "), BorderLayout.NORTH);
		center.add(centerSouth, BorderLayout.SOUTH);
		center.add(new JLabel("   "), BorderLayout.EAST);
		center.add(new JLabel("   "), BorderLayout.WEST);
		center.add(graphicsPanel, BorderLayout.CENTER);
		this.add(center, BorderLayout.CENTER);
		this.add(serial, BorderLayout.SOUTH);
	}
		
	private List<String> getExampleClasses() {
		ClassLoader classLoader = TouscruinoSimulator.class.getClassLoader();
		assert classLoader != null;
		String packageName = TouscruinoSimulator.class.getPackage().getName() + EXAMPLES_PATH;
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
				ImageIO.write((BufferedImage)graphicsPanel.getScreenPhoto(), "png", new File(exampleName+".png"));	
			} catch (Exception e) {
				e.printStackTrace();
			}
			return;
		}
		exampleName = ae.getActionCommand();		
		ClassLoader classLoader = TouscruinoSimulator.class.getClassLoader();
		assert classLoader != null;
		example.stop();
		// try with the filename as a class
		ArduRCT_JAVA.setPortraitOrientation();
		try {
			String className = TouscruinoSimulator.class.getPackage().getName() + EXAMPLES_PATH + "." + exampleName;
			example = (TouscruinoFirmware)classLoader.loadClass(className).newInstance();
		} catch (Exception e) {		
			example = null;
		}
		// try with the directory name
		if (example == null) {
			try {
				String className = TouscruinoSimulator.class.getPackage().getName() + EXAMPLES_PATH + "." + exampleName + "." + exampleName;
				example = (TouscruinoFirmware)classLoader.loadClass(className).newInstance();
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
	
	private static TouscruinoSimulator createAndShowInstance() {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		TouscruinoSimulator simulator = new TouscruinoSimulator("TouScruino");
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);
		simulator.graphicsPanel.start();
		return simulator;
    }

	public static void main(final String[] args) {
		TouscruinoSimulator simulator = createAndShowInstance();
		simulator.example = new BasicExample();
		simulator.example.setHardwareSerial(simulator.serial);
		simulator.example.start();
	}

}
