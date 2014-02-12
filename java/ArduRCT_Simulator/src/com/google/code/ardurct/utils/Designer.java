package com.google.code.ardurct.utils;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.Hashtable;
import java.util.Vector;

import javax.swing.JComboBox;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.UIManager;
import javax.swing.border.EmptyBorder;

import com.google.code.ardurct.hardware.TFTTouchPanel;
import com.google.code.ardurct.libraries.eventManager.IEventDefines;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIButton;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIElement;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIGauge;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUILabel;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIListItem;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIOption;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUISlider;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUITab;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIValue;
import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.graphics.IGraphicsDefines;
import com.google.code.ardurct.libraries.graphics.IUIDrawCallback;
import com.google.code.ardurct.utils.Parser.CodeParameter;

public class Designer extends JFrame 
implements IGraphicsDefines, IEventDefines, WindowListener, Runnable, ActionListener {

	private static final long serialVersionUID = 6598518643986585440L;

	public static final String MARKER = "ArduRCT-Designer";
	
	/**
uis home sel
uiv 10 40 as as home value 10 0 20
uisl sap bopwm 200 20 home slider 10 0 20
uig sap bopwm 200 20 home gauge 16 0 20
uit 0 0 as 25 home tab1 Tab1 0 t
uiti rop 0 sap sap home tab2 draw 0
uis popup f center 0xF6F6
uil 6 5 as as popup label "Are you really sure you\nwant to exit ?" s
uib 5 bopwm as as popup ok Ok
uib ropwm sap as as popup cancel Cancel
!draw
fr 59 6 33 14 blue
	 */
	
	/*
	 * placer
	 * ------
	 * 	p	point (x and y)
	 * 	d	dimension (delta in x and y from last point)
	 * 	r	radius (distance from last point)
	 * 	x	x of new point
	 * 	y	y of new point
	 * 
	 * parameters
	 * ----------
	 * Xparameter
	 * 	X is the type of parameter
	 * 		a	angle
	 * 		b	boolean
	 * 		c	rgb565 color
	 * 		d	define int (used for #define) 
	 * 		f	font size	
	 * 		g	graphics style
	 * 		i	int
	 * 		n	name
	 * 		N	name as upper case
	 * 		p	pointer (& followed by name)
	 * 		s	screen
	 * 		t	text
	 * 		ta	text as int array
	 * 		u	coordinate
	 * 		&	action handler
	 * 		$	drawing handler
	 * [Xoptional_parameter]default_value
	 * Xparameter]default_value
	 * 	]*	random_value
	 * 	]+	sequential_value
	 */
	public static final String[] DESIGNER_ELEMENTS = {
		// name in list,			placer	mnemonic	parameters																		declaration, function		code generation
		"Delay,						-,		d,		ims,																				delay,						- a ims",	
		"Begin,						-,		beg,	cforeground cbackground ffont]m bfont_bold]T [boverlay]T,							begin,						- g =",
		"Rotate screen,				-,		rot,	aangle,																				setRotation,				- g =",
		"Fill screen,				-,		fs,		ccolor,																				fillScreen,					- g =",
		"Draw pixel,				p,		p,		ix iy ccolor]*,																		drawPixel,					- g =",
		"Draw circle,				pr,		c, 		ix0 iy0 iradius ccolor]* [ithickness]1,												drawCircle,					- g =",
		"Fill circle,				pr,		fc,		ix0 iy0 iradius ccolor]*,															fillCircle,					- g =",
		"Draw string,				p,		s,		ix iy ttext ccolor ffont]m [bfont_bold]F [boverlay]T,								drawString,					- g ttext ix iy ccolor ffont bfont_bold boverlay",
		"Draw rectangle,			pd,		r,		ix iy iwidth iheight ccolor]* [ithickness]1,										drawRectangle,				- g =",
		"Fill rectangle,			pd,		fr,		ix iy iwidth iheight ccolor]*,														fillRectangle,				- g =",
		"Draw rounded_rectangle,	pdr,	rr,		ix iy iwidth iheight iradius ccolor]* [ithickness]1,								drawRoundedRectangle,		- g =",
		"Fill rounded_rectangle, 	pdr,	frr,	ix iy iwidth iheight iradius ccolor]*,												fillRoundedRectangle,		- g =",
		"Draw arc,					pr,		a,		ix0 iy0 iradius aoctant ccolor]* [ithickness]1,										drawArc,					- g =",
		"Fill arc,					pr,		fa,		ix0 iy0 iradius aoctant]nw ccolor]*,												fillArc,					- g =",
		"Draw triangle, 			ppp,	t,		ix1 iy1 ix2 iy2 ix3 iy3 ccolor]* [ithickness]1,										drawTriangle,				- g =",
		"Fill triangle, 			ppp,	ft,		ix1 iy1 ix2 iy2 ix3 iy3 ccolor]*,													fillTriangle,				- g =",
		"Draw line, 				pp,		l,		ix1 iy1 ix2 iy2 ccolor]* [ithickness]1,												drawLine,					- g =",
		"Draw horizontal line, 		px,		hl,		ix1 iy ix2 ccolor]* [ithickness]1,													drawHorizontalLine,			- g ix1 ix2 iy ccolor ithickness",
		"Draw vertical line, 		py,		vl,		ix iy1 iy2 ccolor]* [ithickness]1,													drawVerticalLine,			- g =",
		"Fill corner,				pr,		fco,	ix iy isize adirection]ne ccolor]*,													fillCorner,					- g =",
		"Draw big digit,			pdr,	bd,		ix iy iwidth iheight ithickness idigit]8 ccolor]* [gstyle]A,						drawBigDigit,				- g idigit ix iy iwidth iheight ccolor ithickness gstyle",
		"UI Screen, 				-,		uis, 	nname [bselected]F [aposition]0 [cbackground]WHITE,									ArduRCT_GraphicsUIScreen, 	- s",
		"UI Text button,			pd,		uib,	ux uy uwidth uheight sscreen nname ttext [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUIButton,	# e Nname ttext paction",
		"UI Icon button,			pd,		uibi,	ux uy uwidth uheight sscreen nname $draw [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUIButton,	# e Nname pdraw paction",
		"UI Element,				pd,		uie,	ux uy uwidth uheight sscreen nname $draw [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUIElement,	# e Nname pdraw paction",
		"UI Label,					pd,		uil,	ux uy uwidth uheight sscreen nname ttext ffont]m [usection]MA did]+,				ArduRCT_GraphicsUILabel,	# e Nname ttext ffont",
		"UI Gauge,					pd,		uig,	ux uy uwidth uheight sscreen nname ivalue imin imax [usection]MA did]+,				ArduRCT_GraphicsUIGauge,	# e Nname ivalue imin imax",
		"UI List item,				pd,		uili,	ux uy uwidth uheight sscreen nname ttext [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUIListItem,	# e Nname ttext paction",
		"UI Icon list item,			pd,		uilii,	ux uy uwidth uheight sscreen nname $draw [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUIListItem,	# e Nname pdraw paction",
		"UI Option,					pd,		uio,	ux uy uwidth uheight sscreen nname ttext igroup bselected]F [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUIOption,	# e Nname ttext paction igroup",
		"UI Icon option,			pd,		uioi,	ux uy uwidth uheight sscreen nname $draw igroup bselected]F [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUIOption,	# e Nname pdraw paction igroup",
		"UI Slider,					pd,		uisl,	ux uy uwidth uheight sscreen nname ivalue imin imax istep]1 [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUISlider,	# e Nname ivalue imin imax paction istep",
		"UI Tab,					pd,		uit,	ux uy uwidth uheight sscreen nname ttext igroup [bselected]F [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUITab,	# e Nname ttext paction igroup",
		"UI Icon tab,				pd,		uiti,	ux uy uwidth uheight sscreen nname $draw igroup [bselected]F [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUITab,	# e Nname pdraw paction igroup",
		"UI Value,					pd,		uiv,	ux uy uwidth uheight sscreen nname ivalue imin imax istep]1 iradix]10 [usection]MA did]+ &action]handleAction,	ArduRCT_GraphicsUIValue,	# e Nname ivalue imin imax paction istep iradix",
	};
	
	public ArduRCT_GraphicsUIElement getUIElement(String mnemonic, CodeParameter p[]) {
		if (mnemonic.equals("uib")) return new ArduRCT_GraphicsUIButton(p[8].d, p[6].ta, null);
		if (mnemonic.equals("uibi")) return new ArduRCT_GraphicsUIButton(p[8].d, (IUIDrawCallback)null, null);
		if (mnemonic.equals("uil")) return new ArduRCT_GraphicsUILabel(p[9].d, p[6].ta, p[7].f);
		if (mnemonic.equals("uie")) return new ArduRCT_GraphicsUIElement(p[8].d, null, null);
		if (mnemonic.equals("uig")) return new ArduRCT_GraphicsUIGauge(p[10].d, p[6].i, p[7].i, p[8].i);
		if (mnemonic.equals("uili")) return new ArduRCT_GraphicsUIListItem(p[8].d, p[6].ta, null);
		if (mnemonic.equals("uilii")) return new ArduRCT_GraphicsUIListItem(p[8].d, (IUIDrawCallback)null, null);
		if (mnemonic.equals("uio")) return new ArduRCT_GraphicsUIOption(p[10].d, p[6].ta, null, p[7].i);
		if (mnemonic.equals("uioi")) return new ArduRCT_GraphicsUIOption(p[10].d, (IUIDrawCallback)null, null, p[7].i);
		if (mnemonic.equals("uisl")) return new ArduRCT_GraphicsUISlider(p[11].d, p[6].i, p[7].i, p[8].i, null, p[9].i);
		if (mnemonic.equals("uit")) return new ArduRCT_GraphicsUITab(p[10].d, p[6].ta, null, p[7].i);
		if (mnemonic.equals("uiti")) return new ArduRCT_GraphicsUITab(p[10].d, (IUIDrawCallback)null, null, p[7].i);
		if (mnemonic.equals("uiv")) return new ArduRCT_GraphicsUIValue(p[12].d, p[6].i, p[7].i, p[8].i, null, p[9].i, p[10].i);
		return null;
	}

	public void draw(String mnemonic, CodeParameter p[]) {
		if (mnemonic.equals("beg")) graphics.begin(p[0].c, p[1].c, p[2].f, p[3].b, p[4].b);
		if (mnemonic.equals("rot")) graphics.setRotation(p[0].a);
		if (mnemonic.equals("fs")) graphics.fillScreen(p[0].c);
		if (mnemonic.equals("p")) graphics.drawPixel(p[0].i, p[1].i, p[2].c);
		if (mnemonic.equals("c")) graphics.drawCircle(p[0].i, p[1].i, p[2].i, p[3].c, p[4].i);
		if (mnemonic.equals("fc")) graphics.fillCircle(p[0].i, p[1].i, p[2].i, p[3].c);
		if (mnemonic.equals("s")) graphics.drawString(p[2].t, p[0].i, p[1].i, p[3].c, p[4].f, p[5].b, p[6].b);
		if (mnemonic.equals("r")) graphics.drawRectangle(p[0].i, p[1].i, p[2].i, p[3].i, p[4].c, p[5].i);
		if (mnemonic.equals("fr")) graphics.fillRectangle(p[0].i, p[1].i, p[2].i, p[3].i, p[4].c);
		if (mnemonic.equals("rr")) graphics.drawRoundedRectangle(p[0].i, p[1].i, p[2].i, p[3].i, p[4].i, p[5].c, p[6].i);
		if (mnemonic.equals("frr")) graphics.fillRoundedRectangle(p[0].i, p[1].i, p[2].i, p[3].i, p[4].i, p[5].c);
		if (mnemonic.equals("a")) graphics.drawArc(p[0].i, p[1].i, p[2].i, p[3].a, p[4].c, p[5].i);
		if (mnemonic.equals("fa")) graphics.fillArc(p[0].i, p[1].i, p[2].i, p[3].a, p[4].c);
		if (mnemonic.equals("t")) graphics.drawTriangle(p[0].i, p[1].i, p[2].i, p[3].i, p[4].i, p[5].i, p[6].c, p[7].i);
		if (mnemonic.equals("ft")) graphics.fillTriangle(p[0].i, p[1].i, p[2].i, p[3].i, p[4].i, p[5].i, p[6].c);
		if (mnemonic.equals("l")) graphics.drawLine(p[0].i, p[1].i, p[2].i, p[3].i, p[4].c, p[5].i);
		if (mnemonic.equals("hl")) graphics.drawHorizontalLine(p[0].i, p[2].i, p[1].i, p[3].c, p[4].i);
		if (mnemonic.equals("vl")) graphics.drawVerticalLine(p[0].i, p[1].i, p[2].i, p[3].c, p[4].i);
		if (mnemonic.equals("fco")) graphics.fillCorner(p[0].i, p[1].i, p[2].i, p[3].a, p[4].c);
		if (mnemonic.equals("bd")) graphics.drawBigDigit(p[5].i, p[0].i, p[1].i, p[2].i, p[3].i, p[6].c, p[4].i, p[7].g);
	}
	
	public ArduRCT_JAVA graphics = new ArduRCT_JAVA();
	public String target;
	public Hashtable<String, String> parserParams;
	public Hashtable<String, String> parserFunctions;
	public Hashtable<String, String> parserCode;
	public Hashtable<String, String> placer;

	private Thread t = null;
	private boolean running = false;
	private Parser parser;
	private JEditorPane codeArea;
	private JLabel help;
	private JComboBox<String> functionList;
	private Vector<String> listItems;
	private Vector<String> mnemonics;
	private Hashtable<String, String> hints;
	private String listFunction = null;
	private String parserText = "";
	private int waiter = 0;
	private int placerIndex = 0;
	
	public Designer(String target) {
		this.target = target;
		listItems = new Vector<String>();
		mnemonics = new Vector<String>();
		hints = new Hashtable<String, String>();
		parserParams = new Hashtable<String, String>();
		parserFunctions = new Hashtable<String, String>();
		parserCode = new Hashtable<String, String>();
		placer = new Hashtable<String, String>();
		for (String function : DESIGNER_ELEMENTS) {
			String part[] = function.split(",");
			for (int i=0; i<part.length; i++) part[i] = part[i].trim();
			listItems.add(part[0]);
			mnemonics.add(part[2]);
			placer.put(part[0], part[1]);
			hints.put(part[0], extractParamNames(part[2] + " " + part[3]));
			parserParams.put(part[2], part[3]);
			parserFunctions.put(part[2], part[4]);
			parserCode.put(part[2], part[5]);
		}
		listFunction = listItems.elementAt(0);
	}
	
	private String extractParamNames(String line) {
		String paramList[] = line.split(" ");
		String result = "";
		for (String param : paramList) {
			if (param.charAt(0) == '[') result += "[" + param.substring(2, param.indexOf(']')) + "]";
			else if (param.indexOf(']') != -1) result += param.substring(result.equals("") ? 0 : 1, param.indexOf(']'));
			else result += param.substring(result.equals("") ? 0 : 1);
			result += " ";
		}
		return result.trim();
	}
	
	private void buildUI() {	
		JPanel north = new JPanel();
		north.setLayout(new BorderLayout(2, 2));
		functionList = new JComboBox<String>(listItems.toArray(new String[listItems.size()]));
		functionList.setSelectedIndex(0);
		functionList.addActionListener(this);
		help = new JLabel(hints.get(listFunction));
		help.setBorder(new EmptyBorder(2, 10, 2, 2));
		north.add(new JLabel(" "), BorderLayout.WEST);
		north.add(functionList, BorderLayout.CENTER);
		north.add(help, BorderLayout.SOUTH);
		parser = new Parser(this);
		Dimension minimumSize = new Dimension(100, 50);
		JScrollPane textScrollPane = new JScrollPane(parser); 
		textScrollPane.setPreferredSize(new Dimension(800, 200));
		textScrollPane.setMinimumSize(minimumSize);
		codeArea = new JEditorPane("text/html", "");
		codeArea.setTransferHandler(new HTMLNewLineCopyHandler());
		codeArea.setEditable(false);
		JScrollPane codeScrollPane = new JScrollPane(codeArea); 
		codeScrollPane.setPreferredSize(new Dimension(800, 500));
		codeScrollPane.setMinimumSize(minimumSize);
		this.setLayout(new BorderLayout(2, 2));
		JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT, textScrollPane, codeScrollPane);
		splitPane.setOneTouchExpandable(true);
		splitPane.setDividerLocation(150);
		this.add(north, BorderLayout.NORTH);
		this.add(splitPane, BorderLayout.CENTER);
	}

	public static Designer createAndShowInstance(String title, TFTTouchPanel touchpanel) {
		// Adapt to the local look and feel
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {	}

		//Set up the application.
		Designer designer = new Designer(title);
		designer.setTitle(title + " Designer");
		designer.buildUI();
		designer.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		designer.pack();
		designer.setVisible(false);
		designer.addWindowListener(designer);
		return designer;
    }
	
	public void start() {
		running = true;
		t = new Thread(this);
		t.start();
		graphics.fillScreen(WHITE);
	}
	
	public void stop() {
		running = false;
		if (t != null) t.interrupt();
	}

	public void run() {
		while (running) {
			if (parserText.equals(parser.getText())) {
				waiter ++;
			} else {
				parserText = parser.getText();
				waiter = 0;
				String mnemonic = parser.getCurrentMnemonic();
				if (mnemonic != null) {
					int index = mnemonics.indexOf(mnemonic);
					if (index != -1) functionList.setSelectedIndex(index);
				}
			}
			if (waiter == 50) {
				String code = parser.parse();
				if (code == null) codeArea.setText(null);
				else if (!code.equals("")) codeArea.setText(code);
			}
			place();
			try { Thread.sleep(10); } catch (Exception e) { }
		}
	}

	int lastX;
	int lastY;
	public void place() {
		byte placerParam[] = placer.get(listFunction).getBytes();
		if (TFTTouchPanel.getTouchZ() == TOUCHPANEL_NO_TOUCH) {
			// if pressed and now released, advance placer
			if ((placerIndex % 2) == 1) placerIndex ++;
			// if we have finished to insert the params, restart the function
			if (placerParam.length == placerIndex/2) placerIndex = 0;
			return;
		}
		String shortFunction = hints.get(listFunction).split(" ")[0];
		int touchX = TFTTouchPanel.getTouchX();
		int touchY = TFTTouchPanel.getTouchY();
		if (placerIndex == 0) {
			String insert = shortFunction + " ";
			if (placerParam[0] != '-') insert += touchX + " " + touchY;
			parser.addLine(insert);
			lastX = TFTTouchPanel.getTouchX();
			lastY = TFTTouchPanel.getTouchY();
			placerIndex ++;
		} else if ((placerIndex % 2) == 0) {
			if (placerParam[placerIndex/2] == 'p') 
				parser.addText(" " + touchX + " " + touchY + " ");
			else if (placerParam[placerIndex/2] == 'd') 
				parser.addText(" " + Math.abs(touchX-lastX) + " " + Math.abs(touchY-lastY) + " ");
			else if (placerParam[placerIndex/2] == 'x') 
				parser.addText(" " + touchX + " ");
			else if (placerParam[placerIndex/2] == 'y') 
				parser.addText(" " + touchY + " ");
			else if (placerParam[placerIndex/2] == 'r') 
				parser.addText(" " + (int)Math.sqrt((touchX-lastX)*(touchX-lastX)+(touchY-lastY)*(touchY-lastY)) + " ");
			placerIndex ++;
		}
	}
	
	public void windowClosing(WindowEvent arg0) {
		stop();
		setVisible(false);
	}

	public void windowClosed(WindowEvent arg0) {}
	public void windowOpened(WindowEvent arg0) {}
	public void windowActivated(WindowEvent arg0) {}
	public void windowDeactivated(WindowEvent arg0) {}
	public void windowDeiconified(WindowEvent arg0) {}
	public void windowIconified(WindowEvent arg0) {}


	@SuppressWarnings("unchecked")
	public void actionPerformed(ActionEvent ae) {
		listFunction = (String)((JComboBox<String>)(ae.getSource())).getSelectedItem();
		help.setText(hints.get(listFunction));
	}
}

