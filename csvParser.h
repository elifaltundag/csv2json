#pragma once
#include "CharArr.h"
#include "IntArr.h"

// Written
void determineNumCols(FILE* pfInput, char* pDelimiter, int* pNumCols);
int bufferHeaders(FILE* pfInput, CharArr* pArrBufferHeader);
void countLines(FILE* pfInput, char* pDelimiter, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders);
int eachLineHasSameNumCols(FILE* pfInput, char* pDelimiter, const int* pNumCols);

// Will be improved for edge cases
int parseCSV(FILE* pfInput, char* pDelimiter, int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders, char*** pppHeaderList);
int validateData(FILE* pfInput, char* pDelimiter, const int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders);