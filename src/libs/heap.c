#include "../include/heap.h"

#include "string.h"

#pragma pack (push, 1)
/**
 * @brief Структура служебной информации блока данных
 */
typedef struct mem_s
{
    void* this;			/*<! Указатель на данную структуру		*/
    void* next;			/*<! Указатель на следующую структуру	*/
    block_size_t size;	/*<! Размер выделяемых данных			*/
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
static block_size_t* heapPtr;

void heapInit (void* heap, block_size_t size)
{
	// Проверка входных параметров
	if (heap == NULL || size < sizeof (mem_t))
		return;
	// Блок начала разметки памяти
	mem_t intro = {
        .this = heap,
        .next = heap + memStructSize,
        .size = size
    };
	heapPtr = (block_size_t*)heap;
	memset(heapPtr, 0, size * sizeof (block_size_t));
	memcpy(heapPtr, &intro, sizeof (mem_t));	
}

block_size_t* heapAlloc (block_size_t size)
{
	if (heapPtr == NULL || size == 0)
		return NULL;

	const mem_t* mem = (mem_t*)heapPtr;

	// Если начальный указатель не инициализирован и указателя на следующий блок нет
	if (mem->this != mem || mem->next != mem->this + memStructSize)
		return NULL;
	
	block_size_t heapSize = mem->size;

	const block_size_t* endPtr = (block_size_t*)(mem + size);

	for (mem_t* ptr = (mem_t*)mem->next; ptr < (mem_t*)endPtr;)
	{
		// Если следующий элемент существует
		if (ptr->next == ((mem_t*)(ptr->next))->this)
		{
			ptr = ptr->next;
		}
		else
		{

		}

	}
	return NULL;	
}

void heapFree (block_size_t* mem)
{
}
