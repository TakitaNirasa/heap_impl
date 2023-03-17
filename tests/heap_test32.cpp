#include "gtest/gtest.h"
#include <string.h>

#define block_size_t uint32_t
#include "../src/libs/heap.c"

class Heap32BitTest : public ::testing::Test
{
};

void show_ptr (void* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if ((i & 7) == 0)
    		printf ("\r\n %p ", &((uint32_t*)data)[i]);//i+1);
        printf ("0x%08x\t", ((uint32_t*)data)[i]);
    }
    printf ("\r\n");

}

/**
 * @brief Тест инициализации обозначения места под кучу
 */
TEST (Heap32BitTest, InitTest)
{
    uint32_t mass[100];
    memset (mass, 0, sizeof (mass));
    // Неверные параметры
    heapInit (nullptr, 100);
    EXPECT_EQ (heapAlloc (5), nullptr);

    memset (mass, 0, sizeof (mass));
    // Массив для сравнения
    uint32_t zeroMass [100];
    memset (zeroMass, 0, sizeof (zeroMass));

    heapInit (mass, 0);
    EXPECT_EQ (heapAlloc (5), nullptr); 
    EXPECT_EQ (memcmp (mass, zeroMass, sizeof (mass)), 0);

    heapInit (mass, 100);
    uint64_t addr = mass[0] | ((uint64_t)mass[1] << (sizeof(uint32_t) * 8));
    EXPECT_EQ ((uint64_t*)addr, (uint64_t*)mass); 

    //heapAlloc (5);
    show_ptr (mass, 100);
    printf ("0x%08lx\r\n", *((uint64_t*)mass));
    printf ("0x%08lx\r\n", *((uint64_t*)(mass+2)));
    printf ("0x%08lx\r\n", *((uint64_t*)(mass+4)));
}

