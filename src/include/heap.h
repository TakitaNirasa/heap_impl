/**
 * @file heap.h
 * @author Никита Тарасов (nikitatarasov@gmail.com)
 * @brief Библиотека динамического выделения выделения памяти
 * usage:
 *      heapInit (memory, sizeof (memory), sizeof (size_t));
 *      uint8_t* mass = heapAlloc (100);
 *      ....
 *      heapFree (mass);
 * 
 */
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Макрос хранящий размер переменной хранящей размера данных
 */
#ifndef block_size_t
#define block_size_t size_t
#endif

/**
 * @brief Инициализация кучи
 *
 * @param [in] heap Указатель на начало кучи
 * @param [in] size Размер кучи
 */
void heapInit (void* heap, block_size_t size);

/**
 * @brief Выделение памяти в куче
 * 
 * @param [in] size Размер выделяемой памяти 
 * @return Указатель на выделенную память (nullptr если выделить память не удалось)
 */
block_size_t* heapAlloc (block_size_t size);

/**
 * @brief Освобождение выделенной памяти
 * 
 * @param [in] mem Указатель на начало выделенной памяти
 */
void heapFree (block_size_t* mem);
