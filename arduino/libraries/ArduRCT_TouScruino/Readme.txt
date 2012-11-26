In most cases, the only file you may have to change is configuration.hpp
The definitions of that file can be overriden when you initialize the class in your *.ino file

Content
=======
Classes
-------
Graphics*						Graphics functions (includes Graphics_Macros.* if CONFIGURATION_HAS_MACROS is true and Graphics_UI.* if CONFIGURATION_HAS_UI is true)
	TouchScreen*				Touch panel management functions 
		S6D0H40					LCD driver chip functions
			TouScruinoV2*		Graphics functions implementations specific to TouScruino-v2
		ILI9325					LCD driver chip functions
			TouScruinoV3*		Graphics functions implementations specific to TouScruino-v3
	ButtonScreen*				Button management function
		PCD8544					LCD driver chip functions
			TouScruinoV0*		All glued together for TouScruino-v0 
		ST7735					LCD driver chip functions
			TouScruinoV1*		All glued together for TouScruino-v1 

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

