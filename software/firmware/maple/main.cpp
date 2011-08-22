#include <stdio.h>
#include "wirish.h"

#include "accelerometer.h"

Accelerometer accelerometer;

void setup() {

}

void loop() {
	accelerometer.pollAndUpdate();
	accelerometer.sendPosition();
}

__attribute__((constructor)) void premain() {
	init();
}

int main(void) {
	setup();

	while(1) loop();

	return 1;
}
