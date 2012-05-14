package com.google.code.ardurct.libraries;

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Label;
import java.awt.TextArea;

import javax.swing.JPanel;


public class HardwareSerial extends JPanel {

	public static final int DEFAULT_BAUD_RATE = 19200;
	
	private static final long serialVersionUID = 1L;

	public TextArea receivedQueue;
	public TextArea inboundQueue;
	public Label configuration;
	
	private int inboundQueueLength = 0;
	private int receivedQueueLength = 0;
	
	protected int baudrate = DEFAULT_BAUD_RATE;
	
	public HardwareSerial(String aConfiguration) {
		Font font = new Font("Courier", Font.PLAIN, 11);
		receivedQueue = new TextArea(5, 20);
		receivedQueue.setFont(font);
		receivedQueue.setEditable(false);
		inboundQueue = new TextArea(4, 20);
		inboundQueue.setFont(font);
		setLayout(new BorderLayout(2, 2));
		configuration = new Label(aConfiguration);
		add(configuration, BorderLayout.NORTH);
		add(inboundQueue, BorderLayout.CENTER);
		add(receivedQueue, BorderLayout.SOUTH);		
	}

	public void clear() {
		receivedQueue.setText("");
		inboundQueue.setText("");
		inboundQueueLength = 0;
		receivedQueueLength = 0;		
	}
	
	public void setConfiguration(String config) {
		configuration.setText(config);
	}
	
	public void begin(int aBaudrate) {
		baudrate = aBaudrate;
	}
	
	public int getBaudrate() {
		return baudrate;
	}
	
	public void print(String s) {
		receivedQueue.append(s);
	}

	public void print(byte[] s) {
		receivedQueue.append(new String(s, 0, s.length));
	}

	public void print(int[] s) {
		byte bS[] = new byte[s.length];
		for (int i=0; i<s.length; i++) bS[i] = (byte)s[i];
		receivedQueue.append(new String(bS, 0, s.length));
	}

	public void print(int n) {
		receivedQueue.append(n+"");
	}

	public void print(char c) {
		receivedQueue.append("" + ((char)c));
	}

	public void print(long n) {
		receivedQueue.append(n+"");
	}

	public void print(int n, int radix) {
		receivedQueue.append(Integer.toString(n, radix) + "");
	}

	public void print(long n, int radix) {
		receivedQueue.append(Long.toString(n, radix) + "");
	}

	public void println(String s) {
		receivedQueue.append(s + "\n");
	}

	public void println(byte[] s) {
		print(s);
		receivedQueue.append("\n");
	}

	public void println(int[] s) {
		print(s);
		receivedQueue.append("\n");
	}

	
	public void println(int n) {
		receivedQueue.append(n + "\n");
	}

	public void println(long n) {
		receivedQueue.append(n + "\n");
	}

	public void println(int n, int radix) {
		receivedQueue.append(Integer.toString(n, radix)  + "\n");
	}

	public void println(long n, int radix) {
		receivedQueue.append(Long.toString(n, radix)  + "\n");
	}

	public int available() {
		return inboundQueue.getText().length() - inboundQueueLength;
	}
	
	public int read() {
		if (available() <= 0) return -1;
		return inboundQueue.getText().charAt(inboundQueueLength++);
	}
	
	public int availableFromReceivedQueue() {
		return receivedQueue.getText().length() - receivedQueueLength;		
	}

	public int readFromReceivedQueue() {
		if (availableFromReceivedQueue() <= 0) return -1;
		return receivedQueue.getText().charAt(receivedQueueLength++);
	}

}
