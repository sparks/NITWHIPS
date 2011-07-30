package nitwhips.whipulator;

import java.util.*;

public class DataBus {

	MasterController master;
	Vector<Whip> whips;

	public DataBus() {
		whips = new Vector<Whip>();
	}
	
	public void connectController(MasterController master) {
		this.master = master;
	}
	
	public void connectWhip(Whip whip) {
		whips.add(whip);
	}

}