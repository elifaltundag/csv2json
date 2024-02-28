#pragma once
#include "CharArr.h"
#include "IntArr.h"

// Written
void determineNumCols(FILE* pfInput, char* pDelimiter, int* pNumCols);
int bufferHeaders(FILE* pfInput, CharArr* pArrBufferHeader);
void countLines(FILE* pfInput, char* pDelimiter, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders);

// Currently working on
int parseCSV(FILE* pfInput, char* pDelimiter, int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders, CharArr* pHeaderList);
int validateData(FILE* pfInput, char* pDelimiter, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders);

// Not written yet
void checkHeaders(FILE* pfInput, bool* pHasHeaders);
void getToFirstNonCommentLine(FILE *pfInput);