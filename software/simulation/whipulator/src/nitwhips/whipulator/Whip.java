package nitwhips.whipulator;

public class Whip {
	
	static int MAX_MODE = 4;
	
	float x, y;
	
	float lean_magnitude, lean_angle;
	float rotation_speed, rotation_magnitude;
	float[] mode_magnitudes, mode_angles;
	
	public void Whip(float x, float y) {
		this.x = x;
		this.y = y;
		
		lean_angle = 0;
		lean_magnitude = 0;
		
		rotation_speed = 0;
		rotation_magnitude = 0;
		
		mode_magnitudes = new float[MAX_MODE];
		mode_angles = new float[MAX_MODE];
	}
	
	public void draw() {
		
	}
	
	public void setLean(float magnitude, float angle) {
		this.lean_magnitude = magnitude;
		this.lean_angle = angle;
	}
	
	public void setRotation(float magnitude, float speed) {
		this.rotation_magnitude = magnitude;
		this.rotation_speed = speed;
	}
	
	public void setMode(int mode, float magnitude, float angle) {
		this.mode_magnitudes[mode] = magnitude;
		this.mode_angles[mode] = angle;
	}
	
	public void setModes(float[] mode_magnitudes, float[] mode_angles) {
		if(mode_magnitudes.length >= MAX_MODE) this.mode_magnitudes = mode_magnitudes;
		if(mode_angles.length >= MAX_MODE) this.mode_angles = mode_angles;
	}

}