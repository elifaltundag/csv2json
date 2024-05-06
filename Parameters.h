#include <stdio.h>
#include <stdbool.h>

#ifndef PARAMETERS_H
#define PARAMETERS_H 

typedef struct {
	FILE* pfInput;
	FILE* pfOutput;
	
	char delimiter; // single character options: ',' ';' '|' ':'
	
	bool hasHeaders;
	char** ppHeaderList;
	
	size_t numChars;
	char* csvContents;

	int numCols;
	int numValueLines;
	int numCommentLines;
} Parameters;

void initializeParameters(Parameters* pParams);

#endif // !PARAMETERS_H
