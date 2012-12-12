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

import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.libraries.touscruino.IArduinoFirmware;
import com.google.code.ardurct.libraries.touscruino.TouchScreen;
import com.google.code.ardurct.libraries.touscruino.examples.BasicExample;

public class TouchscreenSimulator extends JFrame
implements ActionListener {
	
	private static final long serialVersionUID = -8128231225228032474L;
	
	public final static String EXAMPLES_PATH = ".libraries.touchscreen.examples"; 

	public TouchScreen touchscreen;
	public HardwareSerial serial;
	public String exampleName = "BasicExample";
	
	List<String> exampleClassNames = null;
	
	public IArduinoFirmware example;
	
	TouchscreenSimulator(String title) {
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

		touchscreen = new TouchScreen();
		touchscreen.addMouseListener(touchscreen);
		touchscreen.addMouseMotionListener(touchscreen);
		serial = new HardwareSerial("Serial");
		
		this.setLayout(new BorderLayout(2, 2));
		JPanel center = new JPanel();
		center.setPreferredSize(new Dimension(300, 400));
		center.setLayout(new BorderLayout(3, 3));
		center.add(new JLabel(" "), BorderLayout.NORTH);
		center.add(new JLabel(" "), BorderLayout.SOUTH);
		center.add(new JLabel("   "), BorderLayout.EAST);
		center.add(new JLabel("   "), BorderLayout.WEST);
		center.add(touchscreen, BorderLayout.CENTER);
		this.add(center, BorderLayout.CENTER);
		this.add(serial, BorderLayout.SOUTH);
	}
		
	private List<String> getExampleClasses() {
		ClassLoader classLoader = TouchscreenSimulator.class.getClassLoader();
		assert classLoader != null;
		String packageName = TouchscreenSimulator.class.getPackage().getName() + EXAMPLES_PATH;
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
				ImageIO.write((BufferedImage)touchscreen.getScreenPhoto(), "png", new File(exampleName+".png"));	
			} catch (Exception e) {
				e.printStackTrace();
			}
			return;
		}
		exampleName = ae.getActionCommand();		
		ClassLoader classLoader = TouchscreenSimulator.class.getClassLoader();
		assert classLoader != null;
		example.stop();
		// try with the filename as a class
		try {
			String className = TouchscreenSimulator.class.getPackage().getName() + EXAMPLES_PATH + "." + exampleName;
			example = (IArduinoFirmware)classLoader.loadClass(className).newInstance();
		} catch (Exception e) {		
			example = null;
		}
		// try with the directory name
		if (example == null) {
			try {
				String className = TouchscreenSimulator.class.getPackage().getName() + EXAMPLES_PATH + "." + exampleName + "." + exampleName;
				example = (IArduinoFirmware)classLoader.loadClass(className).newInstance();
			} catch (Exception e) {		
				example = null;
			}			
		}
		serial.begin(HardwareSerial.DEFAULT_BAUD_RATE);
		touchscreen.clear();
		example.setTouchscreen(touchscreen);
		example.setHardwareSerial(serial);
		example.start();
	}

	private static TouchscreenSimulator createAndShowInstance() {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		TouchscreenSimulator simulator = new TouchscreenSimulator("Touchscreen");
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);
		return simulator;
    }

	public static void main(final String[] args) {
		TouchscreenSimulator simulator = createAndShowInstance();
		simulator.example = new BasicExample();
		simulator.example.setTouchscreen(simulator.touchscreen);
		simulator.example.start();
	}

}
