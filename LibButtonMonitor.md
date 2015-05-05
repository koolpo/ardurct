# Arduino library to monitor buttons #
<img src='http://ardurct.googlecode.com/svn/images/Button.jpg' width='200'><img src='http://ardurct.googlecode.com/svn/images/Button.jpg' width='200'><img src='http://ardurct.googlecode.com/svn/images/Button.jpg' width='200'>

<a href='http://code.google.com/p/ardurct/downloads/list'><img src='http://www.gstatic.com/codesite/ph/images/dl_arrow.gif' /></a> <a href='http://code.google.com/p/ardurct/downloads/list'>Download</a>

<h3>Content</h3>
A library to get the state of a pushbutton <a href='http://www.sparkfun.com/products/97'>COM-00097</a>.<br>
Each monitored button can be ascertained to be in the following states :<br>
<ul><li>NOT_PRESSED,<br>
</li><li>DEPRESSED : the button has been pushed down, once retrived, the state will be PRESSED<br>
</li><li>PRESSED : the button is down,<br>
</li><li>REPEATING : the button has been down for some time,<br>
</li><li>FAST_REPEATING : the button has been down for a long time,<br>
</li><li>RELEASED : the button has been releasedn, once retrived, the state will be NOT_DEPRESSED.</li></ul>

<h3>Example usage</h3>
<pre><code>#include &lt;ButtonMonitor.h&gt;<br>
<br>
#define BUTTON1 0<br>
#define BUTTON1_PIN 2<br>
#define BUTTON2 1<br>
#define BUTTON2_PIN 3<br>
<br>
#define LED 13<br>
<br>
void setup() {<br>
    // Monitor the buttons<br>
    ButtonMonitor.add(BUTTON1, BUTTON1_PIN);<br>
    ButtonMonitor.add(BUTTON2, BUTTON2_PIN);<br>
    // set the led as an output<br>
    pinMode(LED, OUTPUT);<br>
}<br>
<br>
void loop() {<br>
    // monitor the buttons: needs to be called at least once per BUTTON_MONITOR_DEBOUNCE_DELAY_MS (50ms)<br>
    ButtonMonitor.update();<br>
    // wait between command reads<br>
    delay(5);<br>
    // adjust the led according to the command<br>
    if (ButtonMonitor.isDepressed(BUTTON1)) digitalWrite(LED, HIGH);<br>
    if (ButtonMonitor.isDepressed(BUTTON2)) digitalWrite(LED, LOW);<br>
}<br>
</code></pre>