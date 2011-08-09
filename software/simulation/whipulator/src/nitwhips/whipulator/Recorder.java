package nitwhips.whipulator;

import processing.core.*;

public class Recorder {
	PrintWriter output_events, output_locations, output_pano;
	output_events = createWriter("experiment-"+now+"-events.txt");
	public Recorder() {
		
	}
	

	output_events.println(millis()+"\t"+comment+"\t"+type);
	output_events.flush();
	
	output_events.flush();
	output_events.close(); // Finishes the file
	
	
}