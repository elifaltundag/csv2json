#pragma once
#include "Parameters.h"
#include "CharArr.h"
#include "IntArr.h"

// Written
void determineNumCols(Parameters* pParams);
int bufferHeaders(FILE* pfInput, CharArr* pArrBufferHeader);
void countParameters(Parameters* pParams);
int eachLineHasSameNumCols(Parameters* pParams);

// Will be improved for edge cases
int parseCSV(Parameters* pParams);
int validateData(Parameters* pParams);