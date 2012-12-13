package com.google.code.ardurct.libraries.eventManager;

public interface IEventCallback {
	
	public boolean handle(int type, int value, int x, int y);
}
