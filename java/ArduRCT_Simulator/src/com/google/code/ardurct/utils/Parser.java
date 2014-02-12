package com.google.code.ardurct.utils;

import java.awt.BorderLayout;
import java.awt.Font;
import java.util.Hashtable;
import java.util.Vector;

import javax.swing.JPanel;
import javax.swing.JTextArea;

import com.google.code.ardurct.hardware.TFTTouchPanel;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIElement;
import com.google.code.ardurct.libraries.graphics.ArduRCT_GraphicsUIScreen;
import com.google.code.ardurct.libraries.graphics.IGraphicsDefines;

public class Parser extends JPanel 
implements IGraphicsDefines {

	private static final long serialVersionUID = 2359201301639457597L;
		
	public static String INDENT = "&nbsp;&nbsp;&nbsp;&nbsp;";
	public static String NL = "<br/>\r\n";
	public static String FONT_NAME = "Courier New";
	public static int FONT_SIZE = 11;
	public static String COMMENT_BLOCK_COLOR = "blue";
	public static String COMMENT_LINE_COLOR = "green";
	public static String KEYWORD_COLOR = "#A0522D";

	private JTextArea textArea;
	private String text = null;
	private Designer designer;

	private Hashtable<String, CodeSection> sections;
	private CodeSection currentSection;
	private CodeSection defines;
	private CodeSection declarations;
	private Hashtable<String, CodeLine> screenLines; 
	private Vector<String> screenNames;
	private Hashtable<String, ArduRCT_GraphicsUIScreen> screens;
	private Vector<String> uiElementNames;
	private Vector<String> sequence;
	
	public Parser(Designer designer) {
		this.designer = designer;
		textArea = new JTextArea(500, 20);
		textArea.setFont(new Font(FONT_NAME, Font.PLAIN, FONT_SIZE));
		setLayout(new BorderLayout());
		add(BorderLayout.CENTER, textArea);
	}
	
	public String getText() {
		if (textArea.getText() == null) return "";
		return textArea.getText();
	}
	
	public String getCurrentMnemonic() {
		// find the current mnemonic
		try {
			int caret = textArea.getCaretPosition();
			int caretLine = textArea.getLineOfOffset(caret);
			int caretLineStart = textArea.getLineStartOffset(caretLine);
			int caretLineEnd = textArea.getLineEndOffset(caretLine);
			String mnemonic = textArea.getText().substring(caretLineStart, caretLineEnd);
			if (mnemonic.indexOf(' ') != -1) mnemonic = mnemonic.substring(0, mnemonic.indexOf(' '));
			return mnemonic;
		} catch (Exception e) {
			return null;
		}	
	}
	
	public void addLine(String line) {
		try {
			int caret = textArea.getCaretPosition();
			int caretLine = textArea.getLineOfOffset(caret);
			int caretLineStart = textArea.getLineStartOffset(caretLine);
			int caretLineEnd = textArea.getLineEndOffset(caretLine);
			if (caret == caretLineStart) {
				textArea.insert(line + (caretLineEnd != caretLineStart ? "\n" : ""), caret);
				caret += (caretLineEnd != caretLineStart ? 1 : 0);
			} else if (caret == caretLineEnd) {
				textArea.insert("\n" + line, caret);
				caret ++;
			} else textArea.insert(line, caretLineEnd);
			caret += line.length();
			textArea.setCaretPosition(caret);
		} catch (Exception e) {
			return;
		}
	}
	
	public void addText(String text) {
		int caret = textArea.getCaretPosition();
		textArea.insert(text, caret);
		textArea.setCaretPosition(caret + text.length());
	}
	
	public String parse() {
		if (textArea.getText() != null && text != null && textArea.getText().equals(text)) return "";
		text = textArea.getText();
		if (text == null || text.equals("")) return null;		
		String text = textArea.getText();
		text = text.replace('\t', ' ');
		text = text.replace('\r', '§');
		text = text.replace('\n', '§');
		text = text.replaceAll("§§", "§");
		String textLines[] = text.split("§");
		// rebuild correct text
		Vector<String> lines = new Vector<String>();
		for (String line : textLines) {
			line = line.trim();
			if (line.startsWith("/*")) continue;
			if (line.endsWith("*/")) continue;
			if (line.startsWith("#")) continue;
			if (line.startsWith("    ")) continue;
			if (line.startsWith("//")) continue;
			if (line.startsWith("}")) continue; 
			if (line.endsWith("{")) continue;
			int indOf = line.indexOf('(');
			if (indOf != -1 && indOf > 0 && line.charAt(indOf-1) != '\\') continue;
			while (line.startsWith("*")) line = line.substring(1).trim();
			lines.add(line);
		}
		String correctText = null;
		for (String line : lines) correctText = (correctText == null ? "" : correctText + "\n") + line;
		if (Math.abs(text.length()-correctText.length()) > 5) {
			textArea.setText(correctText);
			if (correctText != null) textArea.setCaretPosition(correctText.length());
		}
		
		String code = "<html><span style=\"font: " + FONT_SIZE + "pt '" + FONT_NAME + "'\">"; 
		code += color(COMMENT_BLOCK_COLOR) + "/** ---- " + Designer.MARKER + " ---- **" + NL;
		for (String line : lines) {
			if (line.length() > 1) code += "&nbsp;* " + line + NL;
		}
		code += "&nbsp;** ---- " + Designer.MARKER + " ---- **/" + color(null) + NL;
		code += NL;
		
		sections = new Hashtable<String, CodeSection>();
		defines = new CodeSection("__defines__", '#');
		declarations = new CodeSection("__declarations__", 'd');
		CodeSection setup = new CodeSection("setup", 's');
		CodeSection loop = new CodeSection("loop", 'l');
		sections.put("__defines__", defines);
		sections.put("__declarations__", declarations);
		sections.put("setup", setup);
		sections.put("loop", loop);
		
		currentSection = setup;
		sequence = new Vector<String>();
		
		screenLines = new Hashtable<String, CodeLine>(); 
		screenNames = new Vector<String>(); 
		uiElementNames = new Vector<String>();
		// parse the lines
		for (String line : lines) {
			if (line.length() > 0 && line.charAt(0) != '-') parseLine(line);
		}
		// build the sections
		Vector<String> sectionNames = new Vector<String>();
		sectionNames.add("__defines__");
		sectionNames.add("__declarations__");
		sectionNames.add("setup");
		sectionNames.add("loop");
		for (String sectionName : sections.keySet()) {
			if (!sectionNames.contains(sectionName)) sectionNames.add(sectionName);
		}
		
		String includes = "#include &lt;" + keyword("Wire") + ".h&gt;" + NL +
				"#include &lt;" + keyword("SPI") + ".h&gt;" + NL + 
				"#include &lt;" + keyword("ArduRCT_Graphics") + ".h&gt;" + NL;
		if (designer.target.equals("Touscruino")) {
			includes += "#include &lt;" + keyword("ArduRCT_EventManager") + ".h&gt;" + NL + NL +
					"#define TOUSCRUINO_VERSION " + TFTTouchPanel.VERSION + NL + 
					"#include &lt;" + keyword("ArduRCT_TouScruino") + ".h&gt;";
		} else if (TFTTouchPanel.VERSION == 2) includes += "#include &lt;" + keyword("ArduRCT_SPFD5408") + ".h&gt;";
		else includes += "#include &lt;" + keyword("ArduRCT_ST7735") + ".h&gt;";
		includes += NL + NL;
		
		code += includes;
		if (designer.target.equals("Touscruino")) {
			loop.addLine(new CodeLine(designer.target, "manageEvents", null));
			if (setup.touscruinoHasUI) setup.addLine(new CodeLine(designer.target, "enableGraphicsUI", null), 0);
		} else {
			declarations.addLine(new CodeLine(designer.target, TFTTouchPanel.VERSION == 2 ? "ArduRCT_SPFD5408" : "ArduRCT_ST7735", designer.target));
		}
		for (String sectionName : sectionNames) {
			if (sectionName.charAt(0) != '-') code += sections.get(sectionName).toString();
		}
		code += "</span></html>";
		
		// build the screens
		screens = new Hashtable<String, ArduRCT_GraphicsUIScreen>();
		for (String screenName : screenNames) {
			CodeLine screenLine = screenLines.get(screenName);
			ArduRCT_GraphicsUIScreen screen = new ArduRCT_GraphicsUIScreen();
			screen.setGraphics(designer.graphics);
			screen.setBackgroundColor(screenLine.nparam("background").c);
			screens.put(screenName, screen);
		}
		// add the uiElements to the screens
		for (String sectionName : sectionNames) {
			sections.get(sectionName).addUIElements();
		}
		// draw the screens
		designer.graphics.fillScreen(WHITE);
		for (String screenName : screenNames) {
			CodeLine screenLine = screenLines.get(screenName);
			ArduRCT_GraphicsUIScreen screen = screens.get(screenName);
			if (screenLine.nparam("position").a != GRAPHICS_POSITION_NONE) screen.pack(screenLine.nparam("position").a);
			if (screenLine.nparam("selected").b) screen.activate();
		}
		// draw the sections
		for (String sectionName : sectionNames) {
			if (sectionName.charAt(0) != '-') sections.get(sectionName).draw();
		}
		
		return code;
	}
	
	private void parseLine(String line) {
		// isolate quoted strings
		String sline = line;
		if (line.indexOf('"') != -1) {
			boolean in = false;
			int i = 0;
			sline = "";
			while (i < line.length()) {
				if (i>0 && line.charAt(i) == '"' && line.charAt(i-1) != '\\') in = !in;
				if (line.charAt(i) == ' ' && in) sline += "¤";
				else sline += line.charAt(i);
				i ++;
			}
		}
		String linePart[] = sline.split(" ");
		String part[] = new String[20];
		for (int i=0; i<part.length; i++) part[i] = "?";
		for (int i=0; i<linePart.length; i++) part[i] = linePart[i].replace('¤', ' ');				
		if (part[0].startsWith("!")) {
			String sectionName = part[0].substring(1);
			if (sectionName.length() == 0) sectionName = color("red", "function");
			currentSection = sections.get(sectionName);
			if (currentSection == null) {
				currentSection = new CodeSection(sectionName);
				sections.put(sectionName, currentSection);
			}
			if (part.length > 1 && sections.containsKey(part[1])) 
				sections.get(part[1]).addLine(new CodeLine(null, sectionName, null));
		} else {
			CodeLine codeLine = new CodeLine(part);
			currentSection.addLine(codeLine);
			if (codeLine.declaration != null) declarations.addLine(codeLine);
			if (codeLine.define != null) defines.addLine(codeLine);
			// memorize the screen
			if (!codeLine.error && codeLine.mnemonic.equals("uis")) {
				screenLines.put(codeLine.param[0].t, codeLine);
				screenNames.add(codeLine.param[0].t);
			}
			if (!codeLine.error && codeLine.declaration != null) uiElementNames.add(codeLine.nparam("name").n);
		}
	}
	
	public static String color(String color) {
		return color(color, null);
	}
	
	public static String color(String color, String text) {
		if (color == null) return "</span>"; 
		if (text == null) return "<span style=\"color: " + color + "\">";
		return "<span style=\"color: " + color + "\">" + text + "</span>";
	}

	public static String keyword(String text) {
		return color(KEYWORD_COLOR, text);
	}

	public static String target(String text) {
		return color(text.charAt(0) < 'a' ? KEYWORD_COLOR : "BLACK", text);
	}

	public static String comment(String text) {
		return color(COMMENT_LINE_COLOR, "// " + text);
	}

	class CodeSection {	
		String name;
		Vector<CodeLine> lines = new Vector<CodeLine>();
		char type;
		boolean touscruinoHasUI = false;
		boolean isUI = false;
		String prototype = "";
		
		public CodeSection(String n) {		
			this(n, 'v');
		}

		public CodeSection(String n, char t) {		
			name = n;
			type = t;
			if (type == 'v' || type == 's' || type == 'l') prototype = "void " + name + "() {" + NL;
			else if (type == '$') prototype = "void " + name + "(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height) {" + NL;
			else if (type == '&') prototype = "bool " + name + "(uint8_t elementId, int16_t value) {" + NL;
		}

		public void addLine(CodeLine line) {
			lines.add(line);
			if (line.declaration != null)  sections.get("setup").touscruinoHasUI = true;
		}

		public void addLine(CodeLine line, int position) {
			lines.add(position, line);
			if (line.declaration != null)  sections.get("setup").touscruinoHasUI = true;
		}

		
		public String toString() {
			if (lines.size() == 0 && type != '$') return "";
			String section = prototype;
			boolean firstAction = true;
			for (CodeLine line : lines) {
				if (type == 'v' || type == '$' || type == 's' || type == 'l') section += line.text + NL;
				else if (type == '#') section += line.define + NL;
				else if (type == 'd') section += line.declaration + NL;
				else if (type == '&') section += (firstAction ? INDENT : " else ") + line.action;
				firstAction = false;
			}
			for (CodeLine line : lines) {
				if ((type == 'v' || type == 's') && line.pack != null) section += line.pack + NL;
			}
			if (type == '&') section += NL + INDENT + "return false;" + NL;
			if (!prototype.equals("")) section += "}" + NL;
			section += NL;
			return section;
		}
		
		public void draw() {
			if (type != 'v' && type != 's' && type != '$' && type != 'l') return;
			for (CodeLine line : lines) {
				// draw the graphics
				if (!line.error && line.mnemonic != null && line.declaration == null) {
					if (line.mnemonic.equals("d")) delay(line.nparam("ms").i);
					else designer.draw(line.mnemonic, line.param);
				}
			}
		}
		
		public void delay(long ms) {
			try {
				Thread.sleep(ms);			
			} catch (Exception e) {
			}
		}

		public void addUIElements() {
			if (type != 'v' && type != 's') return;
			for (CodeLine line : lines) {
				// add the UI elements
				if (!line.error && line.declaration != null && !line.function.endsWith("Screen")) {
					String screenName = line.nparam("screen").t;
					ArduRCT_GraphicsUIScreen screen = screens.get(screenName);
					ArduRCT_GraphicsUIElement elt = designer.getUIElement(line.mnemonic, line.param);
					if (elt != null && screen != null) {
						screen.addElement(elt, line.param[0].u, line.param[1].u, line.param[2].u, line.param[3].u, line.nparam("section").u);
						if (line.nparam("selected") != null && line.nparam("selected").b) elt.setValue(GRAPHICS_UI_SELECTED);
					}
				}
			}
		}

	}

	class CodeLine {
		CodeParameter param[];
		String function = null;
		String define = null;
		String declaration = null;
		String text = null;
		String action = null;
		String pack = null;
		String mnemonic = null;
		boolean error = false;
		
		public CodeLine(String part[]) {
			error = true;
			part[0] = part[0].toLowerCase();
			// find the function
			function = designer.parserFunctions.get(part[0]);
			if (function == null) {
				text = INDENT + comment(designer.target + "." + color("red", part[0]));
				return;
			}
			mnemonic = part[0];
			error = false;
			// we found the function, let's parse the parameters
			String paramList = designer.parserParams.get(mnemonic);
			String paramDetails[] = paramList.split(" ");
			Vector<CodeParameter> parameters = new Vector<CodeParameter>();
			for (int i=0; i<paramDetails.length; i++) {
				CodeParameter p = new CodeParameter(paramDetails[i], part[i+1]);
				parameters.add(p);
				if (p.error) error = true;
				if (p.type == '&' || p.type == '$') {
					CodeSection handler = sections.get(p.text);
					if (handler == null) {
						handler = new CodeSection(p.text, p.type);
						sections.put(p.text, handler);
					}
					if (p.type == '&') handler.addLine(this);
				}			
			}
			param = parameters.toArray(new CodeParameter[parameters.size()]);
			String parserCode = designer.parserCode.get(mnemonic);
			boolean hasDefine = parserCode.charAt(0) == '#';
			char codeType = parserCode.charAt(2);
			// get the params to put in the code
			String codeParam[] = new String[0];
			if (parserCode.length() > 4) {
				if (parserCode.substring(4).charAt(0) == '=') codeParam = paramDetails;
				else codeParam = parserCode.substring(4).split(" ");
				for (int i=0; i<codeParam.length; i++) {
					if (codeParam[i].charAt(0) == '[') codeParam[i] = codeParam[i].substring(1, codeParam[i].indexOf(']'));
					else if (codeParam[i].indexOf(']') != -1) codeParam[i] = codeParam[i].substring(0, codeParam[i].indexOf(']'));
				}
			}
			if (codeType == 'g' || codeType == 'a') {
				// g: this is a simple graphics function
				// a: this is a straight Arduino function
				paramList = "(";
				for (int i=0; i<codeParam.length; i++) {
					CodeParameter np = nparam(codeParam[i].substring(1));
					if (np != null && (!np.option || !np.empty)) {
						if (!paramList.endsWith("(")) paramList += ", ";
						paramList += np.get(codeParam[i].charAt(0));
					}
				}
				paramList += ");";
				String target = "";
				if (codeType == 'g') target = designer.target + ".";
				if (error) text = INDENT + comment(target + function + paramList);
				else text = INDENT + target(target) + keyword(function) + paramList;
			} else {
				// define
				if (hasDefine) {
					define = "#define " + nparam("name").get('N') + " " + nparam("id");
					if (error) define = comment(define);
				}
				// declaration
				declaration = nparam("name").text;
				if (codeParam.length > 0) {
					declaration += "(";
					for (int i=0; i<codeParam.length; i++) {
						if (!declaration.endsWith("(")) declaration += ", ";
						declaration += nparam(codeParam[i].substring(1)).get(codeParam[i].charAt(0));
					}
					declaration += ");";
				} else declaration += ";";
				if (error) declaration = comment(function + " " + declaration);
				else declaration = keyword(function) + " " + declaration;
				// code line
				if (codeType == 's') {
					if (error) text = INDENT + comment(designer.target + ".addScreen(&" + nparam("name") + ");");
					else text = INDENT + target(designer.target) + "." + keyword("addScreen") + "(&" + nparam("name") + ");";
					// pack
					if (!nparam("position").empty && nparam("position").a != GRAPHICS_POSITION_NONE) {
						if (error) pack = INDENT + comment(nparam("name") + ".pack(" + nparam("position") + ");");
						else pack = INDENT + nparam("name") + "." + keyword("pack") + "(" + nparam("position") + ");";
					}
					if (!nparam("background").empty) {
						pack = (pack != null ? pack + NL : "");
						if (error) pack += INDENT + comment(nparam("name") + ".setBackgroundColor(" + nparam("background") + ");");
						else pack += INDENT + nparam("name") + "." + keyword("setBackgroundColor") + "(" + nparam("background") + ");";
					}
					if (nparam("selected").b) {
						pack = (pack != null ? pack + NL : "");
						if (error) pack += INDENT + comment(designer.target + ".setGraphicsUIScreen(&" + nparam("name") + ");");
						else pack += INDENT + target(designer.target) + "." + keyword("setGraphicsUIScreen") + "(&" + nparam("name") + ");";
					}
				} else {
					text = "(&" + nparam("name") + ", " + nparam("x") + ", " + nparam("y") + ", " + nparam("width") + ", " + nparam("height");
					if (!nparam("section").empty) text += ", " + nparam("section");
					text += ");";
					if (error) text = INDENT + comment(nparam("screen") + ".addElement" + text);
					else text = INDENT + nparam("screen") + "." + keyword("addElement") + text;
					if (nparam("selected") != null && nparam("selected").b) {
						if (error) text = INDENT + comment(nparam("name") + ".setValue(GRAPHICS_UI_SELECTED);");
						else text = INDENT + nparam("name") + "." + keyword("setValue") + "(GRAPHICS_UI_SELECTED);";
					}
				}
				// action line setGraphicsUIScreen
				action = "if (elementId == " + nparam("name").get('N') + ") {" + NL +
							INDENT + INDENT + "return true;" + NL +
							INDENT + "}";
			}	
		}
		
		public CodeLine(String target, String function, String name) {
			if (target == null) text = INDENT + function +"();";
			else if (name == null) text = INDENT + target(target) + "." + keyword(function) + "();";
			else declaration = keyword(function) + " " + name + ";";
		}
		
		public CodeParameter nparam(String name) {
			for (CodeParameter p : param) {
				if (p.name.equals(name)) return p;
			}
			return null;
		}
	}
	
	class CodeParameter 
	implements IGraphicsDefines {
		public int a;		// angle
		public boolean b;	// boolean
		public int c;		// color
		public int d;		// define
		public int g;		// graphics style
		public int f;		// font
		public int i;		// int
		public String n;	// name as text
		public String t;	// text
		public int u;		// gui
		public String s;	// screen
		public int ta[];	// text as int array
		
		public String name;
		public char type;
		public boolean option;
		public String text;	// the final text to show
		public boolean error = false;
		public boolean empty = false;
		
		public CodeParameter(String name, String value) {
			try {
				t = value;
				option = false;
				if (value.equals("?")) {
					empty = true;
					if (name.indexOf(']') != -1) t = name.substring(name.indexOf(']')+1);
				}
				if (name.startsWith("[")) {
					option = true;
					name = name.substring(1, name.indexOf(']'));
				}
				if (name.indexOf(']') != -1) name = name.substring(0, name.indexOf(']'));
				type = name.charAt(0);
				if (t.equals("*") && type == 'c') t = "0x" + Integer.toHexString(TFTTouchPanel.getRGB565()).toUpperCase();
				else if (t.equals("+")) {
					int i = sequence.size() + 1;
					while (sequence.contains("" + i)) i = sequence.size() + 1;
					t = "" + i;
				}
				n = t;
				name = name.substring(1);
				this.name = name;
				if (type == 'i') this.i = parseValue();
				else if (type == 'c') {
					this.c = parseColor();
					t = t.toUpperCase();
					if (t.startsWith("0X")) t = "0x" + t.substring(2);
				} else if (type == 'd') {
					d = parseValue();
					sequence.add("" + d);
				} else if (type == 'f') {
					this.f = parseFont();
					if (f < FONT_SMALL) f = FONT_SMALL;
					else if (f > FONT_HUGE) f = FONT_HUGE;
					if (f == FONT_SMALL) t = "FONT_SMALL";
					else if (f == FONT_MEDIUM) t = "FONT_MEDIUM";
					else if (f == FONT_BIG) t = "FONT_BIG";
					else if (f == FONT_HUGE) t = "FONT_HUGE";
				} else if (type == 'g') {
					this.g = parseStyle();
					if (g < GRAPHICS_STYLE_BASIC) g = GRAPHICS_STYLE_BASIC;
					else if (g > GRAPHICS_STYLE_ADVANCED) g = GRAPHICS_STYLE_ADVANCED;
					if (g == GRAPHICS_STYLE_BASIC) t = "GRAPHICS_STYLE_BASIC";
					else if (g == GRAPHICS_STYLE_NORMAL) t = "GRAPHICS_STYLE_NORMAL";
					else if (g == GRAPHICS_STYLE_ADVANCED) t = "GRAPHICS_STYLE_ADVANCED";			
				} else if (type == 'a') {
					this.a = parseAngle();
					if (t.charAt(0) <= '9') t = "GRAPHICS_ROTATION_" + t;
					else t = "GRAPHICS_" + (name.equals("position") ? "POSITION_" : "ARC_") + t.toUpperCase();
				} else if (type == 'b') {
					this.b = parseBoolean();
					t = b ? this.name.toUpperCase() : "false";
				} else if (type == 't') {
					if (t.charAt(0) == '"') t = t.substring(1);
					if (t.length() > 1 && t.charAt(t.length()-1) == '"') t = t.substring(0, t.length()-1);
				} else if (type == 'u') {
					this.u = parseUI();
					if (t.charAt(0) >= 'A') t = "GUI_" + t.toUpperCase();
				} else if (type == 's') {
					if (!screenLines.containsKey(t)) throw new Exception();
				} else if (type == 'n') {
					if (t.equals("?")) throw new Exception();
					if (uiElementNames.contains(t)) throw new Exception();
				} 
				text = t;
				if (type == 't') text = "\"" + t + "\"";
				if (t.length() > 0) {
					ta = new int[t.length()+1];
					ta[t.length()] = 0;
					for (int j=0; j<t.length(); j++) ta[j] = t.charAt(j);
					for (int j=0; j<t.length()-1; j++) {
						if (ta[j] == '\\' && ta[j+1] == 'n') {
							ta[j] = '\n';
							for (int k=j+1; k<t.length(); k++) ta[k] = ta[k+1];
						}
						if (ta[j] == 0) break;
					}
				}
			} catch (Exception e) {
				text = Parser.color("red", name + ":" + t);
				error = true;
			}
		}
		
		public String get(char T) {
			if (T == 'p') return "&" + text;
			if (T == 'N') return text.toUpperCase();
			return text;
		}
		
		public String toString() {
			return text;
		}
		
		private int parseFont() throws Exception {
			String value = t.toUpperCase();
			if (value.startsWith("S")) return FONT_SMALL;
			if (value.startsWith("M")) return FONT_MEDIUM;
			if (value.startsWith("B")) return FONT_BIG;
			if (value.startsWith("H")) return FONT_HUGE;
			return Integer.parseInt(value, 10);
		}
		
		private int parseStyle() throws Exception {
			String value = t.toUpperCase();
			if (value.startsWith("B")) return GRAPHICS_STYLE_BASIC;
			if (value.startsWith("N")) return GRAPHICS_STYLE_NORMAL;
			if (value.startsWith("A")) return GRAPHICS_STYLE_ADVANCED;
			return Integer.parseInt(value, 10);
		}

		private int parseUI() throws Exception {
			String value = t.toUpperCase();
			if (value.equals("AS")) return GUI_AS;
			if (value.equals("SAP")) return GUI_SAP;
			if (value.equals("W")) return GUI_W;
			if (value.equals("H")) return GUI_H;
			if (value.equals("LOP")) return GUI_LOP;
			if (value.equals("ROP")) return GUI_ROP;
			if (value.equals("ROPWM")) return GUI_ROPWM;
			if (value.equals("TOP")) return GUI_TOP;
			if (value.equals("BOP")) return GUI_BOP;
			if (value.equals("BOPWM")) return GUI_BOPWM;
			if (value.startsWith("HEA")) {
				t = "HEADER";
				return GUI_HEADER;
			}
			if (value.startsWith("FO")) {
				t = "FOOTER";
				return GUI_FOOTER;
			}
			if (value.startsWith("MA")) {
				t = "MAIN";
				return GUI_MAIN;
			}
			return Integer.parseInt(value, 10);
		}
		
		private int parseAngle() throws Exception {
			String value = t.toUpperCase();
			if (value.equals("0")) return GRAPHICS_ROTATION_0;
			if (value.equals("90")) return GRAPHICS_ROTATION_90;
			if (value.equals("180")) return GRAPHICS_ROTATION_180;
			if (value.equals("270")) return GRAPHICS_ROTATION_270;
			if (value.equals("SSE")) return GRAPHICS_ARC_SSE;
			if (value.equals("SEE")) return GRAPHICS_ARC_SEE;
			if (value.equals("NEE")) return GRAPHICS_ARC_NEE;
			if (value.equals("NNE")) return GRAPHICS_ARC_NNE;
			if (value.equals("SSW")) return GRAPHICS_ARC_SSW;
			if (value.equals("SWW")) return GRAPHICS_ARC_SWW;
			if (value.equals("NWW")) return GRAPHICS_ARC_NWW;
			if (value.equals("NNW")) return GRAPHICS_ARC_NNW;
			if (value.equals("NE")) return GRAPHICS_ARC_NE;
			if (value.equals("SE")) return GRAPHICS_ARC_SE;
			if (value.equals("SW")) return GRAPHICS_ARC_SW;
			if (value.equals("NW")) return GRAPHICS_ARC_NW;
			if (value.equals("N")) return GRAPHICS_ARC_N;
			if (value.equals("S")) return GRAPHICS_ARC_S;
			if (value.equals("E")) return GRAPHICS_ARC_E;
			if (value.equals("W")) return GRAPHICS_ARC_W;
			if (value.equals("ALL")) return GRAPHICS_ARC_ALL;
			if (value.startsWith("C")) return GRAPHICS_ARC_ALL;
			if (value.startsWith("NO")) return GRAPHICS_POSITION_NONE;
			throw new Exception();
		}
		
		private boolean parseBoolean() throws Exception {
			String value = t.toUpperCase();
			if (value.charAt(0) == 'Y' || value.charAt(0) == 'T' || value.startsWith("SEL") || value.startsWith("B")) return true;
			if (value.charAt(0) == 'N' || value.charAt(0) == 'F') return false;
			throw new Exception();
		}
		
		private int parseValue() throws Exception {
			return Integer.parseInt(t);
		}
		
		private int parseColor() throws Exception {
			String value = t.toUpperCase();
			if (value.equals("WHITE")) return WHITE;
			if (value.equals("BLACK")) return BLACK;
			if (value.equals("RED")) return RED;
			if (value.equals("GREEN")) return GREEN;
			if (value.equals("BLUE")) return BLUE;
			if (value.equals("CYAN")) return CYAN;
			if (value.equals("MAGENTA")) return MAGENTA;
			if (value.equals("YELLOW")) return YELLOW;
			if (value.equals("LIGHT_GREY")) return LIGHT_GREY;
			if (value.equals("VLIGHT_GREY")) return VLIGHT_GREY;
			if (value.equals("GREY")) return GREY;
			if (value.equals("DARK_GREY")) return DARK_GREY;
			if (value.equals("ORANGE")) return ORANGE;
			if (value.equals("FUSCHIA")) return FUSCHIA;
			if (value.equals("VIOLET")) return VIOLET;
			if (value.equals("BROWN")) return BROWN;
			if (value.equals("DARK_GREEN")) return DARK_GREEN;
			if (value.equals("DARK_BLUE")) return DARK_BLUE;
			if (value.equals("DARK_RED")) return DARK_RED;
			if (value.equals("DARK_CYAN")) return DARK_CYAN;
			if (value.equals("LIGHT_BLUE")) return LIGHT_BLUE;
			if (value.equals("F_LIGHT_GREY")) return F_LIGHT_GREY;
			if (value.equals("F_GREY")) return F_GREY;
			try {
				if (value.startsWith("0X")) value = value.substring(2);
				return Integer.parseInt(value, 16);
			} catch (Exception e) {
				throw new Exception();
			}
		}
	}
}

