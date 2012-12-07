package com.google.code.ardurct.hardware;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.util.HashMap;

import javax.swing.JPanel;


public class ServoBank extends JPanel {

	private static final long serialVersionUID = 1L;

	private static HashMap<String, ServoBank> ServoBankInstances = new HashMap<String, ServoBank>();

	public static ServoBank getInstance() {
		return ServoBankInstances.get(ServoBankInstances.keySet().iterator().next());
	}

	public static ServoBank getInstance(String name) {
		return ServoBankInstances.get(name);
	}
	
	public final static int SERVO_WIDTH = 120;
	public final static int SERVO_HEIGHT = 20;
	public final static int SERVO_INTERVAL = 5;

	private int[] servo;
	private int[] pin;
	private int nbActiveServos;
	
	BufferedImage img = null;
	
	
	public ServoBank(int nbServos, String name) {
		nbActiveServos = 0;
		servo = new int[nbServos];
		pin = new int[nbServos];
		for (int i=0; i<nbServos; i++) {
			servo[i] = 90;
			pin[i] = -1;
		}
		img = new BufferedImage(SERVO_WIDTH+80, 30+(SERVO_HEIGHT+SERVO_INTERVAL)*nbServos, BufferedImage.TYPE_INT_ARGB);
		drawBankImage();
		ServoBankInstances.put(name, this);
	}
	
	public void drawBankImage() {
		Graphics g = img.getGraphics();
		g.setFont(new Font("Courier", Font.BOLD, 13));
		g.setColor(new Color(240, 240, 240));
		g.fillRect(0, 0, img.getWidth(), img.getHeight());
		for (int i=0; i<servo.length; i++) drawServo(g, i);
	}
	
	public void drawServo(Graphics g, int index) {
		int y = 20;
		g.setColor(new Color(240, 240, 240));
		g.fillRect(0, y + SERVO_INTERVAL/2 + index * (SERVO_HEIGHT+SERVO_INTERVAL), img.getWidth(), SERVO_HEIGHT);
		g.setFont(new Font("Courier", Font.BOLD, 13));
		g.setColor(Color.BLACK);
		g.drawString("S" + (index < 9 && servo.length > 9 ? "0" : "") + (index + 1) + (pin[index] != -1 ? "[" + pin[index] + "]" : ""), 
				5,  y + 16 + index * (SERVO_HEIGHT+SERVO_INTERVAL));
		int x = 65;
		g.setColor(pin[index] != -1 ? Color.WHITE : new Color(240, 240, 240));
		g.fillRect(x, y + SERVO_INTERVAL/2 + index * (SERVO_HEIGHT+SERVO_INTERVAL), SERVO_WIDTH+1, SERVO_HEIGHT);
		g.setColor(Color.BLACK);
		g.drawRect(x, y + SERVO_INTERVAL/2 + index * (SERVO_HEIGHT+SERVO_INTERVAL), SERVO_WIDTH+1, SERVO_HEIGHT);
		g.setColor(Color.BLUE);
		int servoX = servo[index]*SERVO_WIDTH/180-2;
		if (servoX < 0) servoX = 0;
		if (servoX + 5 > SERVO_WIDTH) servoX = SERVO_WIDTH-5;
		g.fillRect(x+1+servoX, y + SERVO_INTERVAL/2 + index * (SERVO_HEIGHT+SERVO_INTERVAL)+1, 5, SERVO_HEIGHT-1);
	}
	
	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.drawImage(img, 0, 0, this);
	}

	public int add(int aPin) {
		pin[nbActiveServos] = aPin;
		nbActiveServos ++;
		return nbActiveServos-1;
	}
	
	public void setValue(int index, int value) {
		servo[index] = value;
		drawServo(img.getGraphics(), index);
		repaint();
	}
	
	public Dimension getPreferredSize() {
		return new Dimension(img.getWidth(), img.getHeight());
	}

}
