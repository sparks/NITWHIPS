package nitwhips.whipulator;

public class Whip {
	
	static int MAX_MODES = 5;
	static int HEIGHT = 50;
	
	int id;
	float x, y;
	
	int[][][] panel_colors;
	float[][] modes;
	//Modes are roughly the fourier tansform of the x and y accelerometer signal. 
	//The 0th mode is the DC offset or the lean of the pole.
	//In reality this will be a little trickier to compute
	
	Whipulator p;

	public Whip(Whipulator p, int id, float x, float y) {
		this.p = p;

		this.id = id;

		this.x = x;
		this.y = y;
		
		modes = new float[2][MAX_MODES];
		
		// panel_colors = new int[2][4][3]; //[panel][face][color]

		panel_colors = new int[][][] {
			new int[][] {
				new int[] {
					255, 0, 0
				},
				new int[] {
					0, 255, 0
				},
				new int[] {
					0, 0, 255
				},
				new int[] {
					255, 255, 0
				}
			},
			new int[][] {
				new int[] {
					100, 0, 0
				},
				new int[] {
					0, 100, 0
				},
				new int[] {
					0, 0, 100
				},
				new int[] {
					100, 100, 0
				}
			}
		};
	}
	
	public void draw() {
		p.pushMatrix();
		p.translate(x, 0, y);
		
		for(int i = 0;i < panel_colors.length;i++) {
			drawPanel(i, panel_colors[i]);
		}
		
		p.popMatrix();
	}
	
	void drawPanel(int panel, int face_colors[][]) {
		p.stroke(255);

		int h_sub = HEIGHT/panel_colors.length;

		p.beginShape(p.QUADS);

		p.fill(face_colors[0][0], face_colors[0][1], face_colors[0][2]); 

		p.vertex(x/2-5, -(panel+1)*h_sub, y/2+5);
		p.vertex(x/2+5, -(panel+1)*h_sub, y/2+5);
		p.vertex(x/2+5, -panel*h_sub, y/2+5);
		p.vertex(x/2-5, -panel*h_sub, y/2+5);

		p.fill(face_colors[1][0], face_colors[1][1], face_colors[1][2]); 

		p.vertex(x/2+5, -(panel+1)*h_sub, y/2+5);
		p.vertex(x/2+5, -(panel+1)*h_sub, y/2-5);
		p.vertex(x/2+5, -panel*h_sub, y/2-5);
		p.vertex(x/2+5, -panel*h_sub, y/2+5);

		p.fill(face_colors[2][0], face_colors[2][1], face_colors[2][2]); 

		p.vertex(x/2+5, -(panel+1)*h_sub, y/2-5);
		p.vertex(x/2-5, -(panel+1)*h_sub, y/2-5);
		p.vertex(x/2-5, -panel*h_sub, y/2-5);
		p.vertex(x/2+5, -panel*h_sub, y/2-5);

		p.fill(face_colors[3][0], face_colors[3][1], face_colors[3][2]); 

		p.vertex(x/2-5, -(panel+1)*h_sub, y/2-5);
		p.vertex(x/2-5, -(panel+1)*h_sub, y/2+5);
		p.vertex(x/2-5, -panel*h_sub, y/2+5);
		p.vertex(x/2-5, -panel*h_sub, y/2-5);

		p.fill(100);        

		p.vertex(x/2-5, -(panel+1)*h_sub, y/2-5);
		p.vertex(x/2+5, -(panel+1)*h_sub, y/2-5);
		p.vertex(x/2+5, -(panel+1)*h_sub, y/2+5);
		p.vertex(x/2-5, -(panel+1)*h_sub, y/2+5);

		p.fill(100);        

		p.vertex(x/2-5, -panel*h_sub, y/2-5);
		p.vertex(x/2+5, -panel*h_sub, y/2-5);
		p.vertex(x/2+5, -panel*h_sub, y/2+5);
		p.vertex(x/2-5, -panel*h_sub, y/2+5);

		p.endShape();
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

}