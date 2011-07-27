package nitwhips.whipulator;

public class Whip {
	
	static int MAX_MODES = 5;
	
	int id;
	float x, y;
	
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
	}
	
	public void draw() {
		p.fill(100);
		p.stroke(255);
		p.pushMatrix();
		p.translate(x, 0, y);
		p.box(5, 40, 5);
		p.popMatrix();
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