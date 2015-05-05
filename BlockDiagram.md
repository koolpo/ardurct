# How does it work ? #

### Overview ###
![http://ardurct.googlecode.com/svn/images/ArduRCT_detail1.png](http://ardurct.googlecode.com/svn/images/ArduRCT_detail1.png)

The local and remote parts are based upon :
  * [Arduino](http://arduino.cc/en/Main/Hardware) compatible boards to allow for easy reprogramming for different types of RC vehicules.
  * [XBee modems](http://www.sparkfun.com/categories/111) to allow for the choice of transmitting power, and therefore of range.

### Details ###
![http://ardurct.googlecode.com/svn/images/ArduRCT_detail2.png](http://ardurct.googlecode.com/svn/images/ArduRCT_detail2.png)

The grey blocks form the minimum working system, the dashed ones are optional. <br>The red text describes which Arduino library is used to manage the block from the Arduino MCU.<br>
<br>
The <b>local part</b> contains:<br>
<ul><li>a <a href='RCShield.md'>RCShield</a> which is an Arduino shield, containing:<br>
<ul><li>various sensors to control the remote part<br>
</li><li>an XBee to send control commands and receive telemetry.<br>
</li></ul></li><li>On this shield is plugged a <a href='TouScruino.md'>TouScruino</a> which is an Arduino compatible board.<br>
<ul><li>This main board can be replaced by any Arduino board conforming to the Arduino Uno footprint.</li></ul></li></ul>

The <b>remote part</b> leverages a <a href='Remotuino.md'>Remotuino</a> to control servos and send back telemetry.<br>
<br>
The two pink blocks go together: the GPS knows the position of the remote vehicule and the magnetometer can therefore give the position and the heading of the remote vehicule relative to the local station.<br>
This allows remote control without line of sight (fog, rain, darkness, long distance...).<br>
We assume that the local station is not moving too much, which is the case in most scenarios.