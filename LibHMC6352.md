# Arduino library for HMC6352 #
<img src='http://ardurct.googlecode.com/svn/images/HMC6352.jpg' width='300'>

<a href='http://code.google.com/p/ardurct/downloads/list'><img src='http://www.gstatic.com/codesite/ph/images/dl_arrow.gif' /></a> <a href='http://code.google.com/p/ardurct/downloads/list'>Download</a>

<h3>Content</h3>
A simple library to get the heading value out of a <a href='http://www.sparkfun.com/products/7915'>HMC6352</a>.<br>
It relies on the Wire library, which is included with Arduino.<br>
<br>
<h3>Example usage</h3>
<pre><code>#include &lt;Wire.h&gt;<br>
#include &lt;HMC6352.h&gt;<br>
<br>
void setup() {<br>
    // Initialize the magnetometer<br>
    HMC6352.begin();<br>
    // Initialize the serial port<br>
    Serial.begin(9600);<br>
}<br>
<br>
void loop() {<br>
    Serial.print("Heading: ");<br>
    Serial.println(HMC6352.getHeading());<br>
    delay(500);<br>
}<br>
</code></pre>