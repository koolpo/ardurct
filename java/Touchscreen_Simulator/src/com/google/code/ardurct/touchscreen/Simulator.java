package com.google.code.ardurct.touchscreen;

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

import com.google.code.ardurct.HardwareSerial;
import com.google.code.ardurct.touchscreen.examples.Example;

public class Simulator extends JFrame
implements ActionListener {
	
	private static final long serialVersionUID = -8128231225228032474L;

	public Touchscreen touchscreen;
	public HardwareSerial serial;
	public String exampleName = "Example";
	
	List<String> exampleClassNames = null;
	
	public IExample example;
	
	Simulator(String title) {
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

		touchscreen = new Touchscreen();
		touchscreen.addMouseListener(touchscreen);
		touchscreen.addMouseMotionListener(touchscreen);
		serial = new HardwareSerial();
		
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
		ClassLoader classLoader = Simulator.class.getClassLoader();
		assert classLoader != null;
		String packageName = Simulator.class.getPackage().getName() + ".examples";
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
		ClassLoader classLoader = Simulator.class.getClassLoader();
		assert classLoader != null;
		example.stop();
		// try with the filename as a class
		try {
			String className = Simulator.class.getPackage().getName() + ".examples." + exampleName;
			example = (IExample)classLoader.loadClass(className).newInstance();
		} catch (Exception e) {		
			example = null;
		}
		// try with the directory name
		if (example == null) {
			try {
				String className = Simulator.class.getPackage().getName() + ".examples." + exampleName + "." + exampleName;
				example = (IExample)classLoader.loadClass(className).newInstance();
			} catch (Exception e) {		
				example = null;
			}			
		}
		serial.clear();
		touchscreen.clear();
		example.setTouchscreen(touchscreen);
		example.setHardwareSerial(serial);
		example.start();
	}

	private static void createAndShowGUI(String[] args) {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		Simulator simulator = new Simulator("Touchscreen");
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);
		
		simulator.example = new Example();
		simulator.example.setTouchscreen(simulator.touchscreen);
		simulator.example.start();
    }

	public static void main(final String[] args) {
		createAndShowGUI(args);
	}

}
