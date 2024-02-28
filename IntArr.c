#include <stdio.h>
#include <stdlib.h>

#include "IntArr.h"

void createIntArr(IntArr* pArr, size_t capacity)
{
	pArr->pData = (int*)malloc(capacity * sizeof(int));
	if (!pArr->pData) {
		printf("ERROR: Memory allocation failed!\n");
	}
	
	pArr->capacity = capacity;
	pArr->size = 0;
}

void destroyIntArr(IntArr* pArr)
{
	if (!pArr->pData) return;

	pArr->capacity = 0;
	pArr->size = 0;
	free(pArr->pData);
	pArr->pData = NULL;
}

void addNewInt(IntArr* pArr, int newInt)
{
	if (pArr->size >= pArr->capacity)
	{
		printf("ERROR: Integer array full capacity!\n");
		return;
	}

	pArr->pData[pArr->size] = newInt;
	pArr->size++;
}
