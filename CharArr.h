#ifndef CHARARR_H
#define CHARARR_H

typedef struct {
	char* pData;
	size_t capacity;
	size_t size;
} CharArr;

void createCharArr(CharArr* pArr, size_t capacity);
void destroyCharArr(CharArr* pArr);



#endif // !CCHARARR_H