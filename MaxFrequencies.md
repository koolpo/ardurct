### Calculation formulas ###
According to the Atmega datasheets, in the "speed grades" section, the maximum frequency in linear from 1.8 to 2.7v and 2.5 to 4.5v.<br>
There is a nice graph but it is difficult to define exact values from the curve...<br>
Here are the formulas to get the frequency in Megahertz<br>
<ul><li><b>1.8 < VCC <= 2.7</b>
<ul><li>FMax = 10 - (2.7 - VCC) x (10 - 4) / (2.7 - 1.8)<br>
</li></ul></li><li><b>2.7 < VCC <= 4.5</b>
<ul><li>FMax = 20 - (4.5 - VCC) x (20 - 10) / (4.5 - 2.7)<br>
</li></ul></li><li><b>VCC > 4.5</b>
<ul><li>FMax = 20</li></ul></li></ul>

<h3>Recommended values</h3>
The <b>FMax</b> column contains the maximum frequency the Atmega can safely support.<br>
The <b>Performance</b> contains the nearest standard oscillator frequency, then the <b><i>Error</i></b> in the baudrate for a target of 57 600 b/s.<br>
The <b>Serial</b> column contains the nearest standard oscillator frequency giving 0% error at 57 600 b/s. <br>
The <b><i>Max. baudrate</i></b> column gives the maximum standard baudrate reachable with 0% error at Serial frequency.<br><br>
<u>Note:</u> the receive recovery mechanism on the UART will safely allow transmission+reception errors up to +3.9/-4.0% for 8 bits, no parity.<br>
<table><thead><th>VCC</th><th>FMax</th><th>Performance</th><th><i>Error</i></th><th>Serial</th><th><i>Max. baudrate</i></th></thead><tbody>
<tr><td>1.8</td><td>4.0000</td><td>4.0000</td><td>8.0%</td><td>3.6864</td><td>230 400</td></tr>
<tr><td>2.7</td><td>10.0000</td><td>10.0000</td><td>2.1%</td><td>7.3728</td><td>230 400</td></tr>
<tr><td>3.3</td><td>13.3333</td><td>12.2880</td><td>-2.1%</td><td>11.0592</td><td>230 400</td></tr>
<tr><td>5.0</td><td>20.0000</td><td>20.0000</td><td>0.9%</td><td>18.4320</td><td>230 400</td></tr></tbody></table>

<h3>Wiring standard</h3>
The <b>Freq</b> column contains the recommended value for VCC.<br>
The <b><i>Error</i></b> column shows the baudrate error for a target of 57 600 b/s.<br>
The <b><i>Max. baudrate</i></b> column gives the maximum standard baudrate reachable with less than 1.5% error.<br><br>
<u>Note:</u> following on previous note, one 8Mhz system can safely exchange data at 57 600 b/s with a 16MHz system (worst case scenario gives 2.9% error); but two 8Mhz system can't (worst case scenario gives 4.2% error).<br>
<table><thead><th>VCC</th><th>Freq</th><th><i>Error</i></th><th><i>Max. baudrate</i></th></thead><tbody>
<tr><td>3.3</td><td>8.0000</td><td>2.1%</td><td>38 400</td></tr>
<tr><td>5.0</td><td>16.0000</td><td>-0.8%</td><td>76 800</td></tr>