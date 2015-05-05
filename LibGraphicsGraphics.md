<img src='http://ardurct.googlecode.com/svn/images/Graphics128x160.png' align='right'>
<h3>Description</h3>
The library can draw graphics such as:<br>
<ul><li>Texts: 4 different size, bold and overlay<br>
</li><li>Lines: variable thickness.<br>
</li><li>Triangles: filled or not, with variable thickness.<br>
</li><li>Rectangles: filled or not, with variable thickness.<br>
</li><li>Rounded rectangles: filled or not, with variable thickness.<br>
</li><li>Arcs: filled or not, with variable thickness.<br>
</li><li>Circles: filled or not, with variable thickness.<br>
</li><li>Bitmaps<br>
</li><li>Patterns: scaled or not<br>
</li><li>Big digits: width, height, thickness and style can be controlled<br>
It can also rotate the screen.<br>
After rotation, all subsequent drawings will be done rotated.<br>
But the existing drawing will not be rotated.<br>
<h3>Code</h3>
To produce the Graphics image above, use the following code.<br>
<pre><code>#include &lt;SPI.h&gt;<br>
#include &lt;ArduRCT_ST7735.h&gt;<br>
<br>
ArduRCT_ST7735 graphics(10, 9 , 8);    // graphics(CD, CS, RESET)<br>
<br>
uint16_t wait = 3000;<br>
<br>
void setup() {<br>
    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);<br>
}<br>
<br>
void loop() {<br>
    // Fonts<br>
    graphics.drawString("SMALL", 5, 4, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawString("BOLD", 5, 14, BLACK, FONT_SMALL, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawString("MEDIUM", 5, 30, RED, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawString("BOLD", 5, 44, RED, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawString("BIG", 5, 64, BROWN, FONT_BIG, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawString("BOLD", 5, 84, BROWN, FONT_BIG, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawString("HUGE", 5, 110, BLUE, FONT_HUGE, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawString("BOLD", 5, 134, BLUE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);<br>
    delay(wait);<br>
    <br>
    // Lines<br>
    graphics.fillScreen(WHITE);<br>
    graphics.drawString("Lines", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawLine(10, 30, 104, 69, RED, 2);<br>
    graphics.drawLine(12, 92, 81, 81, BLUE, 1);<br>
    graphics.drawLine(15, 145, 120, 105, GREEN, 5);<br>
    delay(wait);<br>
    <br>
    // Triangles<br>
    graphics.fillScreen(WHITE);<br>
    graphics.drawString("Triangles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawTriangle(7, 31, 71, 30, 20, 63, BLUE, 1);<br>
    graphics.drawString("Thickness 3", 50, 65, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawTriangle(17, 92, 87, 80, 109, 106, DARK_GREEN, 3);<br>
    graphics.drawString("Filled", 12, 145, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillTriangle(18, 112, 13, 136, 94, 128, ORANGE);<br>
    delay(wait);<br>
    <br>
    // Corners<br>
    graphics.fillScreen(WHITE);<br>
    graphics.drawString("Corners", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.fillCorner(74, 90, 40, GRAPHICS_CORNER_SE, RED);<br>
    graphics.drawString("SE", 100, 120, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillCorner(54, 90, 35, GRAPHICS_CORNER_SW, ORANGE);<br>
    graphics.drawString("SW", 10, 115, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillCorner(74, 70, 45, GRAPHICS_CORNER_NE, BLUE);<br>
    graphics.drawString("NE", 110, 40, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillCorner(54, 70, 30, GRAPHICS_CORNER_NW, GREY);<br>
    graphics.drawString("NW", 20, 40, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    delay(wait);<br>
    <br>
    // Rectangle<br>
    graphics.fillScreen(WHITE);<br>
    graphics.drawString("Rectangles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawRectangle(12, 28, 40, 50, GREY, 3);<br>
    graphics.fillRectangle(65, 30, 50, 25, ORANGE);<br>
    graphics.drawString("Filled", 70, 60, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawRoundedRectangle(10, 97, 55, 32, 6, RED, 2);<br>
    graphics.drawString("Rounded", 15, 136, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillRoundedRectangle(80, 80, 33, 48, 8, BLUE);<br>
    graphics.drawString("Rounded", 78, 136, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawString("filled", 78, 146, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    delay(wait);<br>
    <br>
    // Arcs<br>
    graphics.fillScreen(WHITE);<br>
    graphics.drawString("Arcs", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawArc(55, 60, 20, GRAPHICS_ARC_NWW, RED, 3);<br>
    graphics.drawString("NWW", 15, 45, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawArc(62, 53, 20, GRAPHICS_ARC_NNW, ORANGE, 3);<br>
    graphics.drawString("NNW", 30, 27, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawArc(70, 55, 20, GRAPHICS_ARC_NE, BLUE, 3);<br>
    graphics.drawString("NE", 90, 33, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawArc(64, 65, 20, GRAPHICS_ARC_S, GREY, 3);<br>
    graphics.drawString("S", 90, 70, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillArc(55, 130, 20, GRAPHICS_ARC_NWW, GREEN);<br>
    graphics.drawString("NWW", 15, 115, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillArc(62, 123, 20, GRAPHICS_ARC_NNW, LIGHT_GREY);<br>
    graphics.drawString("NNW", 30, 97, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillArc(70, 125, 20, GRAPHICS_ARC_NE, ORANGE);<br>
    graphics.drawString("NE", 90, 103, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.fillArc(64, 115, 30, GRAPHICS_ARC_SSW+GRAPHICS_ARC_SSE, BLUE);<br>
    graphics.drawString("SSW+SSE", 80, 147, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.drawString("Filled", 10, 150, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    delay(wait);<br>
    <br>
    // Circles<br>
    graphics.fillScreen(WHITE);<br>
    graphics.drawString("Circles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawCircle(64, 55, 25, BLUE, 3);<br>
    graphics.fillCircle(64, 115, 25, BROWN);<br>
    graphics.drawString("Filled", 50, 145, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
    delay(wait);<br>
    <br>
    // Big digits<br>
    graphics.fillScreen(WHITE);<br>
    graphics.drawString("Big digits", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.drawBigDigit(6, 10, 25, 40, 60, BLUE, 6, GRAPHICS_STYLE_NORMAL);<br>
    graphics.drawBigDigit(4, 70, 35, 45, 55, RED, 10, GRAPHICS_STYLE_ADVANCED);<br>
    graphics.drawBigDigit(8, 30, 100, 60, 50, BROWN, 12, GRAPHICS_STYLE_ADVANCED);<br>
    delay(wait);<br>
    <br>
    // Rotations<br>
    graphics.fillScreen(WHITE);<br>
    graphics.drawString("Rotations", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);<br>
    graphics.setRotation(GRAPHICS_ROTATION_90);<br>
    graphics.drawString("Rotation 90d", 15, 15, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.setRotation(GRAPHICS_ROTATION_180);<br>
    graphics.drawString("Rotation 180d", 5, 15, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.setRotation(GRAPHICS_ROTATION_270);<br>
    graphics.drawString("Rotation 270d", 15, 15, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);<br>
    graphics.setRotation(GRAPHICS_ROTATION_0);<br>
    delay(wait);<br>
<br>
    graphics.fillScreen(WHITE);<br>
    wait = 1500;<br>
}<br>
</code></pre>