#include "../include/heap.h"

#include "string.h"

/**
 * @brief Структура служебной информации блока данных
 */
typedef struct memIntro_s
{
	uint8_t niddle;				/*<! Иголка - значение ключ для поиска служебной структуры	*/
	uint16_t sizeOfBlock;		/*<! Размер блока данных (без учёта служебной структуры)	*/	
	uint8_t* startOfBlock;		/*<! Указатель на начало блока пользовательских данных		*/
}memIntro_t;

/**
 * @brief Указатель на начало кучи
 */
static uint8_t* heapPtr;

/**
 * @brief Размер кучи
 */
static uint16_t heapSize;

/**
 * @brief Значение иголки
 */
static const uint8_t nidl = 0x77;

/**
 * @brief Инициализация кучи
 * @param [in] heap Указатель на начало кучи
 * @param [in] size размер кучи
 */
void heapInit (void* heap, uint16_t size)
{
	memIntro_t initBlock = {
		.niddle = nidl,
		.sizeOfBlock = 0,
		.startOfBlock = (uint8_t*)heap + sizeof (memIntro_t)
	};
	heapPtr = (uint8_t*)heap;
	heapSize = size;
	memset(heapPtr, 0, size);
	memcpy(heapPtr, &initBlock, sizeof (initBlock));	
}

/**
 * @brief Выделение памяти в куче
 * @param [in] size Размер выделяемой памяти 
 * @return Указатель на выделенную память (NULL если выделить память не удалось)
 */
uint8_t* alloc (uint16_t size)
{
	memIntro_t* mem = NULL;
	uint8_t* ptr = heapPtr;	
	// Поиск последнего блока
	// TODO  добавить определение расстояния между блоками данных
	for (uint16_t i = 0; i < (heapSize - 4); i++)
	{
		// Обнаружена "иголка"
		if (ptr[i] == nidl)
		{
			// Разбираемся, то ли мы нашли
			memIntro_t* memPtr = (memIntro_t*)&ptr[i];
			// Элемент startOfBlock должен быть равен началу сегмента
			if (memPtr->startOfBlock == (ptr + i + sizeof (memIntro_t)))
				mem = (memIntro_t*)(ptr+i);	// Сохраняем
		}
	}
	// Не нашли никакой иголки
	if (mem == NULL)
		return NULL;
	// Сохраняем расположение нового блока памяти
	ptr =  mem->startOfBlock + mem->sizeOfBlock;
	if (ptr + sizeof (memIntro_t) + size < heapPtr + heapSize)
	{
		// Записываем данные
		memIntro_t initBlock = {
		.niddle = nidl,
		.sizeOfBlock = size,
		.startOfBlock = ptr + sizeof (memIntro_t),
		};
		memcpy (ptr, &initBlock, sizeof (memIntro_t));
		// Подчищаем
		memset (initBlock.startOfBlock, 0, size);
		return initBlock.startOfBlock;
	}
	return NULL;	
}

/**
 * @brief Освобождение выделенной памяти
 * @param [in] mem Указатель на начало выделенной памяти
 */
void free (uint8_t* mem)
{
	if (mem < (heapPtr + sizeof (memIntro_t)) && mem > heapPtr + heapSize)
		return;
	memIntro_t* infoBlock = (memIntro_t*)(mem - sizeof (memIntro_t));
	if (infoBlock->niddle == nidl && infoBlock->startOfBlock == mem)
	{
		memset(infoBlock, 0, sizeof (memIntro_t));
	}
}



