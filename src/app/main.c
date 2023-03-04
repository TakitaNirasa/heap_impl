#include "../include/heap.h"
#include "stdint.h"

int main(void)
{
	uint8_t mass [1000];

	heapInit (mass, sizeof(mass), sizeof (size_t));
	uint8_t* whereAmI = heapAlloc (16);
	whereAmI[0] = 22;
	uint8_t* whereAmI2 = heapAlloc (20);
	whereAmI2[0] = 255;
	heapFree (whereAmI);
	whereAmI = heapAlloc (32);
	whereAmI[0] = 22;
	heapFree (whereAmI);

    return 0;
}
