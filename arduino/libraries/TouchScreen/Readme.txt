In most cases, the only file you may have to change is configuration.hpp
The definitions of that file can be overriden when you initialize the class in your *.ino file

Content
=======
Classes
-------
ScreenPL.*							Physical layer
	S6D04H0.*						Derives from ScreenPL and manages the S6D04H0 chip
	ILI932X.*						Derives from ScreenPL and manages the ILI9325 or ILI9328 chip
		ScreenHAL.*					Derives from one other class above and do all the abstract work  
			Touchscreen.* 			Derives from ScreenHAL implements the touchpanel elements
				(UserInterface.*)	Contains the functions to draw UserInterface elements (part of Touchscreen class)
				(Macros.*)			Contains the functions to draw macros (part of Touchscreen class)

Headers
-------
configuration.hpp	Defines the hardware connections and the software options
colors.hpp			Defines some constant colors
fonts.hpp			Describes some constants and structs for the fonts
fontBitmaps.hpp		Defines the patterns for the fonts
eepromUtils.hpp		Utilities to write several bytes at once to EEPROM

fonts
=====
The folder contains some png images of various fonts
And a java class to generate the structure that need to be included in fontBitmaps

