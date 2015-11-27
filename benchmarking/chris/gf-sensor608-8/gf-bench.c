#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include "gf.h" // Change this

// Default # of repeats
#define REPEAT	10000000    // Must be divisible by 8

// Main
int
main(int argc, char **argv)
{
	// Variables
	int		i;
	struct timeval	start, end;
	// Change # of repeats according to the processing bits
	int		repeat = REPEAT / 1; // 8bits
	//int		repeat = REPEAT / 2; // 16bits
	//int		repeat = REPEAT / 4; // 32bits
	//int		repeat = REPEAT / 8; // 64bits
	uint8_t		a, b; // Change type of these according to the processing bits
	//uint16_t	a, b; // 16bits
	//uint32_t	a, b; // 32bits
	//uint64_t	a, b; // 64bits


	// Initialize GF
	//GFinit(); // Some libraries need initialization

	// Start measuring elapsed time
	gettimeofday(&start, NULL); // Get start time

	// Repeat mul and div in GF
	for (i = 0; i < repeat; i++) {
		// Note change these according to the bits,
		// random() must be 64bits
		a = (uint8_t)(random() & 0xff);
		b = (uint8_t)(random() & 0xff);
		// a = (uint16_t)(random() & 0xffff); // In case of 16bits

		// Calculate in GF
		gf_mul(a, b); // You may need to change this function according to the bits like GF8mul(), GF64mul()
		gf_div(a, b);
	}

	// Get end time
	gettimeofday(&end, NULL);

	// Print result
	printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}
