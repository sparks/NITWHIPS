package nitwhips.whipulator;

import processing.core.*;
import processing.serial.*;
import java.io.*;

public class AccelRecorder extends PApplet {
	
	static int WINDOW_SIZE = 128;
	String[] AXES = {"X", "Y"};
	static int MARK_LEN = 2;
	
	int accel_index;
	Serial accelerometer;

	String[] serial_list;
	Button[] serial_buttons;
	
	int[] max;
	
	// PrintWriter data_output;
	
	int[][] data;
	int data_index;
	
	PFont font;
		
	boolean ok, lowpass;

	public void setup() {
		size(1000, 900);
		background(0);
		
		font = createFont("Helvetica", 14);
		textFont(font, 12);
		
		max = new int[AXES.length];
		
		long now = System.currentTimeMillis()/1000;
		// data_output = createWriter("accel-recording-"+now+".txt");
		
		data_index = 0;
		data = new int[AXES.length][width];
		
		accel_index = 0;
		serial_list = Serial.list();

		if(accel_index == -1) {
			serial_buttons = new Button[serial_list.length];
			for(int i = 0;i < serial_list.length;i++) {
				final int tmp = i;
				serial_buttons[i] = new Button(this, 75, height/2-30*serial_list.length/2+30*i, new EventListener() {
					public void event(int arg) {
						accel_index = tmp;
						serial_buttons[tmp].clear();
						for(int i = 0;i < serial_buttons.length;i++) serial_buttons[i].setVisible(false);
						accelerometer = new Serial(AccelRecorder.this, Serial.list()[tmp], 115200);
						accelerometer.buffer(AXES.length*2+MARK_LEN);
					}
				}, serial_list[i]);
			}
		} else if(accel_index >= 0 && accel_index < serial_list.length) {
			accelerometer = new Serial(this, serial_list[accel_index], 115200);
			accelerometer.buffer(AXES.length*2+MARK_LEN);
		}
	}
	
	public void draw() {
		background(0);
		
		if(accel_index != -1) {
			strokeWeight(2);
			stroke(150);
			fill(150);
						
			for(int i = 0;i < AXES.length;i++) {
				text(AXES[i], 25, i*height/AXES.length+25);
				if(i != 0) line(0, i*height/AXES.length, width, i*height/AXES.length);
				for(int j = 0;j < data[0].length;j++) {
					// point(j, map(data[i][(data_index+data[0].length-j)%data[0].length], -max[i], max[i], height/AXES.length, 0)+i*height/AXES.length);
					point(j, map(data[i][(data_index+data[0].length-j)%data[0].length], -2048, 2047, height/AXES.length, 0)+i*height/AXES.length);
					
				}
			}
	
			stroke(150);
			fill(150);
			text(serial_list[accel_index], 10, height-10);
		}
		
		line(32, 0, 32, height);
		line(64, 0, 64, height);
		line(128, 0, 128, height);
		line(256, 0, 256, height);
		line(512, 0, 512, height);
	}
		
	public void stop() {
		// data_output.flush();
		// data_output.close();
	}
	
	public void keyPressed() {
		if(key == 'l') lowpass = !lowpass;
	}
	
	public void serialEvent(Serial port) {
		for(int i = 0;i < MARK_LEN;i++) {
			if(port.read() != 255) {
				if(ok) {
					System.out.println("rej");
					ok = false;
				}
				return;
			}
		}
		
		if(!ok) {
			System.out.println("ok");
			ok = true;
		}

		byte[] incoming = new byte[AXES.length*2];
		incoming = port.readBytes();

		data_index++;
	
		// data_output.print(millis()+"\t");
	
		for(int i = 0;i < AXES.length;i++) {
			data[i][data_index%data[0].length] = (int)(((incoming[i*2] & 0xFF) << 8) | incoming[i*2+1] & 0xFF);
			if((data[i][data_index%data[0].length] & 0x800) != 0) data[i][data_index%data[0].length] = data[i][data_index%data[0].length] | 0xFFFFF000;
			if(lowpass) data[i][data_index%data[0].length] = (int)(data[i][data_index%data[0].length]*0.1f+data[i][(data_index-1)%data[0].length]*0.9f);
			// data_output.print(data[i][data_index%data[i].length]+"\t");
		}
		
		max = new int[AXES.length];
		
		for(int i = 0;i < AXES.length;i++) {
			for(int j = 0;j < data[i].length;j++) {
				if(abs(data[i][j]) > max[i]) max[i] = abs(data[i][j]);
			}
		}
		
		// data_output.println();
		
		// data_output.flush();
	}
	
	public static void main (String [] args) {
		PApplet.main(new String[] { "nitwhips.whipulator.AccelRecorder" });
	}
	
}
