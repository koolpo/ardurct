package com.google.code.ardurct;

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Label;
import java.awt.TextArea;

import javax.swing.JPanel;


public class HardwareSerial extends JPanel {

	private static final long serialVersionUID = 1L;

	TextArea from;
	TextArea to;
	
	private int ptr = 0;
	
	public HardwareSerial() {
		Font font = new Font("Courier", Font.PLAIN, 11);
		from = new TextArea(5, 20);
		from.setFont(font);
		from.setEditable(false);
		to = new TextArea(4, 20);
		to.setFont(font);
		setLayout(new BorderLayout(2, 2));
		add(new Label("Serial"), BorderLayout.NORTH);
		add(to, BorderLayout.CENTER);
		add(from, BorderLayout.SOUTH);		
	}

	public void clear() {
		from.setText("");
		to.setText("");
		ptr = 0;
	}

	public void begin(int speed) {	
	}
	
	public void print(String s) {
		from.append(s);
	}

	public void print(byte[] s) {
		from.append(new String(s, 0, s.length));
	}

	public void print(int[] s) {
		byte bS[] = new byte[s.length];
		for (int i=0; i<s.length; i++) bS[i] = (byte)s[i];
		from.append(new String(bS, 0, s.length));
	}

	public void print(int n) {
		from.append(n+"");
	}

	public void print(long n) {
		from.append(n+"");
	}

	public void print(int n, int radix) {
		from.append(Integer.toString(n, radix) + "");
	}

	public void print(long n, int radix) {
		from.append(Long.toString(n, radix) + "");
	}

	public void println(String s) {
		from.append(s + "\n");
	}

	public void println(byte[] s) {
		print(s);
		from.append("\n");
	}

	public void println(int[] s) {
		print(s);
		from.append("\n");
	}

	
	public void println(int n) {
		from.append(n + "\n");
	}

	public void println(long n) {
		from.append(n + "\n");
	}

	public void println(int n, int radix) {
		from.append(Integer.toString(n, radix)  + "\n");
	}

	public void println(long n, int radix) {
		from.append(Long.toString(n, radix)  + "\n");
	}

	public boolean available() {
		int toLength = to.getText().length();
		return (toLength- ptr > 0);
	}
	
	public int read() {
		if (!available()) return -1;
		return to.getText().charAt(ptr++);
	}
}
