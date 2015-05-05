# Arduino library for graphic screens #
### Content ###
The library is designed to manage small screens such as TouScruino or the <a href='http://www.adafruit.com/products/335'>one from adafruit</a>.<br>
For the moment, it manages screens based on the S6D04H0, and the ST7735. It will soon manage ILI932x based screens.<br>
Porting the library for other screens or other MCUs is quite simple as the hardware has been completely abstracted: see <a href='LibGraphicsCodeDetails.md'>code details</a>
<h3>Pin mapping</h3>
<u>ST7735</u><br>
<table><thead><th>Arduino</th><th>GND</th><th>5V </th><th>8  </th><th>9  </th><th>       </th><th>10    </th><th>MOSI</th><th>SCK</th><th>    </th><th>3.3V</th></thead><tbody>
<tr><td>LCD    </td><td>GND</td><td>VCC</td><td>RST</td><td>D/C</td><td>CARD_CS</td><td>TFT_CS</td><td>MOSI</td><td>SCK</td><td>MISO</td><td>LITE</td></tr></tbody></table>

The library uses hardware SPI to achieve a good refresh speed. At 16MHz, the screen can be painted any color in less than 50ms.<br>
<br>
<br><u>S6D04H0</u><br>
See schematics for <a href='TouScruino.md'>TouScruino</a>

<h3>Functionalities</h3>
The screen can be rotated by 90 degree increments.<br>
<table><tr><td>
<img src='http://ardurct.googlecode.com/svn/images/Graphics.png'>
<td valign='top'>
<u><b>Drawing functions:</b></u>
<ul><li>Texts: 4 different size, bold and overlay<br>
</li><li>Lines: variable thickness.<br>
</li><li>Triangles: filled or not, with variable thickness.<br>
</li><li>Rectangles: filled or not, with variable thickness.<br>
</li><li>Rounded rectangles: filled or not, with variable thickness.<br>
</li><li>Arcs: filled or not, with variable thickness.<br>
</li><li>Circles: filled or not, with variable thickness.<br>
</li><li>Bitmaps<br>
</li><li>Patterns: scaled or not<br>
</li><li>Big digits: any size in width and height<br>
<br>See <a href='LibGraphicsGraphics.md'>Graphics</a> for details.<br>
<tr><td>
<img src='http://ardurct.googlecode.com/svn/images/MacroTests.png'>
<td valign='top'>
<u><b>Macros functions:</b></u><br>
Possibility to draw any of the above drawing through simple commands. <br>See <a href='LibGraphicsMacros.md'>Macros</a> for details.<br>
<br><br>