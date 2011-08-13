package nitwhips.whipulator; 

import java.awt.event.MouseEvent;
import processing.core.*;

public class Button {
	
	PApplet p;
	
	int x, y;
	
	String label;
	
	EventListener event_listener;
	boolean depressed, selected, visible;
	
	public Button(PApplet p, int x, int y, EventListener event_listener) {
		this(p, x, y, event_listener, null);
	}
	
	public Button(PApplet p, int x, int y, EventListener event_listener, String label) {
		this.p = p;
		
		this.event_listener = event_listener;
		this.x = x;
		this.y = y;
		
		this.label = label;
		
		selected = false;
		depressed = false;
		visible = true;
		
		p.registerMouseEvent(this);
		p.registerDraw(this);
	}
	
	public void draw() {
		if(visible) {
			p.strokeWeight(3);
			p.stroke(150);
		
			if(selected) p.fill(255);
			else if(p.mouseX > x && p.mouseX < x+25 && p.mouseY > y && p.mouseY < y+25) p.fill(100);
			else p.fill(0);
			p.rect(x, y, 25, 25);

			p.strokeWeight(1);
			
			p.stroke(150);
			p.fill(150);
			
			if(label != null) {
				p.text(label, x+35, y+19);
			}
		}
	}

	public void select() {
		selected = true;
	}
	
	public void clear() {
		selected = false;
	}
	
	public boolean isSelected() {
		return selected;
	}
	
	public boolean isVisible() {
		return visible;
	}
	
	public void setVisible(boolean visible) {
		this.visible = visible;
	}

	public void mouseEvent(MouseEvent event) {
		int mouseX = event.getX();
		int mouseY = event.getY();

		if(visible) {
			if(event.getID() == MouseEvent.MOUSE_RELEASED) {
				if(depressed && mouseX > x && mouseX < x+25 && mouseY > y && mouseY < y+25) {
					event_listener.event(0);
					select();
				}
				depressed = false;
			} else if(event.getID() == MouseEvent.MOUSE_PRESSED) {
				if(mouseX > x && mouseX < x+25 && mouseY > y && mouseY < y+25) depressed = true;
			}
		}
	}
}
