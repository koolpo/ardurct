In most cases, the only file you may have to change is configuration.hpp
The definitions of that file can be overriden when you initialize the class in your *.ino file

Content
=======
Classes
-------
ArduRCT_Graphics*			High level drawing functions (includes GraphicsMacros.* if GRAPHICS_HAS_MACROS is true and GraphicsUI.* if GRAPHICS_HAS_UI is true)
	ArduRCT_S6D04H0.*		Low level drawing functions for S6D04H0
	ArduRCT_ST7735.*		Low level drawing functions for ST7735

Headers
-------
CArduRCT_olors.hpp			Defines some constant colors
ArduRCT_Fonts.hpp			Describes some constants and structs for the fonts
ArduRCT_FontBitmaps.hpp		Defines the patterns for the fonts
ArduRCT_EepromUtils.hpp		Utilities to write several bytes at once to EEPROM


fonts
=====
The folder contains some png images of various fonts
And a java class to generate the structure that need to be included in fontBitmaps

