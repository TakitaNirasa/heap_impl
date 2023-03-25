#include "gtest/gtest.h"
#include <string.h>

#define block_size_t uint64_t
#include "../src/libs/heap.c"


class Heap64BitTest : public ::testing::Test
{
protected:
    // Размер структуры инициализации в единицах разрядности системы
    const block_size_t memStructSize = (sizeof (mem_t) / sizeof (block_size_t)) + 
									(sizeof (mem_t) & (sizeof (block_size_t) - 1) ? 1 : 0);
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
    uint64_t mass [100];
    memset (mass, 0, sizeof (mass));
    // Массив для сравнения
    uint64_t zeroMass [100];
    memset (zeroMass, 0, sizeof (zeroMass));

    heapInit (mass, 0);
    EXPECT_EQ (heapAlloc (5), nullptr); 
    EXPECT_EQ (memcmp (mass, zeroMass, sizeof (mass)), 0);

    // Если нет возможности указать следующий элемент, так как он за пределами кучи
    heapInit (mass, memStructSize);
    EXPECT_EQ (heapAlloc (5), nullptr); 

    // Проверка на валидные начальные элементы кучи при успешной инициализации
    heapInit (mass, memStructSize+1);
    EXPECT_EQ (mass[0], reinterpret_cast<uint64_t>(mass));
    EXPECT_EQ (mass[1], reinterpret_cast<uint64_t>(mass + memStructSize));
    EXPECT_EQ (mass[2], 1);

    // Проверка повторной инициализации
    uint64_t mass2[10];
    memset (mass2, 0, sizeof (mass2));
    heapInit (mass2, memStructSize+1);
    EXPECT_EQ (memcmp (mass2, zeroMass, sizeof (mass2)), 0);

    // Деинициализация всей кучи
    heapFree (mass);
    EXPECT_EQ (memcmp (mass, zeroMass, sizeof (mass)), 0);
}

/**
 * @brief Тест выделения места под кучу
 */
TEST (Heap64BitTest, HeapAllocTest)
{
    // Массив для кучи
    uint64_t mass [100];
    memset (mass, 0, sizeof (mass));

    heapInit (mass, memStructSize+1);
    EXPECT_EQ (heapAlloc (5), nullptr);

    // Неверные параметры для размещения данных в памяти
    EXPECT_EQ (heapAlloc (5), nullptr);
    EXPECT_EQ (heapAlloc (0), nullptr);
    heapFree (mass);

    heapInit (mass, 40);
    // |начальный блок|-|блок1|-|5 слов|
    EXPECT_EQ (heapAlloc (5), mass + (memStructSize * 2));
    // |начальный блок|-|блок1|-|5 слов|-|блок2|-|5 слов|
    EXPECT_EQ (heapAlloc (5), mass + (memStructSize * 3) + 5);
    // |начальный блок|-|пусто|-|блок2|-|5 слов|
    heapFree (mass + (memStructSize * 2));
    // |начальный блок|-|блок1|-|5 слов|-|блок2|-|5 слов|
    EXPECT_EQ (heapAlloc (5), mass + (memStructSize * 2));
    // |начальный блок|-|пусто|-|блок2|-|5 слов|
    heapFree (mass + (memStructSize * 2));
    // |начальный блок|-|пусто|-|блок2|-|5 слов|-|блок3|-|6 слов|
    EXPECT_EQ (heapAlloc (6), mass + (memStructSize * 4) + 10);

    // Блок не поместится в память
    EXPECT_EQ (heapAlloc (40), nullptr);

    // Блок памяти битый
    mass[0] = 0;
    EXPECT_EQ (heapAlloc (5), nullptr);
    heapFree (mass);
    memset (mass, 0, sizeof (mass));

    heapInit (mass, 25);

    mass[1] = 0;
    EXPECT_EQ (heapAlloc (5), nullptr);
    heapFree (mass);
    memset (mass, 0, sizeof (mass));

    heapInit (mass, 10);
    // |начальный блок|-|блок1|-|1 слов|
    EXPECT_EQ (heapAlloc (1), mass + (memStructSize * 2));
    // Блок не полностью помещается в память
    EXPECT_EQ (heapAlloc (3), nullptr);
    heapFree (mass);

    // Мусор в куче
    heapInit (mass, 10);
    memset (mass + memStructSize, 255, (10 - memStructSize) * 8);
    // |начальный блок|-|блок1|-|1 слов|
    EXPECT_EQ (heapAlloc (1), mass + (memStructSize * 2));
    // Блок не полностью помещается в память
    EXPECT_EQ (heapAlloc (3), nullptr);
    heapFree (mass);
}

/**
 * @brief Тест очистки участков памяти
 */
TEST (Heap64BitTest, HeapFreeTest)
{
    // Массив для кучи
    uint64_t mass [100];
    memset (mass, 0, sizeof (mass));

    // Проверка на входные параметры
    heapFree (nullptr);
    heapInit (mass, 25);
    heapFree (nullptr);

    EXPECT_EQ (mass[0], reinterpret_cast<uint64_t>(mass));
    EXPECT_EQ (mass[1], reinterpret_cast<uint64_t>(mass + memStructSize));
    EXPECT_EQ (mass[2], 25 - memStructSize);
}
