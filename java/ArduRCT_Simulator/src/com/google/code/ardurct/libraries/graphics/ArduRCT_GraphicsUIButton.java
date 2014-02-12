package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIButton extends ArduRCT_GraphicsUIElement 
implements IGraphicsDefines {

	int _text[];
	int _textX;
	int _textY;
	
	public ArduRCT_GraphicsUIButton() {
	}
	
	public ArduRCT_GraphicsUIButton(int id, int[] label, IUIActionCallback actionHandler) {
		_init(id, -1, label, null, actionHandler);
	}
	
	public ArduRCT_GraphicsUIButton(int id, IUIDrawCallback drawHandler, IUIActionCallback actionHandler) {
		_init(id, -1, null, drawHandler, actionHandler);
	}

	public void _init(int id, int group, int[] label, IUIDrawCallback drawHandler, IUIActionCallback actionHandler) {
		_id = id;
		_group = group;
		_text = label;
		_state = GRAPHICS_UI_RELEASED;
		_value = GRAPHICS_UI_RELEASED;
		editable = true;
		_actionHandler = actionHandler;
		_drawHandler = drawHandler;
	}
	
	public void autoSize(ArduRCT_Graphics graphics) {
		if (_text == null) return; 
		int fontSize = getFontSize(_text);
		height = graphics.getFontHeight(fontSize) + GRAPHICS_UI_ELEMENT_TOP_MARGIN * 2;
		width =  graphics.getStringWidth(_text, fontSize) + GRAPHICS_UI_ELEMENT_LEFT_MARGIN * 2;
	}
	
	public void placeLabel(ArduRCT_Graphics graphics) {
		if (_text == null) return; 
		int fontSize = getFontSize(_text);
		_textX = (width - graphics.getStringWidth(_text, fontSize))/2;
		_textY = (height-graphics.getFontHeight(fontSize))/2;
	}
	
	public void draw(ArduRCT_Graphics graphics, int uiX, int uiY, int uiWidth) {
		int bgColor = graphics.getBackgroundColor();
		int color = GRAPHICS_UI_COLOR_RELEASED;
		if (_value == GRAPHICS_UI_SELECTED) color = GRAPHICS_UI_COLOR_PRESSED;
		color = _drawBorder(graphics, uiX, uiY, color);
		graphics.setBackgroundColor(color);
		if (_text != null) {
			int fontSize = getFontSize(_text);
			graphics.drawString(_text, uiX+x+_textX, uiY+y+_textY, editable ? BLACK : GRAPHICS_UI_COLOR_PRESSED, fontSize, GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, false);
		} else if (_drawHandler != null) {
			 _drawHandler.draw(_id, _state, _value, x+uiX, y+uiY, width, height);
		}
		graphics.setBackgroundColor(bgColor);
	}
	
	protected int _drawBorder(ArduRCT_Graphics graphics, int uiX, int uiY, int color) {
		graphics.fillRoundedRectangle(uiX+x, uiY+y, width, height, GRAPHICS_UI_ELEMENT_TOP_MARGIN*75/100, color);
		graphics.drawRoundedRectangle(uiX+x, uiY+y, width, height, GRAPHICS_UI_ELEMENT_TOP_MARGIN*75/100, 
				_state == GRAPHICS_UI_HIGHLIGHTED ? GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK, 1);	
		return color;
	}
		
	public ArduRCT_GraphicsUIElement enter() {
		_value = GRAPHICS_UI_SELECTED;
		return null;
	}

	public boolean release() {
		_value = GRAPHICS_UI_RELEASED;
		return true;
	}
}
