package com.google.code.ardurct;

import java.awt.BorderLayout;
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
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.UIManager;

import com.google.code.ardurct.hardware.TFTTouchPanel;
import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.libraries.graphics.GraphicsFirmware;
import com.google.code.ardurct.libraries.graphics.examples.BasicExample;

public class GraphicsSimulator extends JFrame
implements ActionListener {
	
	private static final long serialVersionUID = -8128231225228032474L;
	
	public final static String EXAMPLES_PATH = ".libraries.graphics.examples"; 

	public HardwareSerial serial;
	public GraphicsFirmware example;	
	public TFTTouchPanel graphicsPanel;
	
	List<String> exampleClassNames = null;
	String exampleName = "BasicExample";
	
	public int imgIndex = 0;

	GraphicsSimulator(String title) {
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

		graphicsPanel = new TFTTouchPanel();
		serial = new HardwareSerial("Serial");
		
		this.setLayout(new BorderLayout(2, 2));
		this.add(graphicsPanel, BorderLayout.CENTER);
		this.add(serial, BorderLayout.SOUTH);
	}
		
	private List<String> getExampleClasses() {
		ClassLoader classLoader = GraphicsSimulator.class.getClassLoader();
		assert classLoader != null;
		String packageName = GraphicsSimulator.class.getPackage().getName() + EXAMPLES_PATH;
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
				ImageIO.write((BufferedImage)graphicsPanel.getContentImage(), "png", new File(exampleName + "." + imgIndex +".png"));
				imgIndex ++;
			} catch (Exception e) {
				e.printStackTrace();
			}
			return;
		}
		exampleName = ae.getActionCommand();		
		ClassLoader classLoader = GraphicsSimulator.class.getClassLoader();
		assert classLoader != null;
		example.stop();
		// try with the filename as a class
		try {
			String className = GraphicsSimulator.class.getPackage().getName() + EXAMPLES_PATH + "." + exampleName;
			example = (GraphicsFirmware)classLoader.loadClass(className).newInstance();
		} catch (Exception e) {		
			example = null;
		}
		// try with the directory name
		if (example == null) {
			try {
				String className = GraphicsSimulator.class.getPackage().getName() + EXAMPLES_PATH + "." + exampleName + "." + exampleName;
				example = (GraphicsFirmware)classLoader.loadClass(className).newInstance();
			} catch (Exception e) {		
				example = null;
			}			
		}
		serial.begin(HardwareSerial.DEFAULT_BAUD_RATE);
		example.setHardwareSerial(serial);
		example.start();
	}

	private static GraphicsSimulator createAndShowInstance() {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		GraphicsSimulator simulator = new GraphicsSimulator("Graphics");
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);
		simulator.graphicsPanel.start();
		return simulator;
    }

	public static void main(final String[] args) {
		GraphicsSimulator simulator = createAndShowInstance();
		simulator.example = new BasicExample();
		simulator.example.setHardwareSerial(simulator.serial);
		simulator.example.start();
	}

}
