package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIListItem extends ArduRCT_GraphicsUIButton 
implements IGraphicsDefines {
	
	public ArduRCT_GraphicsUIListItem(int id, int[] label, IUIActionCallback actionHandler) {
		_init(id, -1, label, null, actionHandler);
	}
	
	public ArduRCT_GraphicsUIListItem(int id, IUIDrawCallback drawHandler, IUIActionCallback actionHandler) {
		_init(id, -1, null, drawHandler, actionHandler);
	}

	protected int _drawBorder(ArduRCT_Graphics graphics, int uiX, int uiY, int color) {
		if (_state == GRAPHICS_UI_SELECTED || _state == GRAPHICS_UI_RELEASED) color = LIGHT_GREY;
		graphics.fillRectangle(uiX+x, uiY+y, width, height, color);
		int bColor = BLACK;
		if (_state == GRAPHICS_UI_SELECTED || _state == GRAPHICS_UI_RELEASED) bColor = GRAPHICS_UI_COLOR_HIGHLIGHTED;
		graphics.drawRectangle(uiX+x, uiY+y, width, height, bColor, 1);
		return color;
	}
	
	public void placeLabel(ArduRCT_Graphics graphics) {
		if (_text == null) return; 
		int fontSize = getFontSize(_text);
		_textX = GRAPHICS_UI_ELEMENT_LEFT_MARGIN;
		_textY = (height-graphics.getFontHeight(fontSize))/2;
	}	
	
	// called when the item is selected (enter is pressed, or item is touched)
	public ArduRCT_GraphicsUIElement press() {
		_state = GRAPHICS_UI_SELECTED;
		return null;
	}

}
