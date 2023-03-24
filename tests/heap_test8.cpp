#include "gtest/gtest.h"
#include <string.h>

#define block_size_t uint8_t
#include "../src/libs/heap.c"

class Heap8BitTest : public ::testing::Test
{
protected:
    // Размер структуры инициализации в единицах разрядности системы
    const block_size_t memStructSize = (sizeof (mem_t) / sizeof (block_size_t)) + 
									(sizeof (mem_t) & (sizeof (block_size_t) - 1) ? 1 : 0);
};

/**
 * @brief Сравнить адрес два элемента массива uint32 с адресом, который должен там храниться
 * 
 * @param mass Указатель на массив данных
 * @param addr Указатель который должен храниться в \p mass
 */
void addr_compare (uint8_t* const  mass, uint8_t* const addr)
{
    EXPECT_EQ (mass[1], (reinterpret_cast<uint64_t>(addr) >> UINT32_WIDTH));
    EXPECT_EQ (mass[0], reinterpret_cast<uint64_t>(addr) & UINT32_MAX);
}

/**
 * @brief Тест инициализации обозначения места под кучу
 */
TEST (Heap32BitTest, InitTest)
{
    // Неверные параметры
    heapInit (nullptr, 100);
    EXPECT_EQ (heapAlloc (5), nullptr);

    // Массив для кучи
    uint8_t mass [100];
    memset (mass, 0, sizeof (mass));
    // Массив для сравнения
    uint32_t zeroMass [100];
    memset (zeroMass, 0, sizeof (zeroMass));

    heapInit (mass, 0);
    EXPECT_EQ (heapAlloc (5), nullptr); 
    EXPECT_EQ (memcmp (mass, zeroMass, sizeof (mass)), 0);

    // Если нет возможности указать следующий элемент, так как он за пределами кучи
    heapInit (mass, memStructSize);
    EXPECT_EQ (heapAlloc (5), nullptr); 
    // Проверка на валидные начальные элементы кучи при успешной инициализации
    heapInit (mass, memStructSize+1);

    // Проверка начального адреса
    addr_compare (mass, mass);
    
    // Проверка элемента, указывающего на следующий элемент
    addr_compare (mass + 2, mass + 5);
    EXPECT_EQ (mass[4], 1);

    // Проверка повторной инициализации
    uint8_t mass2[10];
    memset (mass2, 0, sizeof (mass2));
    heapInit (mass2, memStructSize+1);
    EXPECT_EQ (memcmp (mass2, zeroMass, sizeof (mass2)), 0);

    // Деинициализация всей кучи
    heapFree (mass);
    EXPECT_EQ (memcmp (mass, zeroMass, sizeof (mass)), 0);
}
