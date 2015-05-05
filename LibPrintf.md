# Arduino library for printf #
[![](http://www.gstatic.com/codesite/ph/images/dl_arrow.gif)](http://code.google.com/p/ardurct/downloads/list) [Download](http://code.google.com/p/ardurct/downloads/list)

### Content ###
A simple library to facilitate the printing of messages containing variables.<br>
It works on any class inheriting from Print (TouchScreen, LCD_PCD8544, LCD, Serial...).<br>
It is JUST a repackaging of <a href='http://www.utopiamechanicus.com/article/sprintf-arduino/'><a href='http://www.utopiamechanicus.com/article/sprintf-arduino/'>http://www.utopiamechanicus.com/article/sprintf-arduino/</a></a> by David Pankhurst. Many thanks to him !<br>
<br>
<h3>Example usage</h3>
<pre><code>#include &lt;Printf.h&gt;<br>
<br>
void setup() {<br>
    Serial.begin(57600);<br>
    pinMode(4, INPUT);<br>
}<br>
<br>
void loop() {<br>
    // beware the underscore instead of the dot is required<br>
    Serial_printf("Time=%d, pin_4=%d", millis(), digitalRead(4));<br>
    delay(250);<br>
}<br>
</code></pre>