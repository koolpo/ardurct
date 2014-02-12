package com.google.code.ardurct.libraries.graphics.examples;

import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;
import com.google.code.ardurct.libraries.graphics.GraphicsFirmware;

public class MacroTests extends GraphicsFirmware {

	ArduRCT_JAVA graphics = new ArduRCT_JAVA();
	
	// size 23x37
	String seven = "ld 0 0 22 0 ld 0 9 arnww 38";
	String eight = "ld 10 0 2 0 ae 8 ld -2 0 ld 2 0 ae 10 ld -2 0 aw 10 aw 8";
	
	String octant[] = { "nww", "nnw", "nne", "nee", "see", "sse", "ssw", "sww" };
	boolean spiralDone = false;
	
	public void setup() {
	    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
	    graphics.setBacklight(180);
	    
	    // draw in blue, with a scalable line thickness of 2
	    graphics.executeMacro("pts 2 pc 1F");
	    // draw a seven
	    graphics.executeMacro(seven, 10, 10, 3, 1, true);
	    // write eight into eeprom for future use, in slot 0
	    graphics.executeMacro("w 0 " + eight);
	    // write hello into eeprom for future use, in slot 1
	    graphics.executeMacro("w 1 s 0 0 \"Hello\"");
	    // set foreground to red
	    graphics.executeMacro("pc F800 pts 2");
	    // draw the macro stored in eeprom in slot 0
	    graphics.executeMacro("e 0", 120, 10, 3, 1, true);
	    // draw the macro stored in eeprom in slot 1
	    // draw in size 1 black
	    graphics.executeMacro("pf 1 pc 0 e 1", 10, 160, 1, 1, true);
	    // draw in size 2 bold gray
	    graphics.executeMacro("pfb 2 pc " + Integer.toHexString(GREY) + " e 1", 10, 180, 1, 1, true);
	}

	public void loop() {
	    delay(5000);

	    if (!spiralDone) {
		    graphics.executeMacro("pe pt 3 pc 1F ld 10 160 0 0", 0, 0, 1, 1, false);
		    int d = 0;
		    String spiral = "";
		    for (int i=110; i>0; i-=2) {
		    	spiral += (d > 0 ? " " : "" ) + "a" + octant[d++] + " " + i;
		    	if (d == 8) {
		    	    graphics.executeMacro(spiral, 0, 0, 1, 1, true);	    		
		    		d = 0;
		    		spiral = "";
		    	}
		    }
		    spiralDone = true;
	    }

	}

}
