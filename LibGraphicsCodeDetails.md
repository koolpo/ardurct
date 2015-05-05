# Arduino library for graphic screens - Code details #
## Organisation ##
The library is organized into layers.<br><br>
<img src='http://ardurct.googlecode.com/svn/images/ArduRCT_Graphics.png'><br>
<ul><li>The Physical layer defines the implementation of the basic drawing functions at the hardware level:<br>
<pre><code>void initScreenImpl();<br>
void fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);<br>
uint16_t *retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);<br>
void pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);<br>
void setRotationImpl(uint8_t rotation);<br>
void drawPixelImpl(uint16_t x, uint16_t y, uint16_t color);<br>
void selectScreenImpl();<br>
void unselectScreenImpl();<br>
</code></pre>
</li></ul><blockquote>Several implementation of the physical layer exist, you only have to use one:<br>
<ul><li>ArduRCT_S6D04H0<br>
</li><li>ArduRCT_ST7735<br>
</li></ul></blockquote><ul><li>The Hardware Abstraction Layer manages the drawing functions such as: circles, lines, patterns, fonts, bitmaps. It is called ArduRCT_Graphics. To make the code more readable:<br>
<ul><li>the macros functions have been put in ArduRCT_GraphicsMacros.<br>
<br>
<h2>Inheritance</h2>
<a href='http://code.google.com/p/ardurct/source/browse/trunk/arduino/libraries/ArduRCT_Graphics/ArduRCT_Graphics.cpp'>ArduRCT_Graphics</a> (<a href='http://code.google.com/p/ardurct/source/browse/trunk/arduino/libraries/ArduRCT_Graphics/ArduRCT_GraphicsMacros.cpp'>ArduRCT_GraphicsMacros</a>)<br>
<img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='15' height='15'><img src='http://ardurct.googlecode.com/svn/images/inherit.gif'><a href='http://code.google.com/p/ardurct/source/browse/trunk/arduino/libraries/ArduRCT_Graphics/ArduRCT_S6D04H0.cpp'>ArduRCT_S6D04H0</a><br>
<img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='15' height='15'><img src='http://ardurct.googlecode.com/svn/images/inherit.gif'><a href='http://code.google.com/p/ardurct/source/browse/trunk/arduino/libraries/ArduRCT_Graphics/ArduRCT_ST7735.cpp'>ArduRCT_ST7735</a>