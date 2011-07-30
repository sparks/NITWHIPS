package nitwhips.whipulator;

public enum WhipulatorMode {
	
	FRONT(0),
	SIDE(1),
	TOP(2),
	PERSPECTIVE(3);
	
	int num;
	
	WhipulatorMode(int num) {
		this.num = num;
	}
	
	public int getNum() {
		return num;
	}
		
}
