#pragma once

// In progress
int jsonWriter(FILE* pfOutput, FILE* pfInput, char* pDelimiter, bool* pHasHeaders);

// Done
void printJSON(FILE* pfOutput);

// To be written 
int writeCSVcontent(FILE* pfOutput, FILE* pfInput, char* pDelimiter, bool* pHasHeaders);
