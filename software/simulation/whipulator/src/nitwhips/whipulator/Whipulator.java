package nitwhips.whipulator;

import processing.core.*;
import processing.opengl.*;

public class Whipulator extends PApplet {
	
	public Whipulator() {
		
	}
	
	public void setup() {
		size(700, 500, OPENGL);
		background(0);
	}
	
	public void draw() {
		background(0);
		pushMatrix();
		translate(width/2, height/2, map(mouseX, 0, width, -200, 200));
		rotateY(0.5f);
		fill(100);
		stroke(255);
		box(50, 50, 50);
		popMatrix();
	}
	
	public static void main (String [] args) {
		PApplet.main(new String[] { "nitwhips.whipulator.Whipulator" });
	}
	
}