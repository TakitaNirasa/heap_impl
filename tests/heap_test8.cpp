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
 * @param [in] mass Указатель на массив данных
 * @param [in] addr Указатель который должен храниться в \p mass
 */
void addr_compare (uint8_t* const  mass, uint8_t* const addr)
{
    for (uint8_t i = 0; i < sizeof (uint64_t); i++)
    {
        EXPECT_EQ (mass[i], (reinterpret_cast<uint64_t>(addr) >> (i * UINT8_WIDTH)) & UINT8_MAX);
    }
}

/**
 * @brief Тест инициализации обозначения места под кучу
 */
TEST (Heap8BitTest, InitTest)
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
    addr_compare (mass + 8, mass + memStructSize);
    EXPECT_EQ (mass[16], 1);

    // Проверка повторной инициализации
    uint8_t mass2[10];
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
TEST (Heap8BitTest, HeapAllocTest)
{
    // Массив для кучи
    uint8_t mass [100];
    memset (mass, 0, sizeof (mass));

    heapInit (mass, memStructSize+1);
    EXPECT_EQ (heapAlloc (5), nullptr);

    // Неверные параметры для размещения данных в памяти
    EXPECT_EQ (heapAlloc (5), nullptr);
    EXPECT_EQ (heapAlloc (0), nullptr);
    heapFree (mass);

    heapInit (mass, 100);
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
    memset (mass, 0, sizeof (mass));

    heapInit (mass, 25);

    mass[1] = 0;
    EXPECT_EQ (heapAlloc (5), nullptr);
    heapFree (mass);
    memset (mass, 0, sizeof (mass));

    heapInit (mass, (memStructSize * 2) + 1);
    // |начальный блок|-|блок1|-|1 слов|
    EXPECT_EQ (heapAlloc (1), mass + (memStructSize * 2));
    // Блок не помещается в память
    EXPECT_EQ (heapAlloc (3), nullptr);
    heapFree (mass);

    // Мусор в куче
    heapInit (mass, (memStructSize * 2) + 1);
    memset (mass + memStructSize, 255, (memStructSize + 1));
    // |начальный блок|-|блок1|-|1 слов|
    EXPECT_EQ (heapAlloc (1), mass + (memStructSize * 2));
    // Блок не полностью помещается в память
    EXPECT_EQ (heapAlloc (3), nullptr);
    heapFree (mass);
}

/**
 * @brief Тест очистки участков памяти
 */
TEST (Heap8BitTest, HeapFreeTest)
{
    // Массив для кучи
    uint8_t mass [100];
    memset (mass, 255, sizeof (mass));

    // Проверка на входные параметры
    heapFree (nullptr);
    // Тест неверных указателей очистки инициализированной кучи
    heapInit (mass + 1, 40);
    heapFree (mass);
    addr_compare (mass + 1, mass + 1);
    heapFree (nullptr);

    // Очистка по указателю на второй элемент 
    heapFree (mass + 2);
    addr_compare (mass + 1, mass + 1);
    
    uint8_t* block = heapAlloc (5);
    EXPECT_NE (block, nullptr);
    *(block - memStructSize) = 20;
    heapFree (block);
    *(block - memStructSize) = 0;
    heapFree (block);
    // Удаление только указателя на кучу
    heapFree (mass + 1);

    // Проверка, что инициализация запустилась заново
    heapInit (mass, 25);

    addr_compare (mass, mass);
    addr_compare (mass + 8, mass + memStructSize);
    EXPECT_EQ (mass[16], 25-memStructSize);

    // Проверка на маловероятное событие  равенства размера структуре
    mass [1] += sizeof (uint64_t);
    heapFree (mass);
    EXPECT_NE (mass[0], 0);
    EXPECT_NE (mass[1], 0);
    EXPECT_NE (mass[2], 0);
    memset (mass, 0, sizeof (mass));
}
