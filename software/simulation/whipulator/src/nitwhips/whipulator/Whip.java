package nitwhips.whipulator;

public class Whip {
	
	static int MAX_MODES = 5;
	static int HEIGHT = 50;
	static int RADIUS = 1;
	
	int id;
	float base_x, base_y;
	
	boolean active;
	
	float tip_x, tip_y;
	float[][] modes;
	
	int[][][] leds;
	
	//Modes are roughly the fourier tansform of the x and y accelerometer signal. 
	//The 0th mode is the DC offset or the lean of the pole.
	//In reality this will be a little trickier to compute
	
	SimEngine engine;
	
	Whipulator p;

	public Whip(Whipulator p, int id, float base_x, float base_y) {
		this.p = p;

		this.id = id;

		this.base_x = base_x;
		this.base_y = base_y;
		
		active = false;
		
		tip_x = 0;
		tip_y = 0;
		
		modes = new float[2][MAX_MODES];
		
		leds = new int[5][4][3]; //[y][theta][color]
		
		engine = new SimEngine();
	}
	
	public void draw() {
		p.pushMatrix();
		p.translate(base_x, 0, base_y);
		
		for(int i = 0;i < leds.length;i++) {
			drawPanel(i, leds[i]);
		}
		
		p.popMatrix();
	}
	
	void drawPanel(int panel, int face_colors[][]) {
		p.stroke(255);

		int h_sub = HEIGHT/leds.length;

		p.beginShape(p.QUADS);

		p.fill(face_colors[0][0], face_colors[0][1], face_colors[0][2]); 

		p.vertex(base_x/2-RADIUS, -(panel+1)*h_sub, base_y/2+RADIUS);
		p.vertex(base_x/2+RADIUS, -(panel+1)*h_sub, base_y/2+RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*h_sub, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*h_sub, base_y/2+RADIUS);

		p.fill(face_colors[1][0], face_colors[1][1], face_colors[1][2]); 

		p.vertex(base_x/2+RADIUS, -(panel+1)*h_sub, base_y/2+RADIUS);
		p.vertex(base_x/2+RADIUS, -(panel+1)*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*h_sub, base_y/2+RADIUS);

		p.fill(face_colors[2][0], face_colors[2][1], face_colors[2][2]); 

		p.vertex(base_x/2+RADIUS, -(panel+1)*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2-RADIUS, -(panel+1)*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*h_sub, base_y/2-RADIUS);

		p.fill(face_colors[3][0], face_colors[3][1], face_colors[3][2]); 

		p.vertex(base_x/2-RADIUS, -(panel+1)*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2-RADIUS, -(panel+1)*h_sub, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*h_sub, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*h_sub, base_y/2-RADIUS);

		p.fill(100);        

		p.vertex(base_x/2-RADIUS, -(panel+1)*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -(panel+1)*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -(panel+1)*h_sub, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -(panel+1)*h_sub, base_y/2+RADIUS);

		p.fill(100);        

		p.vertex(base_x/2-RADIUS, -panel*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*h_sub, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*h_sub, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*h_sub, base_y/2+RADIUS);

		p.endShape();
	}
	
	public void setActive(boolean active) {
		this.active = active;
	}
	
	public boolean isActive() {
		return active;
	}

	public void setMode(int xy, int mode, float value) {
		if(mode >= 0 && mode < MAX_MODES && xy >= 0 && xy < 2) {
			modes[xy][mode] = value;
		} else {
			System.err.println("Invalid mode data given");
		}
	}
	
	public void setModes(float[][] modes) {
		if(modes.length >= 2 && modes[0].length >= MAX_MODES) this.modes = modes;
	}

	class SimEngine implements Runnable {
		boolean running = true;
		
		public SimEngine() {
			running = true;
			Thread t = new Thread(this);
			t.setDaemon(true);
			t.start();
		}

		public void run() {
			while(running) {
				tip_x = 0;
				tip_y = 0;
				try {
					Thread.sleep(10);
				} catch(Exception e) {
					
				}
			}
		}
	}

}