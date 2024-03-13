#pragma once

// In progress
int jsonWriter(FILE* pfOutput, FILE* pfInput, char* pDelimiter, bool* pHasHeaders);
size_t writeToJSON(FILE* pfOutput, char* text, size_t textSize);


// Done

// To be written 
void printJSON(FILE* pfOutput);