#include <vector>
#include <iostream>

#include "speedtester.h"

int main() {
	float *results = speedtest();

	printf("Download Speed: %fMbps, Upload Speed: N/A", results[0], results[1]);

	return 0;
}