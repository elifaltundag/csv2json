#pragma once
#include "Parameters.h"
#include "CharArr.h"
#include "IntArr.h"

// Written
// void determineNumCols(FILE* pfInput, char* pDelimiter, int* pNumCols);
void determineNumCols(Parameters* pParams);


int bufferHeaders(FILE* pfInput, CharArr* pArrBufferHeader);


// void countLines(FILE* pfInput, char* pDelimiter, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders);
void countLines(Parameters* pParams);


// int eachLineHasSameNumCols(FILE* pfInput, char* pDelimiter, const int* pNumCols);
int eachLineHasSameNumCols(Parameters* pParams);


// Will be improved for edge cases

// int parseCSV(FILE* pfInput, char* pDelimiter, int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders, char*** pppHeaderList);
int parseCSV(Parameters* pParams);


// int validateData(FILE* pfInput, char* pDelimiter, const int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders);
int validateData(Parameters* pParams);