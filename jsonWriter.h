#pragma once
#include "Parameters.h"

// In progress
int jsonWriter(Parameters* pParams);
bool isEntryNull(Parameters* pParams);

// Done
void printJSON(FILE* pfOutput);
void generateArrOfArrs(Parameters* pParams);
void generateArrOfObjs(Parameters* pParams);

// To be written
int generateJsonContents(Parameters* pParams);