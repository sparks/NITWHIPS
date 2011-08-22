#include <stdio.h>
#include "wirish.h"
#include "accelerometer.h"

void setup() {
	
}

void loop() {
	
}

__attribute__((constructor)) void premain() {
	init();
}

int main(void) {
	setup();

	while(1) loop();

	return 1;
}
