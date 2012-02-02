package com.google.code.ardurct.touchscreen;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.UIManager;

public class Simulator  extends JFrame 
implements ActionListener {
	
	private static final long serialVersionUID = -8128231225228032474L;

	public Touchscreen touchscreen = new Touchscreen();
	
	List<String> exampleClassNames = null;
	
	public IExample example;
	
	Simulator(String title) {
		super(title);
		exampleClassNames = getExampleClasses();
	}

	private void buildUI() {	
		//Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Examples");
		for (String className : exampleClassNames) {
			JMenuItem menuItem = new JMenuItem(className);
			menuItem.addActionListener(this);
			menu.add(menuItem);
		}
		menuBar.add(menu);		
		this.setJMenuBar(menuBar);

		this.setLayout(new GridLayout(1, 2, 10, 10));
		this.add(touchscreen);
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
		        	if (fileNames[i].getName().endsWith(".class")) {
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
		String name = ae.getActionCommand();
		ClassLoader classLoader = Simulator.class.getClassLoader();
		assert classLoader != null;
		String className = Simulator.class.getPackage().getName() + ".examples." + name;
		example.stop();
		try {
			example = (IExample)classLoader.loadClass(className).newInstance();
		} catch (Exception e) {			
		}
		example.setTouchscreen(touchscreen);
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
