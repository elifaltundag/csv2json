#pragma once
#include "CharArr.h"
#include "IntArr.h"


int parseCSV(FILE* pfInput, char* pDelimiter, int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders, CharArr* pHeaderList);
int validateData(FILE* pfInput, char* pDelimiter);
void checkHeaders(FILE* pfInput, bool* pHasHeaders);
void determineNumCols(FILE* pfInput, char* pDelimiter, int* pNumCols);
void getToFirstNonCommentLine(FILE* pfInput);
void setNumHeaderLetters(IntArr* pNumHeaderLetters, FILE* pfInput, char* pDelimiter);
int getNumHeaderLetters(IntArr* pNumHeaderLetters, int* pNumCols);
void setHeaders(FILE* pfInput, CharArr* pHeaderList, IntArr* pNumHeaderLetters);
int bufferHeader(FILE* pfInput, CharArr* pArrBufferHeader);