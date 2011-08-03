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
	
	Button[] view_buttons;
	Button[] whip_buttons;
	
	public void setup() {
		size(1000, 900, OPENGL);
		background(0);
				
		whips = new Whip[] {
			new Whip(this, 0, 0, 0),
			new Whip(this, 1, 10, 0),
			new Whip(this, 2, -20, 5),
			new Whip(this, 3, 15, 10)
		};

		grid_size = 150;
		grid_div = 15;
		
		rot_x = -0.5f;
		rot_y = 0;
		
		font = createFont("Helvetica", 14);
		textFont(font, 12);
		
		view_buttons = new Button[4];
		for(int i = 0;i < 4;i++) {
			final int tmp = i;
			view_buttons[i] = new Button(this, 10+30*i, 10, new EventListener() {
				public void event(int arg) {
					for(int j = 0;j < view_buttons.length;j++) view_buttons[j].clear();
					setMode(WhipulatorMode.values()[tmp]);
				}
			});
		}
		
		view_buttons[3].select();
		
		whip_buttons = new Button[whips.length+1];

		for(int i = 0;i < whips.length;i++) {
			final int tmp = i;
			whip_buttons[i] = new Button(this, 10, 40+30*(i), new EventListener() {
				public void event(int arg) {
					for(int j = 0;j < whip_buttons.length;j++) whip_buttons[j].clear();
					for(int j = 0;j < whips.length;j++) whips[j].selected(false);
					whips[tmp].selected(true);
				}
			});
		}
		
		whip_buttons[whip_buttons.length-1] = new Button(this, 10, 40*(whip_buttons.length-1), new EventListener() {
			public void event(int arg) {
				for(int j = 0;j < whip_buttons.length;j++) whip_buttons[j].clear();
				for(int j = 0;j < whips.length;j++) whips[j].selected(false);
			}
		});
		
		whip_buttons[whip_buttons.length-1].select();

		setMode(WhipulatorMode.PERSPECTIVE);
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
			whip.drawModel();
		}
		
		popMatrix();
		
		for(Whip whip : whips) whip.drawControls();

		//Text
		
		fill(255);
		stroke(255);
		text(mode.toString(), width-150, 20);
	}
	
	public void mousePressed() {
		prev_mouse_x = mouseX;
		prev_mouse_y = mouseY;
	}
	
	public void mouseDragged() {
		if(whip_buttons[whip_buttons.length-1].isSelected() || mouseY < height-250) {
			rot_x -= (float)(mouseY-prev_mouse_y)/height*PI;
			rot_y += (float)(mouseX-prev_mouse_x)/width*PI;
		
			prev_mouse_x = mouseX;
			prev_mouse_y = mouseY;
		
			mode = WhipulatorMode.PERSPECTIVE;
			for(Button b : view_buttons) b.clear();
			view_buttons[mode.getNum()].select();
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
		} else if(key == 'r') {
			for(Whip whip : whips) whip.resetModes();
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
		
		for(Button b : view_buttons) b.clear();
		view_buttons[mode.getNum()].select();
		
		this.mode = mode;
	}
	
	public static void main (String [] args) {
		PApplet.main(new String[] { "nitwhips.whipulator.Whipulator" });
	}
	
}