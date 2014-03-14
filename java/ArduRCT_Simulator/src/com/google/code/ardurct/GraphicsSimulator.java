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
import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.UIManager;

import com.google.code.ardurct.hardware.RockerSwitch;
import com.google.code.ardurct.hardware.Switch;
import com.google.code.ardurct.hardware.TFTTouchPanel;
import com.google.code.ardurct.libraries.ArduinoFirmware;
import com.google.code.ardurct.libraries.HardwareSerial;
import com.google.code.ardurct.libraries.touscruino.ITouscruinoDefines;
import com.google.code.ardurct.utils.Designer;

public class GraphicsSimulator extends JFrame
implements ActionListener, ITouscruinoDefines {
	
	private static final String[] EXAMPLE_PATHS = {
		".libraries.graphics.examples",
		".libraries.touscruino.examplesV1",
		".libraries.touscruino.examplesV2"
	};
	
	private static final long serialVersionUID = -1L;
	
	public static int GRAPHICS = 1;
	public static int TOUSCRUINO = 2;
	public static boolean HAS_ROCKER_SWITCH = true;
	public static boolean HAS_SWITCHES = true;

	public HardwareSerial serial;
	public ArduinoFirmware example;	
	public TFTTouchPanel graphicsPanel;
	public Designer designer = null;
	
	public int imgIndex = 0;
	
	List<String> exampleClassNames = null;
	String examplePath;
	String exampleName;
	String designerText;
	
	int interfaceType = GRAPHICS;
	boolean interfaceOrientation = TFTTouchPanel.PORTRAIT;
	int interfaceHardware = TFTTouchPanel.SIZE_240x320;
	boolean interfaceHasRocker = HAS_ROCKER_SWITCH;
	
	JPanel framePanel = null;
	
	private void buildUI(int interfaceType, int tftHardwareSize, boolean tftIsLandscape, boolean hasRockerSwitch) {
		int pathNumber = 0;
		if (interfaceType == TOUSCRUINO) pathNumber = interfaceHardware;
		this.examplePath = EXAMPLE_PATHS[pathNumber];
		this.interfaceType = interfaceType;
		if (designer != null) designerText = designer.parser.getText();
		
		String title = (interfaceType == GRAPHICS ? "Graphics" : "Touscruino");
		title += " - " + (tftIsLandscape == TFTTouchPanel.LANDSCAPE ? "Landscape" : "Portrait");
		if (tftHardwareSize == TFTTouchPanel.SIZE_128x160 && interfaceType == TOUSCRUINO) {
			title += " - ";
			if (hasRockerSwitch) title += "Rocker switch";
			else title += "Switches";
		}
		this.setTitle(title);
		
		exampleClassNames = getExampleClasses();	
		if (graphicsPanel != null) graphicsPanel.stop();
		
		//Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("File");
		JMenuItem menuItem = new JMenuItem("Quit");
		menuItem.addActionListener(this);
		menu.add(menuItem);
		menuBar.add(menu);		
		
		ButtonGroup iGroup = new ButtonGroup();
		menu = new JMenu("Interface");
		JRadioButtonMenuItem rbMenuItem = new JRadioButtonMenuItem("Graphics 128x160");
		iGroup.add(rbMenuItem);
		if (tftHardwareSize == TFTTouchPanel.SIZE_128x160 && interfaceType == GRAPHICS) rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Graphics 240x320");
		iGroup.add(rbMenuItem);
		if (tftHardwareSize == TFTTouchPanel.SIZE_240x320 && interfaceType == GRAPHICS) rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);		
		rbMenuItem = new JRadioButtonMenuItem("Touscruino V1 - Rocker switch");
		iGroup.add(rbMenuItem);
		if (tftHardwareSize == TFTTouchPanel.SIZE_128x160 && interfaceType == TOUSCRUINO && hasRockerSwitch) rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Touscruino V1 - Switches");
		iGroup.add(rbMenuItem);
		if (tftHardwareSize == TFTTouchPanel.SIZE_128x160 && interfaceType == TOUSCRUINO && !hasRockerSwitch) rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Touscruino V2");
		iGroup.add(rbMenuItem);
		if (tftHardwareSize == TFTTouchPanel.SIZE_240x320 && interfaceType == TOUSCRUINO) rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);

		menu.addSeparator();
		ButtonGroup oGroup = new ButtonGroup();
		rbMenuItem = new JRadioButtonMenuItem("Landscape");
		oGroup.add(rbMenuItem);
		if (tftIsLandscape) rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		rbMenuItem = new JRadioButtonMenuItem("Portrait");
		oGroup.add(rbMenuItem);
		if (!tftIsLandscape) rbMenuItem.setSelected(true);
		rbMenuItem.addActionListener(this);		
		menu.add(rbMenuItem);
		menuBar.add(menu);		

		menu = new JMenu("Examples");
		ButtonGroup eGroup = new ButtonGroup();
		for (String className : exampleClassNames) {
			rbMenuItem = new JRadioButtonMenuItem(className);
			if (className.equals("BasicExample")) rbMenuItem.setSelected(true);
			eGroup.add(rbMenuItem);
			rbMenuItem.addActionListener(this);
			menu.add(rbMenuItem);
		}
		menu.addSeparator();
		rbMenuItem = new JRadioButtonMenuItem("Designer");
		eGroup.add(rbMenuItem);
		rbMenuItem.addActionListener(this);
		menu.add(rbMenuItem);
		menuBar.add(menu);		

		menu = new JMenu("Tools");
		menuItem = new JMenuItem("Sortie png");
		menuItem.addActionListener(this);
		menu.add(menuItem);
		menuBar.add(menu);		

		this.setJMenuBar(menuBar);

		TFTTouchPanel.setHardwareSize(tftHardwareSize);
		TFTTouchPanel.setOrientation(tftIsLandscape);
		graphicsPanel = new TFTTouchPanel();
		
		JPanel switchPanel = new JPanel();
		if (tftHardwareSize == TFTTouchPanel.SIZE_128x160 && interfaceType == TOUSCRUINO && hasRockerSwitch) {
			switchPanel.setLayout(new BorderLayout(2, 2));
			switchPanel.add(new RockerSwitch(TOUSCRUINO_UP, TOUSCRUINO_MENU, TOUSCRUINO_ENTER, TOUSCRUINO_RIGHT, TOUSCRUINO_DOWN),BorderLayout.CENTER);
		} else if (tftHardwareSize == TFTTouchPanel.SIZE_128x160 && interfaceType == TOUSCRUINO && tftIsLandscape) {
			switchPanel.setLayout(new BoxLayout(switchPanel, BoxLayout.Y_AXIS));
			switchPanel.add(new Switch(TOUSCRUINO_ENTER, "Enter"));
			switchPanel.add(new Switch(TOUSCRUINO_UP, "Up"));
			switchPanel.add(new Switch(TOUSCRUINO_DOWN, "Down"));
			switchPanel.add(new Switch(TOUSCRUINO_MENU, "Menu"));
		} else if (tftHardwareSize == TFTTouchPanel.SIZE_128x160 && interfaceType == TOUSCRUINO) {
			switchPanel.setLayout(new BoxLayout(switchPanel, BoxLayout.X_AXIS));
			switchPanel.add(new Switch(TOUSCRUINO_MENU, "Menu"));
			switchPanel.add(new Switch(TOUSCRUINO_DOWN, "Down"));
			switchPanel.add(new Switch(TOUSCRUINO_UP, "Up"));
			switchPanel.add(new Switch(TOUSCRUINO_ENTER, "Enter"));
		}
		JPanel center = new JPanel();
		center.setLayout(new BorderLayout(3, 3));
		if (tftIsLandscape) center.add(switchPanel, BorderLayout.EAST);
		else center.add(switchPanel, BorderLayout.SOUTH);
		center.add(graphicsPanel, BorderLayout.CENTER);

		serial = new HardwareSerial("Serial");

		if (framePanel == null) {
			framePanel = new JPanel();
			this.add(framePanel);
		} else {
			framePanel.removeAll();
		}
		framePanel.setLayout(new BorderLayout(2, 2));
		framePanel.add(center, BorderLayout.CENTER);
		framePanel.add(serial, BorderLayout.SOUTH);
		
		designer = Designer.createInstance(getTitle().startsWith("Graphics")? "graphics" : "Touscruino", graphicsPanel);
		if (designerText != null) designer.parser.setText(designerText);
		
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.pack();
		this.setVisible(true);
		this.graphicsPanel.start();
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
		boolean interfaceChanged = false;
		if (ae.getActionCommand().equals("Quit")) {
			System.exit(0);
		} else if (ae.getActionCommand().equals("Sortie png")) {
			try {
				ImageIO.write(graphicsPanel.getContentImage(), "png", new File(exampleName + "." + imgIndex +".png"));
				imgIndex ++;
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else if (ae.getActionCommand().equals("Designer")) {
			if (example != null) example.stop();
			try { Thread.sleep(10); } catch (Exception e) { }
			example = null;
			designer.setLocation(getWidth(), 0);
			designer.setVisible(true);
			serial.begin(HardwareSerial.DEFAULT_BAUD_RATE);
			serial.println("Select an item in the list and\nclick on the screen to place it");
			designer.start();
		} else if (ae.getActionCommand().equals("Graphics 128x160")) {
			if (interfaceType != GRAPHICS || interfaceHardware != TFTTouchPanel.SIZE_128x160) {
				interfaceHardware = TFTTouchPanel.SIZE_128x160;
				interfaceType = GRAPHICS;
				interfaceChanged = true;
			}
		} else if (ae.getActionCommand().equals("Graphics 240x320")) {
			if (interfaceType != GRAPHICS || interfaceHardware != TFTTouchPanel.SIZE_240x320) {
				interfaceHardware = TFTTouchPanel.SIZE_240x320;
				interfaceType = GRAPHICS;
				interfaceChanged = true;
			}
		} else if (ae.getActionCommand().equals("Touscruino V1 - Rocker switch")) {
			if (interfaceType != TOUSCRUINO || interfaceHardware != TFTTouchPanel.SIZE_128x160 || interfaceHasRocker != HAS_ROCKER_SWITCH) {
				interfaceHardware = TFTTouchPanel.SIZE_128x160;
				interfaceType = TOUSCRUINO;
				interfaceHasRocker = HAS_ROCKER_SWITCH;
				interfaceChanged = true;
			}
		} else if (ae.getActionCommand().equals("Touscruino V1 - Switches")) {
			if (interfaceType != TOUSCRUINO || interfaceHardware != TFTTouchPanel.SIZE_128x160 || interfaceHasRocker == HAS_ROCKER_SWITCH) {
				interfaceHardware = TFTTouchPanel.SIZE_128x160;
				interfaceType = TOUSCRUINO;
				interfaceHasRocker = !HAS_ROCKER_SWITCH;
				interfaceChanged = true;
			}
		} else if (ae.getActionCommand().equals("Touscruino V2")) {
			if (interfaceType != TOUSCRUINO || interfaceHardware != TFTTouchPanel.SIZE_240x320) {
				interfaceHardware = TFTTouchPanel.SIZE_240x320;
				interfaceType = TOUSCRUINO;
				interfaceChanged = true;
			}
		} else if (ae.getActionCommand().equals("Landscape")) {
			if (interfaceOrientation != TFTTouchPanel.LANDSCAPE) {
				interfaceOrientation = TFTTouchPanel.LANDSCAPE;
				interfaceChanged = true;
			}
		} else if (ae.getActionCommand().equals("Portrait")) {
			if (interfaceOrientation != TFTTouchPanel.PORTRAIT) {
				interfaceOrientation = TFTTouchPanel.PORTRAIT;
				interfaceChanged = true;
			}
		} else {
			if (example != null) example.stop();
			if (designer != null) designer.stop();
			try { Thread.sleep(10); } catch (Exception e) { }
			TFTTouchPanel.cancelHighlight();
			startExample(ae.getActionCommand());
		}
		if (interfaceChanged) {
			if (designer != null) designer.stop();
			try { Thread.sleep(10); } catch (Exception e) { }
			TFTTouchPanel.cancelHighlight();
			designer.setVisible(false);
			buildUI(interfaceType, interfaceHardware, interfaceOrientation == TFTTouchPanel.LANDSCAPE, interfaceHasRocker);
			startExample("BasicExample");
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
	
	protected static GraphicsSimulator createAndShowInstance(int interfaceType, int tftHardwareSize, boolean tftIsLandscape, 
			boolean hasRockerSwitch) {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		GraphicsSimulator simulator = new GraphicsSimulator();
		simulator.buildUI(interfaceType, tftHardwareSize, tftIsLandscape, hasRockerSwitch);
		return simulator;
    }

	public static void main(final String[] args) {
		GraphicsSimulator simulator = createAndShowInstance(GRAPHICS, TFTTouchPanel.SIZE_240x320, TFTTouchPanel.PORTRAIT, !HAS_ROCKER_SWITCH);
		simulator.startExample("BasicExample");
	}

}
