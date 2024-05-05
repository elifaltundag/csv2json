#pragma once
#include "Parameters.h"
#include "CharArr.h"
#include "IntArr.h"

// Written
void determineNumCols(Parameters* pParams);
void determineNumLines(Parameters* pParams);
int bufferHeaders(FILE* pfInput, CharArr* pArrBufferHeader);
int eachLineHasSameNumCols(Parameters* pParams);

// Will be improved for edge cases
int parseCSV(Parameters* pParams);
int validateData(Parameters* pParams);

// In progress
int bufferCSVContents(Parameters* pParams);