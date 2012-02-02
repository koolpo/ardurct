package com.google.code.ardurct.lcd_pcd8544;

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

	public LCDGraphicPanel lcdG = new LCDGraphicPanel();
	public LCDTextPanel lcdT = new LCDTextPanel();
	
	List<String> testClassNames = null;
	
	public ILCDTest test;
	
	Simulator(String title) {
		super(title);
		testClassNames = getTestClasses();
	}

	private void buildUI() {	
		//Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Tests");
		for (String className : testClassNames) {
			JMenuItem menuItem = new JMenuItem(className);
			menuItem.addActionListener(this);
			menu.add(menuItem);
		}
		menuBar.add(menu);		
		this.setJMenuBar(menuBar);

		this.setLayout(new GridLayout(1, 2, 10, 10));
		this.add(lcdG);
		this.add(lcdT);
	}
		
	private List<String> getTestClasses() {
		ClassLoader classLoader = Simulator.class.getClassLoader();
		assert classLoader != null;
		String packageName = Simulator.class.getPackage().getName() + ".tests";
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
		String className = Simulator.class.getPackage().getName() + ".tests." + name;
		test.stop();
		lcdG.clear();
		lcdT.clear();
		try {
			test = (ILCDTest)classLoader.loadClass(className).newInstance();
		} catch (Exception e) {			
		}
		test.setDisplays(lcdG, lcdT);
		test.setup();
		test.start();
	}

	private static void createAndShowGUI(String[] args) {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		Simulator simulator = new Simulator("LCDDrawing");
		simulator.buildUI();
		simulator.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		simulator.pack();
		simulator.setVisible(true);
		
		simulator.test = new LCDTest();
		simulator.test.setDisplays(simulator.lcdG, simulator.lcdT);
		simulator.test.setup();
		simulator.test.start();
    }

	public static void main(final String[] args) {
		createAndShowGUI(args);
	}

}
