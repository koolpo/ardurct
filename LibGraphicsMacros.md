<img src='http://ardurct.googlecode.com/svn/images/MacroTests2.png' align='right'><br>

<h3>Description</h3>
The library contains a macro language to draw or affect graphics such as:<br>
<ul><li>Texts: 4 different size, bold and overlay<br>
</li><li>Lines: variable thickness.<br>
</li><li>Triangles: filled or not, with variable thickness.<br>
</li><li>Rectangles: filled or not, with variable thickness.<br>
</li><li>Rounded rectangles: filled or not, with variable thickness.<br>
</li><li>Arcs: filled or not, with variable thickness.<br>
</li><li>Circles: filled or not, with variable thickness.<br>
</li><li>Presets: erase the screen, set the thickness, the font, the scale<br>
</li><li>Eeprom: save and retrieve macros in and from the Eeprom<br>
<h3>Principles</h3>
</li><li>A macro is a serie of drawing commands which can be chained together.<br>
</li><li>Each command is a few letters followed by a space and some parameters separated by spaces<br>
</li><li>Case: There is no case sentitiveness.<br>
</li><li>Chaining: If the number of parameters is not sufficient to finish the command, the last point reached by the previous macro command is used.<br>
</li><li>Numbers: Colors are in hexadecimal, all other numbers are in decimal<br>
<h3>Examples</h3>
<pre><code>// draw a filled circle of radius 10 at 20,20<br>
cf 20 20 10<br>
<br>
// draw a line starting at 10,10 and finishing at 10,20<br>
l 10 10 10 20<br>
<br>
// draw a line starting at 10,10 and finishing at 20,10<br>
ld 10 10 10 0<br>
<br>
// draw a NEE arc of radius 10 with the bottom starting at the last point drawn.<br>
arnee 10<br>
</code></pre>
<h3>Code</h3>
To produce the MacroTests images above, use the following code.<br>
<pre><code>#include &lt;TouchScreen.h&gt;<br>
<br>
TouchScreen touchscreen;<br>
<br>
// size 23x37<br>
uint8_t seven[] = "ld 0 0 22 0 ld 0 9 arnww 38";<br>
uint8_t eight[] = "ld 10 0 2 0 ae 8 ld -2 0 ld 2 0 ae 10 ld -2 0 aw 10 aw 8";<br>
<br>
uint8_t buffer[256];<br>
uint8_t bufferPtr = 0;<br>
<br>
public void setup() {<br>
    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);<br>
    touchscreen.setBacklight(180);<br>
    <br>
    // draw in blue, with a scalable line thickness of 2<br>
    touchscreen.executeMacro("pts 2 pc 1F");<br>
    // draw a seven<br>
    touchscreen.executeMacro(seven, 10, 10, 3, 1, true);<br>
    // write eight into eeprom for future use, in slot 0<br>
    touchscreen.executeMacro("w 0 " + eight);<br>
    // write hello into eeprom for future use, in slot 1<br>
    touchscreen.executeMacro("w 1 s 0 0 \"Hello\"");<br>
    // set foreground to red<br>
    touchscreen.executeMacro("pc F800 pts 2");<br>
    // draw the macro stored in eeprom in slot 0<br>
    touchscreen.executeMacro("e 0", 120, 10, 3, 1, true);<br>
    // draw the macro stored in eeprom in slot 1<br>
    // draw in size 1 black<br>
    touchscreen.executeMacro("pf 1 pc 0 e 1", 10, 160, 1, 1, true);<br>
    // draw in size 2 bold gray<br>
    touchscreen.executeMacro("pfb 2 pc 9CD3 e 1", 10, 180, 1, 1, true);<br>
<br>
    delay(5000);<br>
}<br>
<br>
public void loop() {<br>
    while (Serial.available()) buffer[bufferPtr++] = Serial.read();<br>
    if (bufferPtr == 0) return;<br>
    <br>
    // wait for end of sentence<br>
    if (buffer[bufferPtr-1] == '.') {<br>
        // mark end of macro<br>
        buffer[bufferPtr-1] = 0;<br>
        bufferPtr = 0;<br>
        // execute the macro<br>
        touchscreen.executeMacro(buffer, 10, 10);<br>
    }<br>
    delay(10);<br>
}<br>
<br>
</code></pre></li></ul>

<h3>Language</h3>
Items between brackets can be ommited.<br>
<table>
<tr><td><b>Presets</b><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='50' height='10'><td>can preset the thickness, color, background, scale, fontSize, fontBold, fontOverlay<br>
<tr><td>pe		<td>	<td>erase the screen: fill the screen with background<br>
<tr><td>pt thickness<td>	<td>preset the thickness<br>
<tr><td>ps scale (div)<td>	<td>preset the scale, all further drawing will be scaled by scale/div<br>
<tr><td>pc color<td>	<td>preset the foreground color<br>
<tr><td>pb color<td>	<td>preset the background color<br>
<tr><td>pf(b)(o) size<td>	<td>preset the font<br>
<tr><td>		<td>b<td>if present the font will be bold<br>
<tr><td>		<td>o<td>if present the font will be overlaid<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>

<tr><td><b>Lines</b><td><td>
<tr><td>l(d) x1 y1 (x2 y2)<td><td>
<tr><td>		<td>d<td>delta: if present, x1 and y1 (or x2 and y2 if they exist) are delta values relative to last point drawn<br>
<tr><td>		<td>x1 y1<td>start or next point in the line<br>
<tr><td>		<td>x2 y2<td>next point in the line<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>

<tr><td><b>Arcs</b><td><td>can be drawn clockwise or counter clockwise and filled<br>
<tr><td>a(r)(f)DDD (x0 y0) radius<td><td>
<tr><td>		<td>r<td>if present, the arc will be drawn reversed (counter clockwise).<br>i.e.: if DDD=NE<br>- if r the East end will be connected to the last point drawn<br>- else the North end will be attached<br>
<tr><td>		<td>f<td>if present, the arc will be filled<br>
<tr><td>		<td>DDD<td>can be n, s, e, w, ne, nw, se, sw, nne, nee, see, sse, ssw, sww, nww, nnw<br>
<tr><td>		<td>radius<td>radius of the arc<br>
<tr><td>		<td>x0 y0<td>center of the arc. If ommited the top (or bottom) of the arc will be connected to the last point drawn<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>

<tr><td><b>Rectangles</b><td><td>can be rounded and filled<br>
<tr><td>r(r)(f) x1 y1 width height (radius)<td><td>
<tr><td>		<td>r<td>if present, the rectangle will be rounded and radius is required<br>
<tr><td>		<td>f<td>if present, the rectangle will be filled<br>
<tr><td>		<td>x1 y1<td>top left of the rectangle<br>
<tr><td>		<td>width<td>
<tr><td>		<td>height<td>width and height of the rectangle<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>

<tr><td><b>Triangles</b><td><td>can be filled<br>
<tr><td>t(f) x1 y1 x2 y2 x3 y3<td><td>
<tr><td>		<td>f<td>if present, the triangle will be filled<br>
<tr><td>		<td>x1 y1<td>first point<br>
<tr><td>		<td>x2 y2<td>second point<br>
<tr><td>		<td>x3 y3<td>third point<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>

<tr><td><b>Circles</b><td><td>can be filled<br>
<tr><td>c(f) x0 y0 radius<td><td>
<tr><td>		<td>f<td>if present, the circle will be filled<br>
<tr><td>		<td>x0 y0<td>center of the circle<br>
<tr><td>		<td>radius<td>radius of the circle<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>

<tr><td><b>Strings</b><td><td>
<tr><td>s x1 y1 "text"<td><td>
<tr><td>		<td>x1 y1<td>top left of the first char drawn<br>
<tr><td>		<td>"text"<td>text to draw. Some characters can be escaped \n, \", \\<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>

<tr><td><b>Writes</b><td><td>Write a macro in EEPROM, all following commands are written to the eeprom<br>
<tr><td>w n<td><td>
<tr><td>		<td>n<td>the macro number to write<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>

<tr><td><b>Executes</b><td><td>Execute a macro from EEPROM<br>
<tr><td>e(r) n<td><td>
<tr><td>		<td>r<td>if present, resets the macro variables before executing<br>
<tr><td>		<td>n<td>the macro number to execute<br>
<tr><td><img src='http://ardurct.googlecode.com/svn/images/blank.jpg' width='10' height='15'><td><td>
