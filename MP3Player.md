### Principle ###
Combine the <a href='http://www.sparkfun.com/products/10628'>MP3 Shield</a> from Sparkfun with TouScruino and a battery: we have a portable MP3 Player.
<img src='http://dlnmh9ip6v2uc.cloudfront.net/images/products/10628-01b.jpg' width='300'><img src='http://ardurct.googlecode.com/svn/images/TouScruinoT.jpg' width='300'>

<h3>Why ?</h3>
<ul><li>Only for education reasons since we can find a very good MP3 Player in the commerce.<br>
</li><li>Ah, hang on, we can personalize this one to adapt to the ambient noise or the ambient light and have a PROPER battery with LOOONG autonomy.</li></ul>

<h3>Hardware</h3>
<ul><li>Since the shield only has, for whatever reason, the 5V wired as power, we have to link the 3.3V pin with the output of the 3.3V regulator: quite easy to do.<br>
</li><li>We also have to remove the reset button since it is under the new IOREF pin...<br>
</li><li>Maybe cut the hole tabs to reduce the footprint.<br>
</li><li>The battery fits in between the 2 boards, making the player quite compact... OK, OK: it will never be a whatever nano.</li></ul>

<h3>Software</h3>
<ul><li>We will just use an AREA of the UI manager to show a list of files from the SD under the List tab.<br>
</li><li>When a user clicks on a line in the list, we change tab to the Play tab.<br>
</li><li>One more tab for the options? Volume, backlight...