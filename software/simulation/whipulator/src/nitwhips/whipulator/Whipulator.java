package nitwhips.whipulator;

import processing.core.*;
import processing.opengl.*;

public class Whipulator extends PApplet {

	Whip[] whips;
	
	WhipulatorMode mode;
	
	int grid_size, grid_div;
	
	int prev_mouse_x, prev_mouse_y;
	float rot_x, rot_y;
	
	PFont font;
		
	public void setup() {
		size(1000, 800, OPENGL);
		background(0);
		
		setMode(WhipulatorMode.PERSPECTIVE);
		
		whips = new Whip[] {
			new Whip(this, 0, 0, 0),
			new Whip(this, 0, 10, 0),
			new Whip(this, 0, -20, 5),
			new Whip(this, 0, 15, 10)
		};
		
		grid_size = 150;
		grid_div = 15;
		
		rot_x = -0.5f;
		rot_y = 0;
		
		font = createFont("Helvetica", 14);
		textFont(font, 12);
	}
	
	public void draw() {
		background(0);
		
		pushMatrix();

		translate(width/2, height/2, 500);
		rotateX(rot_x);
		rotateY(rot_y);
		
		for(int i = 0;i < grid_div+1;i++) {
			line(-grid_size/2+grid_size/grid_div*i, 0, -grid_size/2, -grid_size/2+grid_size/grid_div*i, 0, grid_size/2);
			line(-grid_size/2, 0, -grid_size/2+grid_size/grid_div*i, grid_size/2, 0, -grid_size/2+grid_size/grid_div*i);
		}
	
		for(Whip whip : whips) {
			whip.draw();
		}
		
		popMatrix();
		
		//Text
		
		fill(255);
		stroke(255);
		text(mode.toString(), width-150, 20);
		
		//Buttons
						
		strokeWeight(3);
		stroke(150);
		for(int i = 0;i < 4;i++) {
			if(mode.getNum() == i) fill(255);
			else if(mouseX > 10+20*i && mouseX < 10+20*(i+1) && mouseY > 10 && mouseY < 35) fill(100);
			else fill(0);
			rect(10+20*i, 10, 15, 15);
		}
		
		strokeWeight(1);
	}
	
	public void mousePressed() {
		prev_mouse_x = mouseX;
		prev_mouse_y = mouseY;
	}
	
	public void mouseDragged() {
		rot_x -= (float)(mouseY-prev_mouse_y)/height*PI;
		rot_y += (float)(mouseX-prev_mouse_x)/width*PI;
		
		prev_mouse_x = mouseX;
		prev_mouse_y = mouseY;
		
		mode = WhipulatorMode.PERSPECTIVE;
	}
		
	public void mouseReleased() {
		for(int i = 0;i < 4;i++) {
			if(mouseX > 10+20*i && mouseX < 10+20*(i+1) && mouseY > 10 && mouseY < 35) 
			for(WhipulatorMode w : WhipulatorMode.values()) if(w.getNum() == i) setMode(w);
		}
	}
	
	public void keyPressed() {
		if(key == '1') {
			setMode(WhipulatorMode.FRONT);
		} else if(key == '2') {
			setMode(WhipulatorMode.SIDE);
		} else if(key == '3') {
			setMode(WhipulatorMode.TOP);
		} else if(key == '4') {
			setMode(WhipulatorMode.PERSPECTIVE);
		}
	}
	
	public void setMode(WhipulatorMode mode) {
		switch (mode) {
			case PERSPECTIVE:
				rot_x = -0.5f;
				rot_y = 0;
				break;
			case TOP:
				rot_x = -PI/2;
				rot_y = 0;
				break;
			case FRONT:
				rot_x = 0;
				rot_y = 0;
				break;
			case SIDE:
				rot_x = 0;
				rot_y = PI/2;
				break;
		}
		
		this.mode = mode;
	}
	
	public static void main (String [] args) {
		PApplet.main(new String[] { "nitwhips.whipulator.Whipulator" });
	}
	
}