#include "../include/heap.h"

#include "string.h"
#include "stdio.h"

/**
 * @brief Структура служебной информации блока данных
 */
#pragma pack (push, 1)
typedef struct mem_s
{
    block_size_t* memBlock;		/*<! Указатель на данную структуру		*/
    block_size_t* next;			/*<! Указатель на следующую структуру	*/
    block_size_t size;			/*<! Размер выделяемых данных			*/
}mem_t;
#pragma pack (pop)

/**
 * @brief Размер структуры начала блока данных в единицах сдвига указателя (словах)
 * @note Высчитывается с учётом архитектуры
 * 16 байт на 64битной архитектуре будет 2 слова
 * 18 байт на 64битной архитектуре будет 3 слова
 */
const block_size_t memStructSize = (sizeof (mem_t) / sizeof (block_size_t)) + 
									(sizeof (mem_t) & (sizeof (block_size_t) - 1) ? 1 : 0);

/**
 * @brief Указатель на начало кучи
 */
static block_size_t* heapPtr = NULL;

/**
 * @brief Функция добавления блока в память
 * @param ptr Указатель на начало выделяемой памяти
 * @param size Размер требуемой к выделению памяти
 * @return Указатель на память, доступную для пользователя
 */
static block_size_t* putBlock (void* ptr, block_size_t size)
{
	mem_t block = {
        .memBlock = (block_size_t*)ptr,
        .next = (block_size_t*)ptr + memStructSize + size,
        .size = size
    };
	
	memset(ptr, 0, size);
	memcpy(ptr, &block, sizeof (mem_t));
	return (block_size_t*)ptr + memStructSize;
}

void heapInit (block_size_t* heap, block_size_t size)
{
	// Проверка на уже инициализированный блок
	if (heapPtr != NULL)
		return;
	// Проверка входных параметров
	if (heap == NULL || size <= memStructSize)
		return;
	// Блок начала разметки памяти
	mem_t intro = {
        .memBlock = (block_size_t*)heap,
        .next = (block_size_t*)heap + memStructSize,
        .size = size - memStructSize
    };
	heapPtr = (block_size_t*)heap;
	memset(heapPtr, 0, size * sizeof (block_size_t));
	memcpy(heapPtr, &intro, sizeof (mem_t));	
}

block_size_t* heapAlloc (block_size_t size)
{
	if (heapPtr == NULL || size == 0)
		return NULL;
	// Указатель на первый блок памяти
	const mem_t* mem = (mem_t*)heapPtr;

	// Если начальный указатель не инициализирован и указателя на следующий блок нет
	if ((mem_t*)mem->memBlock != mem || mem->next != mem->memBlock + memStructSize)
		return NULL;

	// Полный размер памяти
	block_size_t heapSize = mem->size;

	if (size > heapSize)
		return NULL;
	// Указатель на конец кучи
	const block_size_t* endPtr = (block_size_t*)(mem->next) + heapSize;

	for (mem_t* blockPtr = (mem_t*)mem->next; blockPtr < (mem_t*)endPtr;)
	{
		// Если следующий элемент существует
		if (blockPtr->memBlock != NULL && blockPtr->memBlock == (block_size_t*)blockPtr)
			blockPtr = (mem_t*)(blockPtr->next);
		else
		{
			// Проход по элементам в поиске следующего (если места хватает, то выделение памяти)
			block_size_t* ptr = (block_size_t*)blockPtr;

			for (;ptr < endPtr; ptr++)
			{
				// Если хватает места для помещения блока памяти со структурой блока	
				if ((block_size_t)(ptr - (block_size_t*)blockPtr) >= size + memStructSize)
					return putBlock (blockPtr, size);
				// Найден указатель на следующий элемент
				else if (((mem_t*)ptr)->memBlock == ptr)
				{
					blockPtr = (mem_t*)ptr;
					break;
				}
			}
			if (ptr >= endPtr - 1)
				return NULL;
		}
	}
	return NULL;	
}

void heapFree (block_size_t* mem)
{
	if (heapPtr == NULL || mem == NULL)// || mem < heapPtr + (memStructSize * 2))
		return;

	mem_t* infoBlock;
	if (heapPtr == mem)
	{
		infoBlock = (mem_t*)mem;
		heapPtr = NULL;
	}
	else
		infoBlock = (mem_t*)(mem-memStructSize);
	
	// Если блок существует и правильно инициализирован
	if (infoBlock->memBlock == NULL ||  (infoBlock->memBlock != NULL && infoBlock->memBlock != (block_size_t*)infoBlock))
		return;

	// Проверка на указатели
	block_size_t size_p = infoBlock->next - infoBlock->memBlock;
	block_size_t size_n = infoBlock->size + memStructSize;

	// Очистка выбранного блока или деинициализация всей кучи
	if (size_p == size_n || (size_p == memStructSize && size_n > memStructSize))
		memset(infoBlock, 0, size_n * sizeof (block_size_t));
}
