#include <stdio.h>
#include <stdbool.h>

#ifndef PARAMETERS_H
#define PARAMETERS_H 

typedef struct {
	FILE* pfInput;
	FILE* pfOutput;
	char delimiter; // single character options: ',' ';' '|' ':' '\t'
	bool hasHeaders;
	int numCols;
	int numValueLines;
	int numCommentLines;
	size_t numChars;
	char* csvContents;
	char** ppHeaderList;
} Parameters;

void initializeParameters(Parameters* pParams);

#endif // !PARAMETERS_H
