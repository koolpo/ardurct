package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIOption extends ArduRCT_GraphicsUIButton 
implements IGraphicsDefines {

	public ArduRCT_GraphicsUIOption() {	
	}
	
	public ArduRCT_GraphicsUIOption(int id, int[] label, IUIActionCallback actionHandler, int group) {
		_init(id, group, label, null, actionHandler);
	}
	
	public ArduRCT_GraphicsUIOption(int id, IUIDrawCallback drawHandler, IUIActionCallback actionHandler, int group) {
		_init(id, group, null, drawHandler, actionHandler);
	}
	
	protected int _drawBorder(ArduRCT_Graphics graphics, int uiX, int uiY, int color) {
		// a pushbutton
		graphics.fillRectangle(uiX+x, uiY+y, width, height, color);
		int bColor = BLACK;
		if (_state == GRAPHICS_UI_SELECTED || _state == GRAPHICS_UI_RELEASED) bColor = GRAPHICS_UI_COLOR_HIGHLIGHTED;
		graphics.drawRectangle(uiX+x, uiY+y, width, height, bColor, 1);		
		return color;
	}
	
	public ArduRCT_GraphicsUIElement setValue(int value) {
		_value = value;
		if ((_group == GRAPHICS_UI_NO_GROUP) || (value != GRAPHICS_UI_PRESSED)) return null;
		
		// search backward for an element in the same group
		ArduRCT_GraphicsUIElement elt = getPrevious();
		while (elt != null) {
			// is there is another selected in the same group
			if ((elt.getGroup() == _group) && (elt.getValue() == GRAPHICS_UI_PRESSED)) {
				// release it
				elt.setValue(GRAPHICS_UI_RELEASED);
				elt.setState(GRAPHICS_UI_UNSELECTED);
				// return it for redraw
				return elt;
			}
			elt = elt.getPrevious();
		}
		// search forward for an element in the same group
		elt = getNext();
		while (elt != null) {
			// is there is another selected in the same group
			if ((elt.getGroup() == _group) && (elt.getValue() == GRAPHICS_UI_PRESSED)) {
				// release it
				elt.setValue(GRAPHICS_UI_RELEASED);
				elt.setState(GRAPHICS_UI_UNSELECTED);
				// return it for redraw
				return elt;
			}
			elt = elt.getNext();
		}
		return null;
	}

	public ArduRCT_GraphicsUIElement press() {
		if (_group != GRAPHICS_UI_NO_GROUP) return setValue(GRAPHICS_UI_PRESSED);
		if (_value == GRAPHICS_UI_PRESSED) setValue(GRAPHICS_UI_RELEASED);
		else setValue(GRAPHICS_UI_PRESSED);
		return null;
	}

	public boolean release() {
		return true;
	}
}
