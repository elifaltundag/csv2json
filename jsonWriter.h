#pragma once

// In progress
int jsonWriter(FILE* pfOutput, FILE* pfInput, char* pDelimiter, int* pNumCols, bool* pHasHeaders, char*** pppHeaderList);

// Done
void printJSON(FILE* pfOutput);
int writeArrOfArrs(FILE* pfOutput, FILE* pfInput, char* pDelimiter);

// To be written 
int writeArrOfObjs(FILE* pfOutput, FILE* pfInput, char* pDelimiter, int* pNumCols, char*** pppHeaderList);
