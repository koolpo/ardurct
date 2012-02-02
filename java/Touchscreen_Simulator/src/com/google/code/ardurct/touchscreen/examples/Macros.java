package com.google.code.ardurct.touchscreen.examples;

import com.google.code.ardurct.touchscreen.Example;

public class Macros extends Example {

	String seven = "l 10 10 30 10 l 30 17 arnww 38";
	
	String octant[] = { "nww", "nnw", "nne", "nee", "see", "sse", "ssw", "sww" };
	
	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
	    touchscreen.setBacklight(180);
	    /*
	    // draw in blue, with a line thickness of 2
	    touchscreen.executeMacro("pt 2 pc 1F", 0, 0, 3, 1, true, false);
	    // draw a seven
	    touchscreen.executeMacro(seven, 0, 0, 3, 1, true, true);
	    // write seven into eeprom for future use, in slot 0
	    touchscreen.executeMacro("w 0 " + seven, 0, 0, 3, 1, true, true);
	    // set foreground to red
	    touchscreen.executeMacro("pc F800", 0, 0, 3, 1, true, true);
	    // draw the macro stored in eeprom in slot 0
	    touchscreen.executeMacro("e 0", 0, 160, 3, 1, true, true);
	    */

	    touchscreen.fillRectangle(0, 0, touchscreen.getWidth(),  touchscreen.getHeight(), WHITE);
	    touchscreen.executeMacro("pt 2 pc 1F l 10 160 10 160", 0, 0, 1, 1, false, false);
	    int d = 0;
	    String spiral = "";
	    for (int i=110; i>0; i-=1) {
	    	spiral += (d > 0 ? " " : "" ) + "a" + octant[d++] + " " + i;
	    	if (d == 8) {
	    	    touchscreen.executeMacro(spiral, 0, 0, 1, 1, false, true);	    		
	    		d = 0;
	    		spiral = "";
	    	}
	    }

	}

	public void loop() {
	    delay(1000);
	}

}
