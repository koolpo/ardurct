package com.google.code.ardurct.libraries.eventManager;

public interface IEventCallback {
	
	public int handle(int type, int value, int x, int y);
	
	public int handle(int type, int value);
	
	public int handle(int type);
}
