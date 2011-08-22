package nitwhips.whipulator;

import processing.core.*;
import processing.serial.*;
import java.io.*;

public class FFTRecorder extends PApplet {
	
	static int WINDOW_SIZE = 64;
	static String[] AXES = {"X", "Y"};
	static int MARK_LEN = 2;

	
	int accel_index;
	Serial accelerometer;

	String[] serial_list;
	Button[] serial_buttons;
	
	// PrintWriter data_output;
	
	int[][] fft_amp;
	int max;
	int[][] maxes;
	
	PFont font;
	
	boolean ok;
	
	int count;
	int green, blue;
		
	public void setup() {
		size(1000, 900);
		background(0);
		
		font = createFont("Helvetica", 14);
		textFont(font, 12);
		
		long now = System.currentTimeMillis()/1000;
		// data_output = createWriter("accel-recording-"+now+".txt");
		
		fft_amp = new int[AXES.length][WINDOW_SIZE];
		maxes = new int[AXES.length][WINDOW_SIZE];
		
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
						accelerometer = new Serial(FFTRecorder.this, Serial.list()[tmp], 115200);
						accelerometer.buffer(AXES.length*WINDOW_SIZE*2+MARK_LEN);
					}
				}, serial_list[i]);
			}
		} else if(accel_index >= 0 && accel_index < serial_list.length) {
			accelerometer = new Serial(this, serial_list[accel_index], 115200);
			accelerometer.buffer(AXES.length*WINDOW_SIZE*2+MARK_LEN);
		}
	}
	
	public void draw() {
		background(0, green, blue);
		
		if(count < 3) {
			green--;
			blue--;
			if(green < 0) green = 0;
			if(blue < 0) blue = 0;
		} else if(count < WINDOW_SIZE/6) {
			if(blue > 0) blue -= 2;
			green++;
			if(green > 255) green = 255;
		} else {
			if(green > 0) green -= 2;
			blue++;
			if(blue > 255) blue = 255;
		}
		count = 0;
		
		if(accel_index != -1) {
			strokeWeight(2);
			stroke(150);
			fill(150);
						
			for(int i = 0;i < AXES.length;i++) {
				fill(150);
				text(AXES[i], 25, i*height/AXES.length+25);
				if(i != 0) line(0, i*height/AXES.length, width, i*height/AXES.length);
				for(int j = 0;j < WINDOW_SIZE;j++) {
					if(fft_amp[i][j] > map(j, 0, WINDOW_SIZE, 0.5f, 0.8f)*maxes[i][j]) {
						fill(200, 0, 0);
						count++;
					}
					else fill(150);
					rect(j*width/WINDOW_SIZE, (i+1)*height/AXES.length, width/WINDOW_SIZE, -map(fft_amp[i][j], 0, maxes[i][j], 0, height/AXES.length));
					// rect(j*width/WINDOW_SIZE, (i+1)*height/AXES.length, width/WINDOW_SIZE, -map(fft_amp[i][j], 0, max*1.1f, 0, height/AXES.length));
				}
			}
	
			stroke(150);
			fill(150);
			text(serial_list[accel_index], 10, height-10);
		}
	}
		
	public void stop() {
		// data_output.flush();
		// data_output.close();
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

		byte[] incoming = new byte[AXES.length*WINDOW_SIZE*2];
		incoming = port.readBytes();
		// data_output.print(millis()+"\t");
		for(int i = 0;i < AXES.length;i++) {
			for(int j = 0;j < WINDOW_SIZE;j++) {
				fft_amp[i][j] = (int)(((incoming[i*WINDOW_SIZE*2+j*2] & 0xFF) << 8) | incoming[i*WINDOW_SIZE*2+j*2+1] & 0xFF);
				// data_output.print(data[i][data_index%data[i].length]+"\t");
				if(fft_amp[i][j] > maxes[i][j]) maxes[i][j] = fft_amp[i][j];
				if(fft_amp[i][j] > max) max = fft_amp[i][j];
			}
		}
		
		// data_output.println();
		
		// data_output.flush();
	}
	
	public static void main (String [] args) {
		PApplet.main(new String[] { "nitwhips.whipulator.FFTRecorder" });
	}
	
}
