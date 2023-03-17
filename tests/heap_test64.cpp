#include "gtest/gtest.h"
#include <string.h>

#define block_size_t uint64_t
#include "../src/libs/heap.c"


class Heap64BitTest : public ::testing::Test
{
protected:
    uint64_t mass [1000];
};

/**
 * @brief Тест инициализации обозначения места под кучу
 */
TEST (Heap64BitTest, InitTest)
{
    // Неверные параметры
    heapInit (nullptr, 100);
    EXPECT_EQ (heapAlloc (5), nullptr);

    // Массив для кучи
    uint8_t mass [100];
    memset (mass, 0, sizeof (mass));
    // Массив для сравнения
    uint8_t zeroMass [100];
    memset (zeroMass, 0, sizeof (zeroMass));

    heapInit (mass, 0);
    EXPECT_EQ (heapAlloc (5), nullptr); 
    EXPECT_EQ (memcmp (mass, zeroMass, sizeof (mass)), 0);
}
