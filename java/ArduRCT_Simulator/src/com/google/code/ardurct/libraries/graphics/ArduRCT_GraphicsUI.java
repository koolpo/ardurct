package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUI extends ArduRCT_GraphicsMacros {

	ArduRCT_GraphicsUIScreen _homeScreen;
	ArduRCT_GraphicsUIScreen _screen;
	ArduRCT_GraphicsUIElement _homeElement;
	
	public void addScreen(ArduRCT_GraphicsUIScreen screen) {
		screen.setGraphics(this);
	}
	
	public void setGraphicsUIScreen(ArduRCT_GraphicsUIScreen screen) {
		setGraphicsUIScreen(screen, null);
	}

	public void setGraphicsUIScreen(ArduRCT_GraphicsUIScreen screen, ArduRCT_GraphicsUIScreen returnToScreen) {
		if (_screen == screen) return;
		if (_screen != null) _screen.deactivate();
		_screen = screen;	
		_screen.setPreviousScreen(returnToScreen == null ? _homeScreen : returnToScreen);
		_screen.activate();
	}

	public void setGraphicsUIHome(ArduRCT_GraphicsUIScreen screen, ArduRCT_GraphicsUIElement element) {
		_homeScreen = screen;
		_homeElement = element;
	}
	
	public boolean handleGraphicsUI(int actionId, int x, int y) {
		if (_screen == null) return false;
		if (_screen.process(actionId, x, y)) return true;	
		if (actionId == GRAPHICS_UI_ACTION_MENU) {
			if (_homeElement != null) {
				_homeElement.press();
				_homeElement.release();
				_homeElement.unselect();
			}
			setGraphicsUIScreen(_screen.getPreviousScreen());
			return true;
		}
		return false;
	}
	
}
