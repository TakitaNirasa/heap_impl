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
#include  <stdint.h>
#include <stddef.h>

/**
 * @brief Инициализация кучи
 *
 * @param [in] heap Указатель на начало кучи
 * @param [in] size Размер кучи
 * @param [in] base Разрядность платформы, байт
 */
void heapInit (void* heap, size_t size, size_t base);

/**
 * @brief Выделение памяти в куче
 * 
 * @param [in] size Размер выделяемой памяти 
 * @return Указатель на выделенную память (nullptr если выделить память не удалось)
 */
uint8_t* heapAlloc (uint16_t size);


/**
 * @brief Освобождение выделенной памяти
 * 
 * @param [in] mem Указатель на начало выделенной памяти
 */
void heapFree (uint8_t* mem);
