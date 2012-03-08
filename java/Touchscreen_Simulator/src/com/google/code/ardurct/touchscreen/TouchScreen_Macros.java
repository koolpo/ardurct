package com.google.code.ardurct.touchscreen;

import com.google.code.ardurct.Eeprom;


class macroCommand_t {
	int cmd;
	int nbParams = 0;
	int param[] = new int[6];
	int color = 0;
	int textLen = 0;
	int text[] = new int[1];	
} 

public class TouchScreen_Macros extends TouchScreen_UserInterface {

	/* 
	 *	A macro is a serie of drawing commands which can chain together.
	 *  Each command is a few letters followed by a space and some parameters separated by spaces
	 *
	 *	Case:		There is no case sentitiveness. 
	 *	Chaining: 	If the number of parameters is not sufficient to finish the command, 
	 *				the last point reached by the previous command is used 
	 *	Numbers:	Colors are in hexadecimal
	 *				all other are in decimal
	 *
	 *	Examples
	 *	--------
	 *		Draw a thick red line of 3 segments
	 *			pt 3 pc F800 l 10 10 30 40 l 40 50 
	 *		Draw a blue seven
	 *			pt 1 pc 001F l 10 10 30 10 l 30 13 anw 10 l 15 40
	 *
	 *	Command syntax
	 *  --------------
	 * 	(items between brackets can be ommited)
	 *
	 *		p	Preset			can preset the thickness, color, background, scale, fontSize, fontBold, fontOverlay
	 *			pe				erase the screen: fill the screen with background
	 *			pt[s] thickness	preset the thickness
	 *				s			if present, the thickness is scalable
	 *			ps scale [div]	preset the scale, all further drawing will be scaled by scale/div
	 *			pc color		preset the foreground color
	 *			pb color		preset the background color
	 *			pf[b][o] size	preset the font
	 *				b			if present the font will be bold
	 *				o			if present the font will be overlaid
	 *		
	 *		l	Line
	 *			l[d] x1 y1 [x2 y2]
	 *				d		delta: x1 and y1 (or x2 and y2 if they exist) are delta values to draw the next segment
	 *				x1 y1	start or next point in the line
	 *				x2 y2	next point in the line
	 *
	 *		a	Arc			can be drawn clockwise or counter clockwise and filled
	 *			a[r][f]DDD [x0 y0] radius
	 *				r		if present, the arc will be drawn reversed (counter clockwise). i.e.: if DDD=NE
	 *							if r the East end will be attached to the end of last line. 
	 *							else the North end will be attached
	 *				f		if present, the arc will be filled		
	 *				DDD 	can be n, s, e, w, ne, nw, se, sw, nne, nee, see, sse, ssw, sww, nww, nnw
	 *				radius	radius of the arc
	 *				x0 y0	center of the arc. If ommited the top (or bottom) of the arc will be connected to the last point drawn
	 *
	 *		r	Rectangle	can be rounded and filled
	 *			r[r][f] x1 y1 width height [radius]
	 *				r		if present, the rectangle will be rounded and radius is required
	 *				f		if present, the rectangle will be filled
	 *				x1 y1	top left of the rectangle
	 *				width
	 *				height	width and height of the rectangle
	 *
	 *		t	Triangle	can be filled
	 *			t[f] x1 y1 x2 y2 x3 y3
	 *				x1 y1	first point
	 *				x2 y2	second point
	 *				x3 y3	third point
	 *
	 *		c	Circle		can be filled
	 *			c[f] x0 y0 radius
	 *				x0 y0	center of the circle
	 *				radius	radius of the circle
	 * 
	 *		s	String
	 *			s x1 y1 "text"
	 *				x1 y1	top left of the first char drawn
	 *				"text"	text to draw. To draw a quote, some characters can be escaped \n, \", \\
	 *
	 *		w	Write		Write the macro in EEPROM, all following commands are written to the eeprom
	 *			w n
	 *				n		the macro number to write
	 *
	 *		e	Execute		Execute a macro from EEPROM
	 *			e[r] n
	 *				r		reset macro variables before executing
	 *				n		the macro number to execute
	 */

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public final static int SCREEN_MACRO_MAX_NUMBER  = 16;
	public final static int SCREEN_MACRO_MAX_SIZE  = 128;

	public final static int SCREEN_MACRO_CMD_NONE =						0x00;
	public final static int SCREEN_MACRO_CMD_PRESET_FOREGROUND =		0x11;
	public final static int SCREEN_MACRO_CMD_PRESET_BACKGROUND =		0x12;
	public final static int SCREEN_MACRO_CMD_PRESET_FONT =				0x13;
	public final static int SCREEN_MACRO_CMD_PRESET_THICKNESS =			0x14;
	public final static int SCREEN_MACRO_CMD_PRESET_SCALE	=			0x15;
	public final static int SCREEN_MACRO_CMD_PRESET_ERASE	=			0x16;
	public final static int SCREEN_MACRO_CMD_LINE 		=				0x20;
	public final static int SCREEN_MACRO_CMD_LINE_DELTA 		=		0x21;
	public final static int SCREEN_MACRO_CMD_ARC 	=					0x30;
	public final static int SCREEN_MACRO_CMD_ARC_FILLED 	=			0x31;
	public final static int SCREEN_MACRO_CMD_ARC_REVERSED 	=			0x32;
	public final static int SCREEN_MACRO_CMD_ARC_FILLED_REVERSED 	=	0x33;
	public final static int SCREEN_MACRO_CMD_RECTANGLE	=				0x40;
	public final static int SCREEN_MACRO_CMD_RECTANGLE_FILLED	=		0x41;
	public final static int SCREEN_MACRO_CMD_RECTANGLE_ROUNDED	=		0x42;
	public final static int SCREEN_MACRO_CMD_RECTANGLE_ROUNDED_FILLED=	0x43;
	public final static int SCREEN_MACRO_CMD_TRIANGLE	=				0x50;
	public final static int SCREEN_MACRO_CMD_TRIANGLE_FILLED	=		0x51;
	public final static int SCREEN_MACRO_CMD_CIRCLE		=				0x60;
	public final static int SCREEN_MACRO_CMD_CIRCLE_FILLED	=			0x61;
	public final static int SCREEN_MACRO_CMD_STRING	=					0x70;
	public final static int SCREEN_MACRO_CMD_WRITE	=					0xE0;
	public final static int SCREEN_MACRO_CMD_EXECUTE	=				0xF0;
	public final static int SCREEN_MACRO_CMD_EXECUTE_WITH_RESET	=		0xF1;

	public final static int SCREEN_MACRO_CMD_LINKED_FLAG	=			0x08;

	public final static int SCREEN_MACRO_FORMAT_ERROR = -1;

	public final static int SCREEN_MACRO_CMD_GROUP_MASK = 0xF0;

	public final static int SCREEN_MACRO_CMD_GROUP_NONE		=			0x00;
	public final static int SCREEN_MACRO_CMD_GROUP_PRESET	=			0x10;
	public final static int SCREEN_MACRO_CMD_GROUP_LINE 	=			0x20;
	public final static int SCREEN_MACRO_CMD_GROUP_ARC	 	=			0x30;
	public final static int SCREEN_MACRO_CMD_GROUP_RECTANGLE=			0x40;
	public final static int SCREEN_MACRO_CMD_GROUP_TRIANGLE	=			0x50;
	public final static int SCREEN_MACRO_CMD_GROUP_CIRCLE	=			0x60;
	public final static int SCREEN_MACRO_CMD_GROUP_STRING	=			0x70;
	public final static int SCREEN_MACRO_CMD_GROUP_EXECUTE	=			0xF0;

	public final static int SCREEN_MACRO_PARAM_X1 = 0;
	public final static int SCREEN_MACRO_PARAM_Y1 = 1;
	public final static int SCREEN_MACRO_PARAM_X2 = 2;
	public final static int SCREEN_MACRO_PARAM_Y2 = 3;
	public final static int SCREEN_MACRO_PARAM_X3 = 4;
	public final static int SCREEN_MACRO_PARAM_Y3 = 5;
	public final static int SCREEN_MACRO_PARAM_WIDTH = 2;
	public final static int SCREEN_MACRO_PARAM_HEIGHT = 3;
	public final static int SCREEN_MACRO_PARAM_ROUNDING = 4;
	public final static int SCREEN_MACRO_PARAM_RADIUS = 2;
	public final static int SCREEN_MACRO_PARAM_THICKNESS = 0;
	public final static int SCREEN_MACRO_PARAM_THICKNESS_IS_SCALABLE = 1;
	public final static int SCREEN_MACRO_PARAM_FONT_SIZE = 0;
	public final static int SCREEN_MACRO_PARAM_FONT_IS_BOLD = 1;
	public final static int SCREEN_MACRO_PARAM_FONT_IS_OVERLAY = 2;
	public final static int SCREEN_MACRO_PARAM_SCALE_MUL = 0;
	public final static int SCREEN_MACRO_PARAM_SCALE_DIV = 1;
	public final static int SCREEN_MACRO_PARAM_ARC_1 = 0;
	public final static int SCREEN_MACRO_PARAM_ARC_3 = 2;
	public final static int SCREEN_MACRO_PARAM_ARC_OCTANT = 3;
	public final static int SCREEN_MACRO_PARAM_MACRO_NUMBER = 0;
	
	public final static int SCREEN_MACRO_COS45_LSH16	= 46340;

	int eeprom_read_uint8_t(int address) {
		return Eeprom.read_uint8_t(address);
	}
	
	void eeprom_write_uint8_t(int address, int value) {
		Eeprom.write_uint8_t(address, value);
	}

	public int[] executeMacro(String macro) {
		int buffer[] = new int[macro.length()+1];
		for (int i=0; i<macro.length(); i++) buffer[i] = macro.charAt(i);
		buffer[macro.length()] = 0;	
		return executeMacro(buffer, 0, 0, 1, 1, true, true);
	}

	public  int[] executeMacro(String macro, int x, int y, int scaleMul, int scaleDiv, boolean continueLastMacro) {
		int buffer[] = new int[macro.length()+1];
		for (int i=0; i<macro.length(); i++) buffer[i] = macro.charAt(i);
		buffer[macro.length()] = 0;	
		return executeMacro(buffer, x, y, scaleMul, scaleDiv, continueLastMacro, true);
	}

	public int[] executeMacro(int macro[], int x, int y, int scaleMul, int scaleDiv, boolean continueLastMacro, boolean grabAndReleaseBus) {
		int s[] = macro;
		macroCommand_t mc = new macroCommand_t();
		boolean drawMode = true;
		int wBuffer[] = new int[256];
		int wBufferPtr = 0;
		int wBufferNb = 0;
		
		// initialize relative origin
		if (!continueLastMacro) _initializeMacros();
		int i = 0;
		// convert to upper case, transform all char below space as space
		_formatMacroSentence(s);
		while (s[i] != 0) {
			// get the command
			switch (s[i]) {
				case 'L':
					mc.cmd = SCREEN_MACRO_CMD_LINE;
					if (s[i+1] == 'D') {
						mc.cmd = SCREEN_MACRO_CMD_LINE_DELTA;
						i++;
					}
					if (s[i+1] != ' ') return null;
					break;
				case 'A':
					mc.cmd = SCREEN_MACRO_CMD_ARC;
					if (s[i+1] == 'R') {
						mc.cmd = SCREEN_MACRO_CMD_ARC_REVERSED;
						i++;
					}
					if (s[i+1] == 'F') {
						if (mc.cmd == SCREEN_MACRO_CMD_ARC) mc.cmd = SCREEN_MACRO_CMD_ARC_FILLED;
						else mc.cmd = SCREEN_MACRO_CMD_ARC_FILLED_REVERSED;
						i++;
					} 
					if (s[i+1] == 'N') {
						if (s[i+2] == 'N') {
							if (s[i+3] == 'E') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_NNE;
							else if (s[i+3] == 'W') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_NNW;
							else if (s[i+3] != ' ') return null;
						} else if (s[i+2] == 'E') {
							if (s[i+3] == 'E') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_NEE;
							else if (s[i+3] == ' ') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_NE;
							else return null;
						} else if (s[i+2] == 'W') {
							if (s[i+3] == 'W') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_NWW;
							else if (s[i+3] == ' ') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_NW;
							else return null;
						} else if (s[i+2] == ' ') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_N;
						else return null;
					} else if (s[i+1] == 'S') {
						if (s[i+2] == 'S') {
							if (s[i+3] == 'E') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_SSE;
							else if (s[i+3] == 'W') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_SSW;
							else if (s[i+3] != ' ') return null;
						} else if (s[i+2] == 'E') {
							if (s[i+3] == 'E') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_SEE;
							else if (s[i+3] == ' ') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_SE;
							else return null;
						} else if (s[i+2] == 'W') {
							if (s[i+3] == 'W') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_SWW;
							else if (s[i+3] == ' ') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_SW;
							else return null;
						} else if (s[i+2] == ' ') mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_S;
						else return null;
					} else if ((s[i+1] == 'E') && (s[i+2] == ' ')) mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_E;
					else if ((s[i+1] == 'W') && (s[i+2] == ' ')) mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = SCREEN_ARC_W;
					else return null;
					while (s[i+1] != ' ') i++;
					break;
				case 'R':
					if (s[i+1] == 'R') {
						i++;
						if (s[i+1] == 'F') {
							i++;
							mc.cmd = SCREEN_MACRO_CMD_RECTANGLE_ROUNDED_FILLED;
						} else if (s[i+1] == ' ') mc.cmd = SCREEN_MACRO_CMD_RECTANGLE_ROUNDED;
					} else if (s[i+1] == 'F') {
						mc.cmd = SCREEN_MACRO_CMD_RECTANGLE_FILLED;
						i++;
					} else mc.cmd = SCREEN_MACRO_CMD_RECTANGLE;
					if (s[i+1] != ' ') return null;
					break;
				case 'S':
					mc.cmd = SCREEN_MACRO_CMD_STRING;
					if (s[i+1] != ' ') return null;
					break;
				case 'C':
					if (s[i+1] == 'F') {
						i++;
						mc.cmd = SCREEN_MACRO_CMD_CIRCLE_FILLED;
					} else mc.cmd = SCREEN_MACRO_CMD_CIRCLE;
					if (s[i+1] != ' ') return null;
					break;
				case 'T':
					if (s[i+1] == 'F') {
						i++;
						mc.cmd = SCREEN_MACRO_CMD_TRIANGLE_FILLED;
					} else mc.cmd = SCREEN_MACRO_CMD_TRIANGLE;
					if (s[i+1] != ' ') return null;
					break;
				case 'P':
					if (s[i+1] == 'T') {
						mc.param[SCREEN_MACRO_PARAM_THICKNESS_IS_SCALABLE] = 0;
						mc.cmd = SCREEN_MACRO_CMD_PRESET_THICKNESS;
						i++;
						if (s[i+1] == 'S') {
							i++;
							mc.param[SCREEN_MACRO_PARAM_THICKNESS_IS_SCALABLE] = 1;
						}
						if (s[i+1] != ' ') return null;
					} else if (s[i+1] == 'S') {
						mc.cmd = SCREEN_MACRO_CMD_PRESET_SCALE;
						i++;
						if (s[i+1] != ' ') return null;
					} else if (s[i+1] == 'F') {
						mc.cmd = SCREEN_MACRO_CMD_PRESET_FONT;
						mc.param[SCREEN_MACRO_PARAM_FONT_IS_BOLD] = 0;
						mc.param[SCREEN_MACRO_PARAM_FONT_IS_OVERLAY] = 0;
						i++;
						if (s[i+1] == 'B') {
							mc.param[SCREEN_MACRO_PARAM_FONT_IS_BOLD] = 1;
							i++;
						} 
						if (s[i+1] == 'O') {
							mc.param[SCREEN_MACRO_PARAM_FONT_IS_OVERLAY] = 1;
							i++;
						} 
						if (s[i+1] != ' ') return null;
					} else if (s[i+1] == 'C') {
						int len = _parseMacroCommandHexColor(s, i+2, mc);
						if (len == SCREEN_MACRO_FORMAT_ERROR) return null;
						i += len+1;
						mc.cmd = SCREEN_MACRO_CMD_PRESET_FOREGROUND;
					} else if (s[i+1] == 'B') {
						int len = _parseMacroCommandHexColor(s, i+2, mc);
						if (len == SCREEN_MACRO_FORMAT_ERROR) return null;
						i += len+1;
						mc.cmd = SCREEN_MACRO_CMD_PRESET_BACKGROUND;
					} else if (s[i+1] == 'E') {
						mc.cmd = SCREEN_MACRO_CMD_PRESET_ERASE;
						i++;
						if (s[i+1] != ' ') return null;
					} else return null;
					break;
				case 'E':
					if (s[i+1] == 'R') {
						i++;
						mc.cmd = SCREEN_MACRO_CMD_EXECUTE_WITH_RESET;
					} else mc.cmd = SCREEN_MACRO_CMD_EXECUTE;
					if (s[i+1] != ' ') return null;
					break;
				case 'W':
					drawMode = false;
					mc.cmd = SCREEN_MACRO_CMD_WRITE;
					if (s[i+1] != ' ') return null;
					break;
				case ' ':
					mc.cmd = SCREEN_MACRO_CMD_NONE;
					break;
				default:
					return null;
			}
			i++;
			
			if (mc.cmd == SCREEN_MACRO_CMD_NONE) continue;
			
			// parse the command parameters
			mc.nbParams = 0;
			int len = 0;
			while (true) {
				len = _parseMacroCommandParameter(s, i, mc, mc.nbParams);
				if (len == SCREEN_MACRO_FORMAT_ERROR) break;
				i += len;
				mc.nbParams ++;
			}
			if (mc.cmd == SCREEN_MACRO_CMD_STRING) {
				// for text, always 2 coordinates, followed by text
				while ((s[i] != 0) && (s[i] != '"')) i++;
				if (s[i] != '"') return null;
				i++;
				int txtStart = i;
				// search for end of string
				while ((s[i] != 0) && (s[i] != '"')) {
					// replace the FF by quotes
					if (s[i] == 0xFF) s[i] = '"';
					i++;
				}
				if (s[i] != '"') return null;
				s[i] = 0;
				mc.text = new int[i-txtStart+1];
				memcpy_P(mc.text, s, txtStart, i-txtStart+1);
				mc.textLen = i - txtStart;
			}

			
			// Execute the command
			if (drawMode) drawMacroCommand(mc, x, y, scaleMul, scaleDiv, true, grabAndReleaseBus);
			else if (mc.cmd == SCREEN_MACRO_CMD_WRITE) {
				wBufferNb = mc.param[SCREEN_MACRO_PARAM_MACRO_NUMBER];
				if (wBufferNb > SCREEN_MACRO_MAX_NUMBER) return null;
			}

			// At this stage, we have a struct containing the command to execute
			// this can be stored in EEPROM for future execution
			if (mc.cmd != SCREEN_MACRO_CMD_WRITE) {
				int endWrite = _compressMacroCommand(mc, wBuffer, wBufferPtr);
				if (endWrite != SCREEN_MACRO_FORMAT_ERROR) wBufferPtr = endWrite;
			}
		}
		if (!drawMode) {
			// write to EEPROM
			if ((wBufferPtr > 0) && (wBufferPtr < SCREEN_MACRO_MAX_SIZE)) {
				eeprom_write_uint8_t(wBufferNb, wBufferPtr);
				for (int j=0; j<wBufferPtr; j++) eeprom_write_uint8_t(SCREEN_MACRO_MAX_NUMBER + wBufferNb*SCREEN_MACRO_MAX_SIZE+j, wBuffer[j]);
			}
		}
		System.out.println(wBufferPtr);
		//debugArray(wBuffer, wBufferPtr, 1, 2);
		return wBuffer;
	}

	void _formatMacroSentence(int s[]) {
		int i=0;
		while (s[i] != 0) {
			// before quotes, convert lower into higher
			while ((s[i] != 0) && (s[i] != '"')) {
				if ((s[i] >= 'a') && (s[i] <= 'z')) s[i] = s[i] + 'A' - 'a';
				// replace characters not allowed by spaces
				if ((s[i] != '-') && ((s[i] < '0') || (s[i] > 'Z') || ((s[i] > '9') && (s[i] < 'A')))) s[i] = ' ';
				i++;
			}
			if (s[i] == 0) break;
			i++;
			// inside the quotes, replace escapes
			while ((s[i] != 0) && (s[i] != '"')) {
				if (s[i] == '\\') {
					if (s[i+1] == 'n') s[i+1] = '\n';
					else if (s[i+1] == 't') s[i+1] = ' ';
					// replace the inside quotes by FF, this will be undone when the string command is parsed
					else if (s[i+1] == '"') s[i+1] = 0xFF;
					int j = i;
					while (s[j] != 0) s[j] = s[++j];
					i++;
				}
				i++;
			}
			if (s[i] == 0) break;
			i++;
		}
		/*
		i=0;
		while (s[i] != 0) System.out.print((char)s[i++]);
		System.out.println();
		*/	
	}

	void drawMacroCommand(macroCommand_t mc, int x, int y, int scaleMul, int scaleDiv, boolean continueLastMacro, boolean grabAndReleaseBus) {
		// initialize relative origin
		if (!continueLastMacro) _initializeMacros();
		if (grabAndReleaseBus) _grabBus();
		_executeMacroCommand(mc, x, y, scaleMul, scaleDiv);
		if (grabAndReleaseBus) _releaseBus();
	}


	void executeEepromMacro(int macroNb, int x, int y, int scaleMul, int scaleDiv, boolean continueLastMacro, boolean grabAndReleaseBus) {
		macroCommand_t mc = new macroCommand_t();;
		// initialize relative origin
		if (!continueLastMacro) _initializeMacros();
		mc.cmd = SCREEN_MACRO_CMD_EXECUTE;
		mc.nbParams = macroNb;
		if (grabAndReleaseBus) _grabBus();
		_executeMacroCommand(mc, x, y, scaleMul, scaleDiv);
		if (grabAndReleaseBus) _releaseBus();
	}


	void _executeMacroCommand(macroCommand_t mc, int x, int y, int scaleMul, int scaleDiv) {
		int group = mc.cmd & SCREEN_MACRO_CMD_GROUP_MASK;
		
		// presets
		if (group == SCREEN_MACRO_CMD_GROUP_PRESET) {
			if (mc.cmd == SCREEN_MACRO_CMD_PRESET_FOREGROUND) _mForegroundColor = mc.color;
			else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_BACKGROUND) _mBackgroundColor = mc.color;
			else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_THICKNESS) {
				_mThickness = mc.param[SCREEN_MACRO_PARAM_THICKNESS];
				_mIsThicknessScalable = mc.param[SCREEN_MACRO_PARAM_THICKNESS_IS_SCALABLE] != 0;
			} else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_FONT) {
				_mFontSize = mc.param[SCREEN_MACRO_PARAM_FONT_SIZE];
				_mIsFontBold = (mc.param[SCREEN_MACRO_PARAM_FONT_IS_BOLD] != 0);
				_mIsFontOverlay = (mc.param[SCREEN_MACRO_PARAM_FONT_IS_OVERLAY] != 0);
			} else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_SCALE) {
				_mScaleMul = mc.param[SCREEN_MACRO_PARAM_SCALE_MUL];
				_mScaleDiv = 1;
				if ((mc.nbParams > 1) && (mc.param[SCREEN_MACRO_PARAM_SCALE_DIV] != 0)) _mScaleDiv = mc.param[SCREEN_MACRO_PARAM_SCALE_DIV];
			} else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_ERASE) {
				fillScreen(_mBackgroundColor);
			}
			return;
		}
		
		int sMul = _mScaleMul * (scaleMul == 0 ? 1 : scaleMul);
		int sDiv = _mScaleDiv * (scaleDiv == 0 ? 1 : scaleDiv);
		int sX = x + mc.param[SCREEN_MACRO_PARAM_X1] * sMul/sDiv;
		int sY = y + mc.param[SCREEN_MACRO_PARAM_Y1] * sMul/sDiv;		
		int sThickness = _mThickness;
		if (_mIsThicknessScalable) sThickness = sThickness * sMul/sDiv;	
		
		// lines
		if (group == SCREEN_MACRO_CMD_GROUP_LINE) {
			int sX2, sY2;
			if (mc.nbParams < 2) return;
			if (mc.cmd == SCREEN_MACRO_CMD_LINE) {
				if (mc.nbParams > 2) {
					sX2 = x + mc.param[SCREEN_MACRO_PARAM_X2] * sMul/sDiv;
					sY2 = y + mc.param[SCREEN_MACRO_PARAM_Y2] * sMul/sDiv;
					// p2 becomes the end point
					_mX = mc.param[SCREEN_MACRO_PARAM_X2];
					_mY = mc.param[SCREEN_MACRO_PARAM_Y2];
				} else {
					sX2 = sX;
					sY2 = sY;
					sX = x + _mX * sMul/sDiv;
					sY = y + _mY * sMul/sDiv;	
					// p1 becomes the end point
					_mX = mc.param[SCREEN_MACRO_PARAM_X1];
					_mY = mc.param[SCREEN_MACRO_PARAM_Y1];
				}
			} else {
				// delta values
				if (mc.nbParams > 2) {
					sX2 = x + (mc.param[SCREEN_MACRO_PARAM_X1] + mc.param[SCREEN_MACRO_PARAM_X2]) * sMul/sDiv;
					sY2 = y + (mc.param[SCREEN_MACRO_PARAM_Y1] + mc.param[SCREEN_MACRO_PARAM_Y2]) * sMul/sDiv;
					// p2 becomes the end point
					_mX = mc.param[SCREEN_MACRO_PARAM_X1] + mc.param[SCREEN_MACRO_PARAM_X2];
					_mY = mc.param[SCREEN_MACRO_PARAM_Y1] + mc.param[SCREEN_MACRO_PARAM_Y2];
				} else {
					sX2 = x + (_mX + mc.param[SCREEN_MACRO_PARAM_X1]) * sMul/sDiv;
					sY2 = y + (_mY + mc.param[SCREEN_MACRO_PARAM_Y1]) * sMul/sDiv;
					sX = x + _mX * sMul/sDiv;
					sY = y + _mY * sMul/sDiv;	
					// p1 becomes the end point
					_mX += mc.param[SCREEN_MACRO_PARAM_X1];
					_mY += mc.param[SCREEN_MACRO_PARAM_Y1];
				}		
			}
			drawLine(sX, sY, sX2, sY2, _mForegroundColor, sThickness, false);
			return;
		}	
		
		// arcs
		if (group == SCREEN_MACRO_CMD_GROUP_ARC) {
			if (mc.nbParams < 1) return;
			boolean reversed = (mc.cmd == SCREEN_MACRO_CMD_ARC_REVERSED) || (mc.cmd == SCREEN_MACRO_CMD_ARC_FILLED_REVERSED);
			int sRadius = 0;
			if (mc.nbParams == 1) {
				sRadius = mc.param[SCREEN_MACRO_PARAM_ARC_1] * sMul/sDiv;
				int sArcStartX = _getArcEnd(sRadius, mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT], !reversed, true);
				int sArcStartY = _getArcEnd(sRadius, mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT], !reversed, false);
				int sArcEndX = _getArcEnd(sRadius, mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT], reversed, true);
				int sArcEndY = _getArcEnd(sRadius, mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT], reversed, false);
				sX = x + _mX * sMul/sDiv - sArcStartX;
				sY = y + _mY * sMul/sDiv - sArcStartY;
				_mX += (sArcEndX - sArcStartX) * sDiv/sMul;
				_mY += (sArcEndY - sArcStartY) * sDiv/sMul;

			} else if (mc.nbParams == 3) {
				sRadius = mc.param[SCREEN_MACRO_PARAM_ARC_3] * sMul/sDiv;
				_mX = mc.param[SCREEN_MACRO_PARAM_X1] + _getArcEnd(mc.param[SCREEN_MACRO_PARAM_ARC_3], mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT], reversed, true);
				_mY = mc.param[SCREEN_MACRO_PARAM_Y1] + _getArcEnd(mc.param[SCREEN_MACRO_PARAM_ARC_3], mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT], reversed, false);
			} else return;
			if ((mc.cmd == SCREEN_MACRO_CMD_ARC_REVERSED) || (mc.cmd == SCREEN_MACRO_CMD_ARC)) 
				drawArc(sX, sY, sRadius, mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT], _mForegroundColor, sThickness, false);
			else fillArc(sX, sY, sRadius, mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT], _mForegroundColor, false);			
			return;
		}
		
		// circles
		if (group == SCREEN_MACRO_CMD_GROUP_CIRCLE) {
			// we need at least 3 parameters
			if (mc.nbParams < 3) return;
			int sRadius = mc.param[SCREEN_MACRO_PARAM_RADIUS] * sMul/sDiv;
			if (mc.cmd == SCREEN_MACRO_CMD_CIRCLE) drawCircle(sX, sY, sRadius, _mForegroundColor, sThickness, false);
			else fillCircle(sX, sY, sRadius, _mForegroundColor, false);
			return;
		}
		
		// rectangles
		if (group == SCREEN_MACRO_CMD_GROUP_RECTANGLE) {
			// we need at least 4 parameters
			if (mc.nbParams < 4) return;
			int sWidth = mc.param[SCREEN_MACRO_PARAM_WIDTH] * sMul/sDiv;
			int sHeight = mc.param[SCREEN_MACRO_PARAM_HEIGHT] * sMul/sDiv;
			if (mc.cmd == SCREEN_MACRO_CMD_RECTANGLE) drawRectangle(sX, sY, sWidth, sHeight, _mForegroundColor, sThickness, false);
			else if (mc.cmd == SCREEN_MACRO_CMD_RECTANGLE_FILLED) fillRectangle(sX, sY, sWidth, sHeight, _mForegroundColor, false);
			// we need at least 1 more parameter to display the rounded rectangle
			if (mc.nbParams < 5) return;
			int sRadius = mc.param[SCREEN_MACRO_PARAM_ROUNDING] * sMul/sDiv;
			if (mc.cmd == SCREEN_MACRO_CMD_RECTANGLE_ROUNDED) drawRoundedRectangle(sX, sY, sWidth, sHeight, sRadius, _mForegroundColor, sThickness, false);
			else if (mc.cmd == SCREEN_MACRO_CMD_RECTANGLE_ROUNDED_FILLED) fillRoundedRectangle(sX, sY, sWidth, sHeight, sRadius, _mForegroundColor, false);
			return;
		}
		
		// triangles
		if (group == SCREEN_MACRO_CMD_GROUP_TRIANGLE) {
			// we need at least 6 parameters
			if (mc.nbParams < 6) return;
			int sX2 = x + mc.param[SCREEN_MACRO_PARAM_X2] * sMul/sDiv;
			int sY2 = y + mc.param[SCREEN_MACRO_PARAM_Y2] * sMul/sDiv;
			int sX3 = x + mc.param[SCREEN_MACRO_PARAM_X3] * sMul/sDiv;
			int sY3 = y + mc.param[SCREEN_MACRO_PARAM_Y3] * sMul/sDiv;
			if (mc.cmd == SCREEN_MACRO_CMD_TRIANGLE) drawTriangle(sX, sY, sX2, sY2, sX3, sY3, _mForegroundColor, sThickness, false);
			else if (mc.cmd == SCREEN_MACRO_CMD_TRIANGLE_FILLED) fillTriangle(sX, sY, sX2, sY2, sX3, sY3, _mForegroundColor, false);
			return;
		}
			
		// strings
		if (mc.cmd == SCREEN_MACRO_CMD_STRING) {
			if (mc.nbParams < 2) return;
			int bc = _backgroundColor;
			if (!_mIsFontOverlay) setBackgroundColor(_mBackgroundColor);
			drawString(mc.text, sX, sY, _mForegroundColor, _mFontSize, _mIsFontBold, _mIsFontOverlay, false);
			if (!_mIsFontOverlay) setBackgroundColor(bc);
			return;
		}
		
		// executes
		if (group == SCREEN_MACRO_CMD_GROUP_EXECUTE) {
			if (mc.param[SCREEN_MACRO_PARAM_MACRO_NUMBER] >= SCREEN_MACRO_MAX_NUMBER) return;
			// read the length in the EEPROM allocation table
			int length = eeprom_read_uint8_t(mc.param[SCREEN_MACRO_PARAM_MACRO_NUMBER]);
			if (length == 0xFF) return;
			// read the EEPROM pointers table to get the start
			int start = SCREEN_MACRO_MAX_NUMBER + mc.param[SCREEN_MACRO_PARAM_MACRO_NUMBER] * SCREEN_MACRO_MAX_SIZE;
			// get the compressed macro
			int buffer[] = new int[SCREEN_MACRO_MAX_SIZE];
			int i=0;
			while (i < length) {
				buffer[i] = eeprom_read_uint8_t(start+i);
				i++;
			}
			buffer[i] = 0;
			macroCommand_t emc = new macroCommand_t();
			// uncompress the macro commands and execute them
			if (mc.cmd == SCREEN_MACRO_CMD_EXECUTE_WITH_RESET) _initializeMacros();
			i = 0;
			while (i < length) {
				int len = _uncompressMacroCommand(buffer, i, emc);
				if (len == SCREEN_MACRO_FORMAT_ERROR) return;
				_executeMacroCommand(emc, x, y, scaleMul, scaleDiv);
				i = len;
			}
		}
	}
	
	int _parseMacroCommandHexColor(int s[], int n, macroCommand_t mc) {
		int i = n;
		mc.color = 0;
		// remove front spaces
		while ((s[i] != 0) && (s[i] <= ' ')) i++;
		if (s[i] == 0) return SCREEN_MACRO_FORMAT_ERROR;
		// get the digits	
		while ((s[i] != 0) && (s[i] > ' ')) {
			// check for unknown digits
			if ((s[i] < '0') || (s[i] > 'F') || ((s[i] > '9') && (s[i] < 'A'))) return SCREEN_MACRO_FORMAT_ERROR;
			mc.color = (mc.color << 4) + s[i] - (s[i] < 'A' ? '0' :  'A' - 10);
			i++;
		}
		return i-n;
	}

	int _parseMacroCommandParameter(int s[], int n, macroCommand_t mc, int paramId) {
		int i = n;
		boolean negative = false;
		// remove front spaces
		while ((s[i] != 0) && (s[i] <= ' ')) i++;
		// if no value, return error
		if (s[i] == 0) return SCREEN_MACRO_FORMAT_ERROR;
		int value = 0;
		// get the digits	
		if (s[i] == '-') {
			negative = true;
			i++;
		}
		while ((s[i] != 0) && (s[i] > ' ')) {
			// check for unknown digits
			if ((s[i] < '0') || (s[i] > '9')) return SCREEN_MACRO_FORMAT_ERROR;
			value = value * 10 + s[i] - '0';
			i++;
		}
		if (negative) mc.param[paramId] = -value;
		else mc.param[paramId] = value;
		return i-n;
	}

	int _getArcEnd(int radius, int octant, boolean isReversed, boolean isX) {
		int value = (radius * SCREEN_MACRO_COS45_LSH16) >> 16;
		switch (octant) {
			case SCREEN_ARC_SSE:
				if (isReversed && isX) return value;
				if (isReversed) return value;
				if (isX) return 0;
				return radius;
			case SCREEN_ARC_SEE:
				if (isReversed && isX) return radius;
				if (isReversed) return 0;
				if (isX) return value;
				return value;
			case SCREEN_ARC_NEE:
				if (isReversed && isX) return value;
				if (isReversed) return -value;
				if (isX) return radius;
				return 0;
			case SCREEN_ARC_NNE:
				if (isReversed && isX) return 0;
				if (isReversed) return -radius;
				if (isX) return value;
				return -value;
			case SCREEN_ARC_SSW:
				if (isReversed && isX) return 0;
				if (isReversed) return radius;
				if (isX) return -value;
				return value;
			case SCREEN_ARC_SWW:
				if (isReversed && isX) return -value;
				if (isReversed) return value;
				if (isX) return -radius;
				return 0;
			case SCREEN_ARC_NWW:
				if (isReversed && isX) return -radius;
				if (isReversed) return 0;
				if (isX) return -value;
				return -value;
			case SCREEN_ARC_NNW:
				if (isReversed && isX) return -value;
				if (isReversed) return -value;
				if (isX) return 0;
				return -radius;
			case SCREEN_ARC_NE:
				if (isReversed && isX) return 0;
				if (isReversed) return -radius;
				if (isX) return radius;
				return 0;
			case SCREEN_ARC_SE:
				if (isReversed && isX) return radius;
				if (isReversed) return 0;
				if (isX) return 0;
				return radius;
			case SCREEN_ARC_SW:
				if (isReversed && isX) return 0;
				if (isReversed) return radius;
				if (isX) return -radius;
				return 0;
			case SCREEN_ARC_NW:
				if (isReversed && isX) return -radius;
				if (isReversed) return 0;
				if (isX) return 0;
				return -radius;
			case SCREEN_ARC_N:
				if (isReversed && isX) return -radius;
				if (isReversed) return 0;
				if (isX) return radius;
				return 0;
			case SCREEN_ARC_S:
				if (isReversed && isX) return radius;
				if (isReversed) return 0;
				if (isX) return -radius;
				return 0;
			case SCREEN_ARC_E:
				if (isReversed && isX) return 0;
				if (isReversed) return -radius;
				if (isX) return 0;
				return radius;
			case SCREEN_ARC_W:
				if (isReversed && isX) return 0;
				if (isReversed) return radius;
				if (isX) return 0;
				return -radius;
		}
		return 0;
	}

	/*
	 * Compression format for commands
	 *	dependant on command
	 *		cmd param1 .. paramN
	 *
	 * Compression format for numbers
	 *		NSxxxxxx
	 *			N	if N is null, only one byte of data
	 *			S	if S is null, number is positive
	 *		1Snnnnnn nnnnnnnn
	 *			From -16383 [11111111 11111111] to 16383 [10111111 11111111]
	 *		0Snnnnnn
	 *			From -63 [01111111] to 63 [00111111]
	 */
	int _compressMacroCommand(macroCommand_t mc, int buffer[], int bufferPtr) {
		int group = mc.cmd & SCREEN_MACRO_CMD_GROUP_MASK;
		
		int i = bufferPtr;
		buffer[i++] = mc.cmd;
		// presets
		if (group == SCREEN_MACRO_CMD_GROUP_PRESET) {
			if ((mc.cmd == SCREEN_MACRO_CMD_PRESET_FOREGROUND) || (mc.cmd == SCREEN_MACRO_CMD_PRESET_BACKGROUND)) { 
				buffer[i++] = mc.color >> 8;
				buffer[i++] = mc.color & 0x0FF;
			} else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_THICKNESS) { 
				buffer[i++] = (mc.param[SCREEN_MACRO_PARAM_THICKNESS] & 0x07F) +
					(mc.param[SCREEN_MACRO_PARAM_THICKNESS_IS_SCALABLE] != 0 ? 0x80 : 0);
			} else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_FONT) {
				buffer[i++] = (mc.param[SCREEN_MACRO_PARAM_FONT_SIZE] & 0x0F) + 
					(mc.param[SCREEN_MACRO_PARAM_FONT_IS_BOLD] != 0 ? 0x40 : 0) + 
					(mc.param[SCREEN_MACRO_PARAM_FONT_IS_OVERLAY] != 0 ? 0x80 : 0);
			} 	
			if ((mc.cmd == SCREEN_MACRO_CMD_PRESET_SCALE) && (mc.nbParams == 1)) {
				mc.param[SCREEN_MACRO_PARAM_SCALE_DIV] = 1;
				mc.nbParams = 2;
			}
			// for the other presets, we can return, nothing more to do
			// SCREEN_MACRO_CMD_PRESET_ERASE
			else return i;
		} else if (group == SCREEN_MACRO_CMD_GROUP_ARC) {
			// if only radius, indicate that the command is linked with previous one
			if (mc.nbParams == 1) buffer[i-1] |= SCREEN_MACRO_CMD_LINKED_FLAG;
			buffer[i++] = mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT];
		} else if (group == SCREEN_MACRO_CMD_GROUP_LINE) {
			// if only 1 point, indicate that the command is linked with previous one
			if (mc.nbParams == 2) buffer[i-1] |= SCREEN_MACRO_CMD_LINKED_FLAG;
		} else if (group == SCREEN_MACRO_CMD_GROUP_EXECUTE) {
			if (mc.nbParams >= SCREEN_MACRO_MAX_NUMBER) return SCREEN_MACRO_FORMAT_ERROR;
			buffer[i++] = mc.param[SCREEN_MACRO_PARAM_MACRO_NUMBER];
			return i;
		}

		// compress the parameters
		for (int j=0; j<mc.nbParams; j++) {
			int len = _compressNumber(mc.param[j], buffer, i);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		if (mc.cmd == SCREEN_MACRO_CMD_STRING) {
			buffer[i++] = mc.textLen;
			for (int j=0; j<mc.textLen; j++) buffer[i++] = mc.text[j];
		} 
		return i;
	}

	int _uncompressMacroCommand(int buffer[], int n, macroCommand_t mc) {
		int i = n;
		mc.cmd = buffer[i++];
		
		int group = mc.cmd & SCREEN_MACRO_CMD_GROUP_MASK;
		boolean linked = (mc.cmd & SCREEN_MACRO_CMD_LINKED_FLAG) != 0;
		mc.cmd = mc.cmd & ~SCREEN_MACRO_CMD_LINKED_FLAG;
		
		// initialize
		for (int j=0; j<mc.param.length; j++) mc.param[j] = 0;
		// presets
		if (group == SCREEN_MACRO_CMD_GROUP_PRESET) {
			if ((mc.cmd == SCREEN_MACRO_CMD_PRESET_FOREGROUND) || (mc.cmd == SCREEN_MACRO_CMD_PRESET_BACKGROUND)) { 
				mc.color = (buffer[i] << 8) + buffer[i+1];
				return i+2;
			} 
			if (mc.cmd == SCREEN_MACRO_CMD_PRESET_ERASE) return i; 
			if (mc.cmd == SCREEN_MACRO_CMD_PRESET_THICKNESS) { 
				mc.param[SCREEN_MACRO_PARAM_THICKNESS] = buffer[i++];
				if ((mc.param[SCREEN_MACRO_PARAM_THICKNESS] & 0x80) != 0) mc.param[SCREEN_MACRO_PARAM_THICKNESS_IS_SCALABLE] = 1;
				mc.param[SCREEN_MACRO_PARAM_THICKNESS] = mc.param[SCREEN_MACRO_PARAM_THICKNESS] & 0x7F;
				return i;
			}
			if (mc.cmd == SCREEN_MACRO_CMD_PRESET_FONT) {
				mc.param[SCREEN_MACRO_PARAM_FONT_SIZE] = buffer[i++];
				mc.param[SCREEN_MACRO_PARAM_FONT_IS_BOLD] = ((mc.param[SCREEN_MACRO_PARAM_FONT_SIZE] & 0x40) != 0 ? 1 : 0);
				mc.param[SCREEN_MACRO_PARAM_FONT_IS_OVERLAY] = ((mc.param[SCREEN_MACRO_PARAM_FONT_SIZE] & 0x80) != 0 ? 1 : 0);
				mc.param[SCREEN_MACRO_PARAM_FONT_SIZE] = mc.param[SCREEN_MACRO_PARAM_FONT_SIZE] & 0x0F;
				return i;
			} 	
		} else if (group == SCREEN_MACRO_CMD_GROUP_EXECUTE) {
			mc.param[SCREEN_MACRO_PARAM_MACRO_NUMBER] = buffer[i++];
			if (mc.nbParams >= SCREEN_MACRO_MAX_NUMBER) return SCREEN_MACRO_FORMAT_ERROR;
			return i;
		} else if (group == SCREEN_MACRO_CMD_GROUP_ARC) {
			mc.param[SCREEN_MACRO_PARAM_ARC_OCTANT] = buffer[i++];
			if (linked) mc.nbParams = 1;
			else mc.nbParams = 3;
		} else if (group == SCREEN_MACRO_CMD_GROUP_LINE) {
			if (linked) mc.nbParams = 2;
			else mc.nbParams = 4;
		} else if (group == SCREEN_MACRO_CMD_GROUP_RECTANGLE) {
			if (mc.cmd > SCREEN_MACRO_CMD_RECTANGLE_FILLED) mc.nbParams = 5;
			else mc.nbParams = 4;
		} else if (group == SCREEN_MACRO_CMD_GROUP_CIRCLE) {
			mc.nbParams = 3;
		} else if (group == SCREEN_MACRO_CMD_GROUP_TRIANGLE) {
			mc.nbParams = 6;
		} else if (group == SCREEN_MACRO_CMD_GROUP_STRING) {
			mc.nbParams = 2;
		}
	
		for (int j=0; j<mc.nbParams; j++) {
			int len = _uncompressNumber(buffer, i, mc, j);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		if (group == SCREEN_MACRO_CMD_GROUP_STRING) {
			mc.textLen = buffer[i++];
			mc.text = new int[mc.textLen+1];
			mc.text[mc.textLen] = 0;
			for (int j=0; j<mc.textLen; j++) mc.text[j] = buffer[i++];
		}
		return i;
	}

	int _compressNumber(int in, int out[], int n) {
		boolean negative = false;
		if (in < 0) {
			negative = true;
			in = -in;
		}
		if (in > (0x01 << 14)-1) return SCREEN_MACRO_FORMAT_ERROR;
		if (in > (0x01 << 6)-1) {
			// 2 bytes
			out[n] = (in >> 8) | (negative ? (0x01 << 6) : 0x00) | (0x01 << 7);
			out[n+1] = (in & 0x0FF);
			return 2;
		}
		// 1 byte
		out[n] = in | (negative ? (0x01 << 6) : 0x00);
		return 1;
	}


	int _uncompressNumber(int in[], int n,  macroCommand_t mc, int paramId) {
		boolean negative = false;
		if ((in[n] & (0x01 << 6)) != 0) negative = true;
		mc.param[paramId] = in[n] & 0x3F;
		if ((in[n] & (0x01 << 7)) != 0) {
			mc.param[paramId] = (mc.param[paramId] << 8) | in[n+1];
			if (negative) mc.param[paramId] = -mc.param[paramId];
			return 2;
		}
		if (negative) mc.param[paramId] = -mc.param[paramId];
		return 1;	
	}

}
