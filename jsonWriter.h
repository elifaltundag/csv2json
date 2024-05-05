#pragma once

// In progress
int jsonWriter(FILE* pfOutput, FILE* pfInput, char* pDelimiter, int* pNumCols, bool* pHasHeaders, char*** pppHeaderList);
bool isEntryNull(FILE* pfOutput, FILE* pfInput, char* pDelimiter);

// Done
void printJSON(FILE* pfOutput);
int writeArrOfArrs(FILE* pfOutput, FILE* pfInput, char* pDelimiter);
int writeArrOfObjs(FILE* pfOutput, FILE* pfInput, char* pDelimiter, int* pNumCols, char*** pppHeaderList);

// To be written 
