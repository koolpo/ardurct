# What is ArduRCT? #

### Overview ###
[ArduRCT](http://ardurct.googlecode.com/svn/images/ArduRCT_concept.png) stands for **Ardu** ino **R** emote **C** ontrol and **T** elemetry.<br> It is a versatile solution for controlling and receiving telemetry from any remotely controlled vehicule.<br>
<br>
<img src='http://ardurct.googlecode.com/svn/images/ArduRCT_overview.png' />

ArduRCT provides 2 sub-systems:<br>
<ul><li>a local one which:<br>
<ul><li>can be standalone or connected to a computer,<br>
</li><li>sends command and displays telemetry.<br>
</li></ul></li><li>a remote one which:<br>
<ul><li>can be used in replacement of any Receiver on a RC vehicule,<br>
</li><li>processes commands and sends telemetry.</li></ul></li></ul>

<h3>Implementation</h3>
One of the possible implementations.<br>
<a href='http://ardurct.googlecode.com/svn/images/ArduRC.png'>
<img src='http://ardurct.googlecode.com/svn/images/ArduRC.png' width='490'><a>
<a href='http://ardurct.googlecode.com/svn/images/Remotuino.png'>
<img src='http://ardurct.googlecode.com/svn/images/Remotuino.png' width='250'><a>

<h3>Guiding principles</h3>
<ul><li>Describe both the hardware and the firmware (software) required to build the ArduRCT solution.<br>
</li><li>Promote re-usability of software components through extensive use of libraries.<br>
</li><li>Promote re-usability of hardware parts. Use easily available hardware.<br>
</li><li>Aim the main trunk at controling an <a href='http://code.google.com/p/arducopter/'>ArduCopter</a>.