#include "../include/heap.h"
#include "stdint.h"

int main(void)
{
	uint8_t mass [1000];

	heapInit (mass, sizeof(mass));
	uint8_t* whereAmI = alloc (16);
	whereAmI[0] = 22;
	uint8_t* whereAmI2 = alloc (20);
	whereAmI2[0] = 255;
	free (whereAmI);
	whereAmI = alloc (32);
	whereAmI[0] = 22;
	free (whereAmI);

    return 0;
}