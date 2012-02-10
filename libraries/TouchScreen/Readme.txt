In most cases, the only file you may have to change is hardware.hpp
The definitions of that file can be overriden when you initialize the class in your *.ino file

Content
=======
Classes
-------
ScreenHAL.*			All the abstract work is done in 
	Macros.*		Contains the functions to draw macros, belongs to the ScreenHAL class
	UserInterface.*	Contains the functions to draw UserInterface elements
Touchscreen.* 		Derives from one of the class below and implements the touchpanel elements
	S6D04H0.*		Derives from ScreenHAL and manages the S6D04H0 chip
	ILI932X.*		Derives from ScreenHAL and manages the ILI9325 or ILI9328 chip

Headers
-------
hardware.hpp		Defines the hardware connections 
colors.hpp			Defines some constant colors
fonts.hpp			Describes some constants and structs for the fonts
fontBitmaps.hpp		Defines the patterns of the fonts
eepromUtils.hpp		Utilities to write several bytes at once to EEPROM

fonts
=====
The folder contains some png images of various fonts
And a java class to generate the structure that need to be included in fontBitmaps

