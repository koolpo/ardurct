## Introduction ##

TouScruino is a contraction for Tou(ch) Scr(een) (Ard)uino.<br>
It is a versatile Arduino compatible board: it copies the connector positions of the Arduino Uno (Rev. 3) and offers<br>
almost the same functions at the same pin positions (D3 is not a PWM anymore, it became TX1).<br>
See <a href='PinMapping.md'>pins mapping</a> for details for TouScruino-avr.<br>
<br>
<h2>Features</h2>

<ul><li>Can be used as a standalone Arduino compatible board, capable of accepting shields (they are "under shields" though :)).<br>
</li><li>Has a single cell LiPo charger and connector to allow battery operation, and a ULDO regulator to generate 3.3V from the LiPo battery. The charge is done through a micro USB connector.<br>
</li><li>Contains a screen and some input to interact with it: either a joystick, some buttons, or a touchpanel.</li></ul>

By default, TouScruino contains a firmware which will draw the commands passed through the Serial pins (RX=D0 and TX=D1) on the screen,<br>
and returns upon request the state of the inputs.<br>
<br><br>
<h2>Building principle</h2>
<img src='http://ardurct.googlecode.com/svn/images/TouScruino-versions.png'><br><br>
The Main board on top is combined with a User Interface to give the different Versions of TouScruino.<br>
<h3>Main board</h3>
At this stage only the avr version is fully functional. The arm version will come at a later date.<br>
<br>
<u><b>avr</b></u><br>
Content: ATMEGA1284 as MPU, AR1021 as touch controller.<br>
<br>
<u><b>arm</b></u><br>
Content: MK20DX256 as MPU and touch controller.<br>
<br>
<h3>User Interfaces</h3>
<u><b>ui0</b></u><br>
Content: PCD8544 as screen, 4 buttons for interaction.<br>
Although usefull for simple interaction, the screen is too limited to be used for ArduRCT.<br>
<br><br>
<u><b>ui1</b></u><br>
Content: JD-T1800 as screen, 4 buttons or a 5 ways switch for interaction.<br>
<br><br>
<u><b>ui2</b></u><br>
Content: IL934X or equivalent as touchscreen.<br><br>
<h3>Versions</h3>
<u><b><a href='TouScruinoAvrUi0.md'>avr-ui0</a></b></u>
Status: Terminated.<br>
<br><br>
<u><b><a href='TouScruinoAvrUi1.md'>avr-ui1</a></b></u>
Status: Complete.<br>
<br><br>
<u><b><a href='TouScruinoAvrUi2.md'>avr-ui2</a></b></u>
Status: Complete.<br>
<br><br>
<u><b><a href='TouScruinoArmUi2.md'>arm-ui2</a></b></u>
Status: Complete.<br>
<br><br>