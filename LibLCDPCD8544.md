# Arduino library for LCD\_PCD8544 #
<img src='http://ardurct.googlecode.com/svn/images/LCD_PCD8544.jpg' width='300'>

<a href='http://code.google.com/p/ardurct/downloads/list'><img src='http://www.gstatic.com/codesite/ph/images/dl_arrow.gif' /></a> <a href='http://code.google.com/p/ardurct/downloads/list'>Download</a>

<h3>Content</h3>
The library contains code to drive one or several LCD displays at the same time (see the dual screen example below) such as a <a href='http://www.sparkfun.com/products/10168'>Nokia5100 LCD</a>.<br>
This library contains 2 drivers:<br>
<ul><li>a text based driver <a href='http://code.google.com/p/ardurct/source/browse/trunk/libraries/LCD_PCD8544/LCD_PCD8544.h'>&lt;LCD_PCD8544.h&gt;</a> to print simple texts on the screen.<br>
<ul><li>can act as a terminal (including new lines)<br>
</li><li>can draw progress bars and toggle switches<br>
</li><li>allows definition and printing of glyphs<br>
</li></ul></li><li>a vector graphic driver <a href='http://code.google.com/p/ardurct/source/browse/trunk/libraries/LCD_PCD8544/LCD_PCD8544.h'>&lt;LCD_PCD8544_VG.h&gt;</a> to draw text and graphical elements.<br>
<ul><li>can draw or fill points, lines, circles, rectangles and ellipses.<br>
</li><li>can draw lines of various thickness.<br>
</li><li>can draw text with 3 different font size:<br>
<ul><li>3x5, complete: lowercase letters are transformed in uppercase<br>
</li><li>4x6, only numbers and space (see drawMediumFontTime and drawMediumFontValueRA in LCD_PCD8544_VG.h)<br>
</li><li>5x7, complete<br>
The 2 drivers co-exist because the graphic one will take 504 bytes of RAM to store the screen. The text one will not take any but still allows limited graphics through bitmaps.</li></ul></li></ul></li></ul>

<h3>Pin mapping</h3>
<table><thead><th><b>Arduino</b></th><th>+3.3v </th><th>GND </th><th>3  </th><th>4  </th><th>5  </th><th>6  </th><th>7  </th></thead><tbody>
<tr><td><b>LCD</b></td><td>VCC </td><td> GND </td><td>SCLK </td><td>SDIN </td><td>DC </td><td>RESET </td><td>SCE </td></tr></tbody></table>

This default mapping can be overwritten by calling<br>
<code>LCD_PCD8544(uint8_t sce, uint8_t sclk, uint8_t sdin, uint8_t dc, uint8_t reset);</code>
at initialization with as many parameters as required by your design.<br>
<br>
It can also be changed in LCD_PCD8544_Config.cpp.<br>
<br>
<h3>Example usage</h3>
<img src='http://ardurct.googlecode.com/svn/images/LCD_PCD8544_library.jpg' /><br>
<sub>Note: this screen copy is obtained with the </sub><a href='http://code.google.com/p/ardurct/downloads/list'>LCD_PCD8544_Simulator</a>
<pre><code>#include &lt;LCD_PCD8544.h&gt;<br>
#include &lt;LCD_PCD8544_VG.h&gt;<br>
<br>
LCD_PCD8544_VG lcdG(7);    // uses D7 as SCE<br>
LCD_PCD8544 lcdT(8);       // uses D8 as SCE<br>
<br>
uint8_t header[] = { <br>
    0x5F, 0x45, 0x5B, 0x40, 0x54, 0x48, 0x54, 0x40, 0x40, 0x50, 0x50, 0x50, 0x58, 0x58, 0x58, 0x5C,<br>
    0x5C, 0x5C, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x40, 0x40, 0x40, 0x7F, 0x40, 0x40, 0x5F, 0x45, <br>
    0x5B, 0x40, 0x5F, 0x55, 0x51, 0x40, 0x5F, 0x42, 0x5F, 0x40, 0x40, 0x5F, 0x51, 0x51, 0x51, 0x51, <br>
    0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, <br>
    0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, <br>
    0x51, 0x5F, 0x4E, 0x4E };<br>
<br>
void setup() {<br>
    lcdG.begin();<br>
    lcdG.setCursor(0, 0);<br>
    lcdG.print("Hello world");<br>
    lcdG.drawSmallFontText("Hello world", 0, 10);<br>
    lcdG.setCursor(7, 3);<br>
    lcdG.print("8:20:35");<br>
    lcdG.drawMediumFontTime(8, 20, 35, 50, 33);<br>
    lcdG.drawSmallFontText("8:20:35", 56, 42);<br>
    lcdG.drawEllipse(10, 30, 9, 8, ON);<br>
    lcdG.drawLine(10, 30, 20, 48, ON, 2);<br>
    lcdG.fillRectangle(23, 20, 28, 28, ON);<br>
    lcdG.updateDisplay();<br>
<br>
    lcdT.begin();<br>
    lcdT.drawBitmap(header, 0, 0, 84, 8);<br>
    lcdT.setCursor(0, 2);<br>
    lcdT.print("Ya");<br>
    lcdT.drawProgressBar(2, 2, 12, 50, LCD_PCD8544_PROGRESS_BAR_STYLE_LINE);<br>
    lcdT.setCursor(0, 3);<br>
    lcdT.print("Th");<br>
    lcdT.drawProgressBar(2, 3, 12, 80, LCD_PCD8544_PROGRESS_BAR_STYLE_FILLED);<br>
    lcdT.setCursor(0, 5);<br>
    lcdT.print("Au");<br>
    lcdT.drawToggleSwitch(2,5, 12, 4, 1);<br>
}<br>
<br>
void loop() {<br>
}<br>
</code></pre>