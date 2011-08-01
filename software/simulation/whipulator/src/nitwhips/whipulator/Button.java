package nitwhips.whipulator; 

class Button {
	
	Whipulator p;
	
	int x, y;
	
	EventListener event_listener;
	boolean selected;
	
	public Button(Whipulator p, int x, int y, EventListener event_listener) {
		this.p = p;
		
		this.event_listener = event_listener;
		this.x = x;
		this.y = y;
		selected = false;
	}
	
	public void draw() {
		p.strokeWeight(3);
		p.stroke(150);
		
		if(selected) p.fill(255);
		else if(p.mouseX > x && p.mouseX < x+25 && p.mouseY > y && p.mouseY < y+25) p.fill(100);
		else p.fill(0);
		p.rect(x, y, 25, 25);

		p.strokeWeight(1);
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
	
	public void mouseReleased() {
		if(p.mouseX > x && p.mouseX < x+25 && p.mouseY > y && p.mouseY < y+25) {
			event_listener.event(0);
			select();
		}
	}
}
