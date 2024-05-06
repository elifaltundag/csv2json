#pragma once
#include "Parameters.h"

// In progress
int jsonWriter(Parameters* pParams);
bool isEntryNull(Parameters* pParams);

// Done
void printJSON(FILE* pfOutput);
int writeArrOfArrs(Parameters* pParams);
int writeArrOfObjs(Parameters* pParams);

// To be written
void generateJsonContents(Parameters* pParams);