package nitwhips.whipulator;

public class Whip {
	
	/* Static constants */
	
	static int MAX_MODES = 5;
	static int HEIGHT = 50;
	static int RADIUS = 1;

	/* Drawing */
		
	Whipulator p;
	boolean selected;
	
	/* Whip information */
	
	int id;
	float base_x, base_y;
	
	/* Sensor data */
		
	float tip_x, tip_y;
	
	float[][] tip_history;
	float tip_max[];
	int tip_history_index;
	
	float[][][] modes;
	//Modes are roughly the fourier tansform of the x and y accelerometer signal. 
	//The 0th mode is the DC offset or the lean of the pole.
	//In reality this will be a little trickier to compute
	
	SimEngine engine;
	
	/* LED Output */
	
	int[][][] leds;
	
	public Whip(Whipulator p, int id, float base_x, float base_y) {
		/* Drawing */
	
		this.p = p;
		selected = false;
		
		/* Whip information */
	
		this.id = id;
		this.base_x = base_x;
		this.base_y = base_y;
	
		/* Sensor data */
		
		tip_x = 0;
		tip_y = 0;

		tip_history = new float[0][2];
		tip_max = new float[2];
		tip_history_index = 0;

		modes = new float[MAX_MODES][2][2]; //[mode][x/y][real/complexe] (where real is cos and complexe is sin)
		
		engine = new SimEngine();
		
		/* LED Output */

		leds = new int[5][4][3]; //[panel][face][RGB]
	}
	
	public void drawModel() {
		p.pushMatrix();
		p.translate(base_x, 0, base_y);
		
		for(int i = 0;i < leds.length;i++) {
			drawPanel(i, leds[i]);
		}
		
		p.popMatrix();
	}
	
	public void drawTipWave() {
		if(isSelected()) {
			int x = p.width-350;
			int y = p.height-250;
			int width = 350;
			int height = 250;
		
			if(tip_history.length != width) {
				tip_history = new float[width][2];
				tip_max = new float[2];
				tip_history_index = 0;
			}

			p.strokeWeight(3);
			p.stroke(100);
			p.fill(0);
			p.rect(x, y, width-1, height/2-1);
			p.rect(x, y+height/2, width-1, height/2-1);
		
			p.strokeWeight(3);
			p.stroke(100);
		
			for(int i = 0;i < tip_history.length;i++) {
				p.point(i+x, y+p.map(tip_history[(tip_history_index+i)%tip_history.length][0], -tip_max[0], tip_max[0], 5, height/2-5));
				p.point(i+x, y+height/2+p.map(tip_history[(tip_history_index+i)%tip_history.length][1], -tip_max[1], tip_max[1], 5, height/2-5));
			}
		}
	}
	
	public void drawModeControls() {
		if(isSelected()) {
			int x = 0;
			int y = p.height-250;
			int width = p.width-350;
			int height = 250;
			int padding = 3;
		
			p.strokeWeight(3);
			p.stroke(100);
			p.fill(0);
			p.rect(x, y, width-1, height-1);
	
			p.strokeWeight(1);
			
			p.stroke(255);
			p.fill(255);
			
			float band_width = width/MAX_MODES/4;
			
			
			for(int i = 0;i < MAX_MODES;i++) {
				for(int j = 0;j < 2;j++) {
					for(int k = 0;k < 2;k++) {
						p.rect(padding+(i*4+j*2+k)*band_width, y+height/2, band_width-2*padding, -p.map(modes[i][j][k], -1, 1, -(height-2*padding)/2, (height-2*padding)/2));
					}
				}
			}
		}
	}
	
	void drawPanel(int panel, int face_colors[][]) {
		if(!selected) p.stroke(100);
		else p.stroke(255);
		p.strokeWeight(3);

		int panel_height = HEIGHT/leds.length;

		p.beginShape(p.QUADS);

		p.fill(face_colors[0][0], face_colors[0][1], face_colors[0][2]); 

		p.vertex(base_x/2-RADIUS, -(panel+1)*panel_height, base_y/2+RADIUS);
		p.vertex(base_x/2+RADIUS, -(panel+1)*panel_height, base_y/2+RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*panel_height, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*panel_height, base_y/2+RADIUS);

		p.fill(face_colors[1][0], face_colors[1][1], face_colors[1][2]); 

		p.vertex(base_x/2+RADIUS, -(panel+1)*panel_height, base_y/2+RADIUS);
		p.vertex(base_x/2+RADIUS, -(panel+1)*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*panel_height, base_y/2+RADIUS);

		p.fill(face_colors[2][0], face_colors[2][1], face_colors[2][2]); 

		p.vertex(base_x/2+RADIUS, -(panel+1)*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2-RADIUS, -(panel+1)*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*panel_height, base_y/2-RADIUS);

		p.fill(face_colors[3][0], face_colors[3][1], face_colors[3][2]); 

		p.vertex(base_x/2-RADIUS, -(panel+1)*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2-RADIUS, -(panel+1)*panel_height, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*panel_height, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*panel_height, base_y/2-RADIUS);

		p.fill(100);        

		p.vertex(base_x/2-RADIUS, -(panel+1)*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -(panel+1)*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -(panel+1)*panel_height, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -(panel+1)*panel_height, base_y/2+RADIUS);

		p.fill(100);        

		p.vertex(base_x/2-RADIUS, -panel*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*panel_height, base_y/2-RADIUS);
		p.vertex(base_x/2+RADIUS, -panel*panel_height, base_y/2+RADIUS);
		p.vertex(base_x/2-RADIUS, -panel*panel_height, base_y/2+RADIUS);

		p.endShape();
		
		p.strokeWeight(1);
	}
	
	public boolean isSelected() {
		return selected;
	}
	
	public void selected(boolean selected) {
		this.selected = selected;
	}
	
	public void setMode(int mode, float real_value_x, float img_value_x, float real_value_y, float img_value_y) {
		if(mode >= 0 && mode < MAX_MODES) {
			modes[mode][0][0] = real_value_x;
			modes[mode][0][1] = img_value_x;
			modes[mode][1][0] = real_value_y;
			modes[mode][1][1] = img_value_y;
		} else {
			System.err.println("Invalid mode data given");
		}
	}
	
	public void setModes(float[][][] modes) {
		if(modes.length >= MAX_MODES && modes[0].length >= 2 && modes[0][0].length >= 2) this.modes = modes;
	}
	
	public float[][] getMode(int mode) {
		return modes[mode];
	}

	public float getMode(int mode, int x_y, int real_img) {
		return modes[mode][x_y][real_img];
	}
	
	public void mousePressed() {
		if(isSelected()) {
			mouseAction();
		}
	}
	
	public void mouseDragged() {
		if(isSelected()) {
			mouseAction();
		}
	}

	public void mouseAction() {
	}

	class SimEngine implements Runnable {
		
		Thread sim_thread;
		boolean running = true;
		int start_time;

		public SimEngine() {
			running = true;
		
			start_time = p.millis();
		
			sim_thread = new Thread(this);
			sim_thread.setDaemon(true);
			sim_thread.start();
		}

		public void run() {
			while(running) {
				int time = p.millis()-start_time;
				tip_x = 0;
				tip_y = 0;
				
				for(int i = 0;i < MAX_MODES;i++) {
					tip_x += modes[i][0][0]*p.cos(6*p.PI*i*time/1000)+modes[i][0][1]*p.sin(6*p.PI*i*time/1000);
					tip_y += modes[i][1][0]*p.cos(6*p.PI*i*time/1000)+modes[i][1][1]*p.sin(6*p.PI*i*time/1000);
				}
				
				if(tip_history.length != 0) {
					tip_history[tip_history_index%tip_history.length][0] = tip_x;
					tip_history[tip_history_index%tip_history.length][1] = tip_y;

					if(tip_x > tip_max[0]) tip_max[0] = tip_x;
					if(tip_y > tip_max[1]) tip_max[1] = tip_y;
					
					tip_history_index++;
				}
				
				try {
					Thread.sleep(5);
				} catch(Exception e) {
					
				}
			}
		}
		
		public void stop() {
			running = false;
			while(sim_thread.isAlive()) Thread.yield();
		}
		
	}

}