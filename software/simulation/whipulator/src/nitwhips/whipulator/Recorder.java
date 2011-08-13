package nitwhips.whipulator;

import processing.core.*;
import processing.serial.*;
import java.io.*;

public class Recorder extends PApplet {
	
	int accel_index;
	Serial accelerometer;

	String[] serial_list;
	Button[] serial_buttons;
	
	PrintWriter data_output;
	
	String[] axes = {"X", "Y", "Z"};
	int[][] data;
	int data_index;
	
	PFont font;

	public void setup() {
		size(1000, 900);
		background(0);
		
		font = createFont("Helvetica", 14);
		textFont(font, 12);
		
		long now = System.currentTimeMillis()/1000;
		data_output = createWriter("accel-recording-"+now+".txt");
		
		data_index = 0;
		data = new int[3][width];
		
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
						accelerometer = new Serial(Recorder.this, Serial.list()[tmp], 115200);
						accelerometer.buffer(6);
					}
				}, serial_list[i]);
			}
		} else if(accel_index >= 0 && accel_index < serial_list.length) {
			accelerometer = new Serial(this, serial_list[accel_index], 115200);
			accelerometer.buffer(6);
		}
	}
	
	public void draw() {
		background(0);
		if(accel_index != -1) {
			strokeWeight(2);
			stroke(150);
			fill(150);

			for(int i = 0;i < 3;i++) {
				text(axes[i], 25, i*height/3+25);
				if(i != 0) line(0, i*height/3, width, i*height/3);
				for(int j = 0;j < width;j++) {
					point((data_index+j)%width, map(data[i][(data_index+j)%width], 0, 1023, height/3, 0)+i*height/3);
				}
			}
	
			stroke(150);
			fill(150);
			text(serial_list[accel_index], 10, height-10);
		}
	}
	
	public void stop() {
		data_output.flush();
		data_output.close();
	}
	
	public void serialEvent(Serial port) {
		byte[] incoming = port.readBytes();
		
		data[0][data_index%width] = (incoming[0] << 8) | incoming[1];
		data[2][data_index%width] = (incoming[2] << 8) | incoming[3];
		data[1][data_index%width] = (incoming[4] << 8) | incoming[5];
		
		data_index++;
		
		data_output.print(millis()+"\t");
		data_output.print(data[0][data_index%width]+"\t");
		data_output.print(data[1][data_index%width]+"\t");
		data_output.println(data[2][data_index%width]);
		
		data_output.flush();
	}
	
	public static void main (String [] args) {
		PApplet.main(new String[] { "nitwhips.whipulator.Recorder" });
	}
	
}