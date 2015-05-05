![http://ardurct.googlecode.com/svn/images/LCD_PCD8544_library.jpg](http://ardurct.googlecode.com/svn/images/LCD_PCD8544_library.jpg)

[![](http://www.gstatic.com/codesite/ph/images/dl_arrow.gif)](http://code.google.com/p/ardurct/downloads/list) [Download](http://code.google.com/p/ardurct/downloads/list)

### Content ###
This archive contains the Eclipse source for a simulator of the LCD. This facilitates the development of screen layouts for the display.

The idea is to write code in java as if it was code for Arduino and then copy and paste the code in Arduino and obtain the same result on the hardware. That is to say: fill in a setup and a loop function.

### Usage ###
  1. Duplicate one of the files under the tests folder, such as Tests,
  1. Modify it to suit your needs,
  1. Run the 'Simulator' application, and choose from the menu your test.
  1. Copy the content of setup and loop into your Arduino code.
<u>Example</u><br>
To produce the image above, use the following code.<br>
<pre><code>package com.google.code.ardurct.lcd_pcd8544_simulator.tests;<br>
<br>
import com.google.code.ardurct.lcd_pcd8544_simulator.LCDTest;<br>
<br>
public class Tests extends LCDTest {<br>
	<br>
	private int header[] = { <br>
		0x5F, 0x45, 0x5B, 0x40, 0x54, 0x48, 0x54, 0x40, 0x40, 0x50, 0x50, 0x50, 0x58, 0x58, 0x58, 0x5C,<br>
		0x5C, 0x5C, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x40, 0x40, 0x40, 0x7F, 0x40, 0x40, 0x5F, 0x45, <br>
		0x5B, 0x40, 0x5F, 0x55, 0x51, 0x40, 0x5F, 0x42, 0x5F, 0x40, 0x40, 0x5F, 0x51, 0x51, 0x51, 0x51, <br>
		0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, <br>
		0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, <br>
		0x51, 0x5F, 0x4E, 0x4E };<br>
	<br>
	public void setup() {<br>
		lcdT.begin();<br>
		lcdT.drawBitmap(header, 0, 0, 84, 8);<br>
		lcdT.setCursor(0, 2);<br>
		lcdT.print("Ya");<br>
		lcdT.setCursor(0, 3);<br>
		lcdT.print("Th");<br>
		lcdT.drawProgressBar(2, 2, 12, 50, false);<br>
		lcdT.drawProgressBar(2, 3, 12, 80, true);<br>
		<br>
		lcdG.begin();<br>
		lcdG.setCursor(0, 0);<br>
		lcdG.print("Hello world");<br>
		lcdG.drawSmallFontText("Hello world", 0, 10);<br>
		lcdG.setCursor(7, 3);<br>
		lcdG.print("8:20:35");<br>
		lcdG.drawMediumFontTime(8, 20, 35, 50, 33);<br>
		lcdG.drawSmallFontText("8:20:35", 56, 42);<br>
		lcdG.drawEllipse(10, 30, 9, 8, true);<br>
		lcdG.drawLine(10, 30, 20, 48, true, 2);<br>
		lcdG.fillRectangle(23, 20, 28, 28, true);<br>
		lcdG.updateDisplay();<br>
	}<br>
	<br>
	public void loop() {<br>
	}<br>
}<br>
</code></pre>