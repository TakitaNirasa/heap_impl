#include "../include/heap.h"
#include "stdint.h"
#include "stdio.h"

void show_ptr (void* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if ((i & 7) == 0)
    		printf ("\r\n %p ", &((uint64_t*)data)[i]);//i+1);
        printf ("0x%08lx\t", ((uint64_t*)data)[i]);
    }
    printf ("\r\n");

}

int main(void)
{
	
	uint64_t mass[100];

	heapInit (mass, sizeof (mass) / sizeof (size_t));
	show_ptr (mass, sizeof (mass) / sizeof (size_t));
    uint64_t* ptr = (uint64_t *)heapAlloc (10);
	show_ptr (mass, sizeof (mass) / sizeof (size_t));
    uint64_t* ptr1 = (uint64_t *)heapAlloc (11);
    heapFree (ptr);
    ptr = (uint64_t *)heapAlloc (4);
    ptr = (uint64_t *)heapAlloc (1);
	show_ptr (mass, sizeof (mass) / sizeof (size_t));

    return 0;
}