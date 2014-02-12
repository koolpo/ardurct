package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUILabel extends ArduRCT_GraphicsUIElement {
	
	int _text[];
	int _textX;
	int _textY;
	int _fontSize;
	
	public ArduRCT_GraphicsUILabel(int[] label) {
		this(-1, label, GRAPHICS_UI_ELEMENT_FONT);
	}

	public ArduRCT_GraphicsUILabel(int id, int[] label) {
		this(id, label, GRAPHICS_UI_ELEMENT_FONT);
	}

	public ArduRCT_GraphicsUILabel(int[] label, int fontSize) {
		this(-1, label, fontSize);
	}

	public ArduRCT_GraphicsUILabel(int id, int[] label, int fontSize) {
		_fontSize = fontSize;
		_id = id;
		_text = label;
		editable = false;
	}

	public void setText(int[] text) {
		_text = text;
	}

	public int[] getText() {
		return _text;
	}
	
	public void draw(ArduRCT_Graphics graphics, int uiX, int uiY, int uiWidth) {
		graphics.fillRectangle(x+uiX, y+uiY, width-1, height-1, graphics.getBackgroundColor());
		graphics.drawString(_text, uiX+x+_textX, uiY+y+_textY, BLACK, _fontSize, _fontSize == FONT_SMALL ? false : GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, false);
	}
	
	public void autoSize(ArduRCT_Graphics graphics) {
		if (_text == null) return;
		int tW[] = new int[1];
		int tH[] = new int[1];
		graphics.getStringBoundingBox(_text, _fontSize,  _fontSize == FONT_SMALL ? false : GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, 
				GRAPHICS_UI_ELEMENT_LEFT_MARGIN, tW, tH);
		height = tH[0] + GRAPHICS_UI_ELEMENT_TOP_MARGIN * 2;
		width =  tW[0];
	}
	
	public void placeLabel(ArduRCT_Graphics graphics) {
		if (_text == null) return; 
		_textX = GRAPHICS_UI_ELEMENT_LEFT_MARGIN/2;
		int tW[] = new int[1];
		int tH[] = new int[1];
		graphics.getStringBoundingBox(_text, _fontSize,  _fontSize == FONT_SMALL ? false : GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, 
				GRAPHICS_UI_ELEMENT_LEFT_MARGIN/2, tW, tH);
		_textY = (height-tH[0])/2;
	}
}
