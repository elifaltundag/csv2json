#pragma once

// In progress
int jsonWriter(FILE* pfOutput, FILE* pfInput, char* pDelimiter, bool* pHasHeaders);

// Done
void printJSON(FILE* pfOutput);
int writeArrOfArrs(FILE* pfOutput, FILE* pfInput, char* pDelimiter);

// To be written 
int writeArrOfObjs(FILE* pfOutput, FILE* pfInput, char* pDelimiter, const bool hasHeaders);
