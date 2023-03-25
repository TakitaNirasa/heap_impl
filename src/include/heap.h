/**
 * @file heap.h
 * @author Никита Тарасов (anikitatarasov@gmail.com)
 * @brief Библиотека динамического выделения памяти
 * usage:
 * 
 *      #define block_size_t uintXX_t
 *      #include "heap.h"
 * 
 *      uintXX_t memory [1000];
 *      heapInit (memory, 1000);
 *      uintXX_t* mass = heapAlloc (100);
 *      ....
 *      heapFree (mass);
 * 
 */
#pragma once
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
 * @param [in] size Размер кучи в единицах разрядности платформы
 */
void heapInit (block_size_t* heap, block_size_t size);

/**
 * @brief Выделение памяти в куче
 * 
 * @param [in] size Размер выделяемой памяти 
 * @return Указатель на выделенную память (nullptr если выделить память не удалось)
 */
block_size_t* heapAlloc (block_size_t size);

/**
 * @brief Освобождение выделенной памяти
 * @note Так же работает для освобождения всей кучи при передаче 
 * начального адреса кучи
 * 
 * @param [in] mem Указатель на начало выделенной памяти
 */
void heapFree (block_size_t* mem);
