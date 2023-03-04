#include "stdint.h"

void heapInit (void* heap, uint16_t size);
uint8_t* alloc (uint16_t size);
void free (uint8_t* mem);