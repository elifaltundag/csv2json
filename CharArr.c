#include <stdio.h>
#include <stdlib.h>

#include "CharArr.h"

void createCharArr(CharArr* pArr, size_t capacity)
{
	pArr->pData = (char*)malloc(capacity * sizeof(char));
	if (!pArr->pData) {
		printf("ERROR: Memory allocation failed!\n");
	}
	
	pArr->capacity = capacity;
	pArr->size = 0;
}

void destroyCharArr(CharArr* pArr)
{
	if (!pArr->pData) return;

	pArr->capacity = 0;
	pArr->size = 0;
	free(pArr->pData);
	pArr->pData = NULL;
}