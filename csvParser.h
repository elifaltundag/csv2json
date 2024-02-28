#pragma once
#include "CharArr.h"
#include "IntArr.h"

// Written
int parseCSV(FILE* pfInput, char* pDelimiter, int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders, CharArr* pHeaderList);
void determineNumCols(FILE* pfInput, char* pDelimiter, int* pNumCols);
int bufferHeader(FILE* pfInput, CharArr* pArrBufferHeader);

// Not written yet
void checkHeaders(FILE* pfInput, bool* pHasHeaders);
int validateData(FILE* pfInput, char* pDelimiter);
