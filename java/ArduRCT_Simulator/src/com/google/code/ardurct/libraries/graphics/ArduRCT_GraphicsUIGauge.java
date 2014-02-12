package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIGauge extends ArduRCT_GraphicsUIElement {
	
	int _max;
	int _min;
	
	public ArduRCT_GraphicsUIGauge(int id, int value, int min, int max) {
		_id = id;
		_value = value;
		_min = min;
		_max = max;
		editable = false;
	}
	
	public void draw(ArduRCT_Graphics graphics, int uiX, int uiY, int uiWidth) {
		if (width > height) {
			// horizontal gauge
			int fill = _value*width/(_max-_min);
			if (fill > 0) graphics.fillRectangle(uiX+x, uiY+y, fill, height, GRAPHICS_UI_COLOR_GAUGE);
			graphics.fillRectangle(uiX+x+fill, uiY+y, width-fill, height, WHITE);
		} else {
			// vertical gauge
			int fill = _value*height/(_max-_min);
			if (fill > 0) graphics.fillRectangle(uiX+x, uiY+y+height-fill, width, fill, GRAPHICS_UI_COLOR_GAUGE);
			graphics.fillRectangle(uiX+x, uiY+y, width, height-fill, WHITE);
		}
		graphics.drawRectangle(uiX+x, y+uiY, width, height, BLACK, 1);
	}
		
	public ArduRCT_GraphicsUIElement setValue(int value) {
		_value = value;
		if (_value > _max) _value = _max;
		if (_value < _min) _value = _min;
		return null;	
	}
}
