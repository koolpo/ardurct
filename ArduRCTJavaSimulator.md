<img src='http://ardurct.googlecode.com/svn/images/Graphics.png'><img src='http://ardurct.googlecode.com/svn/images/UITests.png' align='right'>

<a href='http://code.google.com/p/ardurct/downloads/list'><img src='http://www.gstatic.com/codesite/ph/images/dl_arrow.gif' /></a> <a href='http://code.google.com/p/ardurct/downloads/list'>Download</a>

<h3>Overview</h3>
The idea is to write code in java as if it was code for Arduino and then copy and paste the code in Arduino and obtain the same result on the hardware.<br>
That is to say: fill in a setup and a loop function.<br>
Because the objective is ultimately to write Arduino code, the Java code is not written as good Java code should be written!<br>
<br>
<h3>Content</h3>
This archive contains the Eclipse source for various simulators of ArduRCT components:<br>
<ul><li>ArduRCTSimulator running the complete application: local and remote<br>
</li><li>ArduRCSimulator running ArduRCT_Local code<br>
</li><li>RemotuinoSimulator running ArduRCT_Remote code<br>
</li><li>TouchscreenSimulator running the TouchScreen library examples code</li></ul>

<h3>Functionalities</h3>
As of now, the Simulator can simulate:<br>
<ul><li>HardwareSerial<br>
</li><li>XBeeRadio<br>
</li><li>GPS<br>
</li><li>SD<br>
</li><li>EEPROM<br>
</li><li>TouchScreen<br>
<ul><li>Screen<br>
</li><li>Touch panel<br>
</li></ul></li><li>Servo</li></ul>

<h3>Usage</h3>
<ol><li>Duplicate one of the files under the examples folder, such as Graphics,<br>
</li><li>Modify it to suit your needs,<br>
</li><li>Run the 'TouchscreenSimulator' application, and choose from the examples menu your Application.<br>
</li><li>Copy the content of the file into your code and remove the 'public' word.<br>
</li><li>Adjust the indentation, compile and run on the hardware.</li></ol>

<h3>Examples</h3>
<u><b>Graphics</b></u><br>
To produce the Graphics image above, use the following code.<br>
<pre><code>package com.google.code.ardurct.touchscreen.examples;<br>
<br>
<br>
public class Graphics extends Example {<br>
<br>
    public void setup() {<br>
        touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);<br>
        touchscreen.setBacklight(180);<br>
<br>
        // Fonts<br>
        touchscreen.drawString("SMALL", 5, 5, BLACK, FONT_SMALL, false, false);<br>
        touchscreen.drawString("BOLD", 100, 5, BLACK, FONT_SMALL, FONT_BOLD, false);<br>
        touchscreen.drawString("MEDIUM", 5, 20, BLACK, FONT_MEDIUM, false, false);<br>
        touchscreen.drawString("BOLD", 100, 20, BLACK, FONT_MEDIUM, FONT_BOLD, false);<br>
        touchscreen.drawString("BIG", 5, 40, BLACK, FONT_BIG, false, false);<br>
        touchscreen.drawString("BOLD", 100, 40, BLACK, FONT_BIG, FONT_BOLD, false);<br>
        touchscreen.drawString("HUGE", 5, 65, BLACK, FONT_HUGE, false, false);<br>
        touchscreen.drawString("BOLD", 100, 65, BLACK, FONT_HUGE, FONT_BOLD, false);<br>
        // Lines<br>
        touchscreen.drawLine(10, 95, 60, 95, RED, 2);<br>
        touchscreen.drawLine(80, 95, 80, 105, RED, 3);<br>
        touchscreen.drawLine(100, 95, 130, 105, RED, 1);<br>
        // Triangles<br>
        touchscreen.drawTriangle(20, 140, 40, 120, 50, 135, BLUE, 2);<br>
        touchscreen.fillTriangle(120, 130, 140, 120, 150, 135, BLUE);<br>
        // Rectangle<br>
        touchscreen.drawRectangle(30, 150, 40, 20, MAGENTA, 3);<br>
        touchscreen.fillRectangle(130, 150, 40, 20, MAGENTA);<br>
        // Rounded rectangle<br>
        touchscreen.drawRoundedRectangle(30, 180, 40, 20, 6, GREEN, 2);<br>
        touchscreen.fillRoundedRectangle(130, 180, 40, 20, 6, GREEN);<br>
        // Arcs<br>
        touchscreen.drawArc(40, 210, 30, SCREEN_ARC_SE, RED, 4);<br>
        touchscreen.drawArc(80, 240, 30, SCREEN_ARC_NEE, RED, 3);<br>
        touchscreen.fillArc(140, 210, 20, SCREEN_ARC_SW, RED);<br>
        // Circles<br>
        touchscreen.drawCircle(50, 270, 20, BLUE, 1);<br>
        touchscreen.drawCircle(100, 270, 20, BLUE, 3);<br>
        touchscreen.fillCircle(150, 270, 20, BLUE);<br>
    }<br>
<br>
    public void loop() {<br>
        delay(100);<br>
    }<br>
}<br>
</code></pre>
<br>
<u><b>UITests</b></u><br>
To produce the UITests image above, use the following code.<br><br>
<u>Note:</u><br>
Some minor changes will have to be done to this code after copying it to the Arduino IDE, as Java and Arduino handle pointers differently.<br>
See <a href='LibTouchScreenUserInterface.md'>User interface</a>.<br>
<pre><code>public class UITests extends Example {<br>
<br>
    public int listTab;<br>
    public int testsTab;<br>
    public int colorsTab;<br>
    <br>
    public static int TEST_ID = 100;<br>
    public static int PUSH_1_ID = 101;<br>
    public static int PUSH_2_ID = 102;<br>
    public static int PUSH_3_ID = 103;<br>
    public static int T1_ID = 110;<br>
    public static int T2_ID = 111;<br>
    public static int LABEL_B_ID = 118;<br>
    public static int LABEL_O_ID = 119;<br>
    public static int LABEL_1_ID = 120;<br>
    public static int LABEL_2_ID = 125;<br>
    public static int LABEL_3_ID = 127;<br>
    public static int GAUGE_1_ID = 130;<br>
    public static int SLIDER_1_ID = 140;<br>
    public static int GAUGE_2_ID = 150;<br>
    public static int SLIDER_2_ID = 160;<br>
    <br>
    public static int SLIDER_R_ID = 200;<br>
    public static int SLIDER_G_ID = 201;<br>
    public static int SLIDER_B_ID = 202;<br>
    public static int LABEL_R_ID = 205;<br>
    public static int AREA_COLOR_ID = 210;<br>
    <br>
    public static int GROUP_1 = 3;<br>
<br>
    public void setup() {<br>
        touchscreen.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);<br>
        touchscreen.setBacklight(180);<br>
        <br>
        touchscreen.setupUI(0, 0, touchscreen.getWidth(), touchscreen.getHeight());<br>
        listTab = touchscreen.addUITab("List");<br>
        testsTab = touchscreen.addUITab("Tests", doTestsAction);<br>
        colorsTab = touchscreen.addUITab("Colors", setColor, drawColor);<br>
        <br>
        touchscreen.addUILabel(testsTab, LABEL_B_ID, 5, 5, UI_AUTO_SIZE, UI_AUTO_SIZE, "Button");<br>
        touchscreen.addUIButton(testsTab, TEST_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_B_ID, 5, UI_AUTO_SIZE, UI_AUTO_SIZE, <br>
                "Test");<br>
        touchscreen.addUILabel(testsTab, LABEL_O_ID, 5, 35, UI_AUTO_SIZE, UI_AUTO_SIZE, "Option");<br>
        touchscreen.addUIPushButton(testsTab, PUSH_1_ID, GROUP_1, UI_RIGHT_OF_WITH_MARGIN + LABEL_O_ID, 35, <br>
                UI_AUTO_SIZE, UI_AUTO_SIZE, "30", UI_SELECTED);<br>
        touchscreen.addUIPushButton(testsTab, PUSH_2_ID, GROUP_1, UI_RIGHT_OF + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, <br>
                UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, "60", UI_UNSELECTED);<br>
        touchscreen.addUIPushButton(testsTab, PUSH_3_ID, GROUP_1, UI_RIGHT_OF + PUSH_2_ID, UI_SAME_AS + PUSH_1_ID, <br>
                UI_SAME_AS + PUSH_1_ID, UI_SAME_AS + PUSH_1_ID, "90", UI_UNSELECTED);<br>
        touchscreen.addUILabel(testsTab, LABEL_1_ID, 5, 65, UI_AUTO_SIZE, UI_AUTO_SIZE, "Push button");<br>
        touchscreen.addUIPushButton(testsTab, T1_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + LABEL_1_ID, <br>
                UI_SAME_AS + LABEL_1_ID, UI_AUTO_SIZE, UI_AUTO_SIZE, "T1", UI_SELECTED);<br>
        touchscreen.addUIPushButton(testsTab, T2_ID, UI_NO_GROUP, UI_RIGHT_OF_WITH_MARGIN + T1_ID, UI_SAME_AS + T1_ID, <br>
                UI_SAME_AS + T1_ID, UI_SAME_AS + T1_ID, "T2", UI_UNSELECTED);<br>
        touchscreen.addUILabel(testsTab, LABEL_2_ID, 5, 95, UI_AUTO_SIZE, UI_AUTO_SIZE, "Gauge");<br>
        touchscreen.addUIGauge(testsTab, GAUGE_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_2_ID, UI_SAME_AS + LABEL_2_ID, <br>
                150, UI_SAME_AS + LABEL_2_ID, 40, 0, 100);<br>
        touchscreen.addUILabel(testsTab, LABEL_3_ID, 5, 125, UI_AUTO_SIZE, UI_AUTO_SIZE, "Slider");<br>
        touchscreen.addUISlider(testsTab, SLIDER_1_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_3_ID, UI_SAME_AS + LABEL_3_ID, <br>
                150, UI_SAME_AS + LABEL_3_ID, 40, 0, 100);<br>
        touchscreen.addUISlider(testsTab, SLIDER_2_ID, 15, 150, 20, 120, 2, 0, 10);<br>
        touchscreen.addUIGauge(testsTab, GAUGE_2_ID, 60, 150, 20, 120, 2, 0, 10);<br>
        touchscreen.setUITab(testsTab);<br>
        <br>
        touchscreen.addUILabel(colorsTab, LABEL_R_ID, 5, 15, UI_AUTO_SIZE, UI_AUTO_SIZE, "R");<br>
        touchscreen.addUISlider(colorsTab, SLIDER_R_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 15, 180, <br>
                UI_SAME_AS + LABEL_R_ID, 0x15, 0, 0x1F);<br>
        touchscreen.addUILabel(colorsTab, 0, 5, 45, UI_AUTO_SIZE, UI_AUTO_SIZE, "G");<br>
        touchscreen.addUISlider(colorsTab, SLIDER_G_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 45, 180, <br>
                UI_SAME_AS + LABEL_R_ID, 0x1E, 0, 0x3F);<br>
        touchscreen.addUILabel(colorsTab, 0, 5, 75, UI_AUTO_SIZE, UI_AUTO_SIZE, "B");<br>
        touchscreen.addUISlider(colorsTab, SLIDER_B_ID, UI_RIGHT_OF_WITH_MARGIN + LABEL_R_ID, 75, 180, <br>
                UI_SAME_AS + LABEL_R_ID, 0xD, 0, 0x1F);<br>
        touchscreen.addUIArea(colorsTab, AREA_COLOR_ID, 50, 130, 140, 140);<br>
        touchscreen.setUIElementEditable(AREA_COLOR_ID, false);<br>
        touchscreen.setUIElementValue(AREA_COLOR_ID, 0xABCD);<br>
        <br>
    }<br>
<br>
    public void loop() {<br>
        touchscreen.handleUI();<br>
        delay(20);<br>
    }<br>
<br>
    private ActionCB doTestsAction = new ActionCB();<br>
    class ActionCB implements IUIActionCallback {<br>
        public void uiActionCallback(int id) {<br>
            if (id == SLIDER_1_ID) touchscreen.setUIElementValue(GAUGE_1_ID, touchscreen.getUIElementValue(SLIDER_1_ID));<br>
            if (id == SLIDER_2_ID) touchscreen.setUIElementValue(GAUGE_2_ID, touchscreen.getUIElementValue(SLIDER_2_ID));<br>
        }<br>
    }<br>
    <br>
    private SetColorCB setColor = new SetColorCB();<br>
    class SetColorCB implements IUIActionCallback {<br>
        public void uiActionCallback(int id) {<br>
            touchscreen.setUIElementValue(AREA_COLOR_ID, <br>
                    (touchscreen.getUIElementValue(SLIDER_R_ID) &lt;&lt; 11) + <br>
                    (touchscreen.getUIElementValue(SLIDER_G_ID) &lt;&lt; 5) + <br>
                    touchscreen.getUIElementValue(SLIDER_B_ID));<br>
        }<br>
    }<br>
    <br>
    private DrawColorCB drawColor = new DrawColorCB();<br>
    class DrawColorCB implements IUIDrawCallback {<br>
        public void uiDrawCallback(int id, int x, int y, int width, int height, int value) {<br>
            if (id == AREA_COLOR_ID) {<br>
                touchscreen.fillRectangle(x, y, width, 40, WHITE);<br>
                touchscreen.setCursor(x+5, y);<br>
                touchscreen.print("RGB = 0x");<br>
                touchscreen.print(touchscreen.getUIElementValue(SLIDER_R_ID), HEX);<br>
                touchscreen.print(" 0x");<br>
                touchscreen.print(touchscreen.getUIElementValue(SLIDER_G_ID), HEX);<br>
                touchscreen.print(" 0x");<br>
                touchscreen.print(touchscreen.getUIElementValue(SLIDER_B_ID), HEX);<br>
                touchscreen.setCursor(x+5, y+touchscreen.getFontHeight(FONT_SMALL)+4);<br>
                touchscreen.print("RGB565 = 0x");<br>
                touchscreen.print(value, HEX);<br>
                touchscreen.fillRectangle(x, y+30, width, height-30, value);<br>
                touchscreen.drawRectangle(x, y+30, width, height-30, BLACK, 1);<br>
            }<br>
        }<br>
    }<br>
}<br>
</code></pre>