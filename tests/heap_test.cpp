#include "gtest/gtest.h"
#include "../src/libs/heap.c"
#include <string.h>

/**
 * @brief Тест инициализации обозначения места под кучу
 */
TEST (testInitHeap, HeapTest)
{
    // Неверные параметры
    heapInit (nullptr, 100, 8);
    EXPECT_EQ (heapAlloc (5), nullptr);

    // Массив для кучи
    uint8_t mass [100];
    memset (mass, 0, sizeof (mass));
    // Массив для сравнения
    uint8_t zeroMass [100];
    memset (zeroMass, 0, sizeof (zeroMass));

    heapInit (mass, 0, 8);
    EXPECT_EQ (heapAlloc (5), nullptr); 
    EXPECT_EQ (memcmp (mass, zeroMass, sizeof (mass)), 0);
}

/**
 * @brief Тест выделения пространства на 8ми битной платформе
 */
TEST (testAllocation8, HeapTest)
{

}

/**
 * @brief Тест выделения пространства на 16ти битной платформе
 */
TEST (testAllocation16, HeapTest)
{
    
}

/**
 * @brief Тест выделения пространства на 32х битной платформе
 */
TEST (testAllocation32, HeapTest)
{
    
}

/**
 * @brief Тест выделения пространства на 64х битной платформе
 */
TEST (testAllocation64, HeapTest)
{
    
}
