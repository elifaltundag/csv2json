#ifndef INTARRAY_H
#define INTARRAY_H

typedef struct {
	int* pData;
	size_t capacity;
	size_t size;
} IntArr;

void createIntArr(IntArr* pArr, size_t capacity);
void destroyIntArr(IntArr* pArr);
void addNewInt(IntArr* pArr, int newInt);

#endif