package nitwhips.whipulator;

import processing.core.*;
import processing.opengl.*;

public class Whipulator extends PApplet {
	
	Whip[] whips;
	
	public Whipulator() {
		whips = new Whip[] {
			new Whip(this, 0, 0, 0),
			new Whip(this, 0, 10, 0),
			new Whip(this, 0, -20, 5),
			new Whip(this, 0, 15, 10)
		};
	}
	
	public void setup() {
		size(700, 500, OPENGL);
		background(0);
	}
	
	public void draw() {
		background(0);
		
		pushMatrix();
		
		translate(width/2, height/2+20, map(mouseY, 0, height, 250, 350));
		rotateX(-0.5f);
		rotateY(map(mouseX, 0, width, -PI, PI));
		
		for(Whip whip : whips) {
			whip.draw();
		}
		
		popMatrix();
	}
	
	public static void main (String [] args) {
		PApplet.main(new String[] { "nitwhips.whipulator.Whipulator" });
	}
	
}