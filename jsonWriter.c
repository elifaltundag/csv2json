#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "definitions.h"

#include "jsonWriter.h"

// -----------------------------------------------------------------
// Example CSV input
// John, 25, New York
// Alice, 30, Los Angeles
// Bob, 22, Chicago
// Eve, 35, San Francisco
//
// Expected JSON output: array of arrays
// data = [
//		["John", "25", "New York"],
//		["Alice", "30", "Los Angeles"],
//		["Bob", "22", "Chicago"],
//		["Eve", "35", "San Francisco"]	
// ]
// 
// write data = [\n
// John, 25, New York
// ^ isNewLine start value: true --> !isNewLine, write ["J
//     ^ : delimiter --> write ", "
//					 ^: \n --> isNewLine, write "],
// 
// Eve, 35, San Francisco
//						 ^ EOF --> write "]\n]
// 
// -----------------------------------------------------------------

int jsonWriter(FILE* pfOutput, FILE* pfInput, char* pDelimiter, bool* pHasHeaders) {
	if (pfInput == NULL) return JSON_WRITING_ERROR;
	if (pfOutput == NULL) return JSON_WRITING_ERROR;

	char startJSON[] = "data = [\n";
	char endJSON[] = "\n]"; 

	char startElement[] = "[\"";
	char endElement[] = "\"]";
	
	char delimiter[] = "\", \"";
	
	size_t written = writeToJSON(pfOutput, &startJSON, sizeof(startJSON) - 1);
	printJSON(pfOutput);

	return SUCCESS;
}

size_t writeToJSON(FILE* pfOutput, char* text, size_t textSize)
{
	size_t numCharsWritten = fwrite(text, sizeof(char), textSize, pfOutput);
	return numCharsWritten;
}

void printJSON(FILE* pfOutput)
{
	printf("\nCurrent JSON file contents:\n");

	rewind(pfOutput);
	int cur;
	while ((cur = getc(pfOutput)) != EOF)
	{
		char curChar = (char)cur;
		printf("%c", curChar);
	}
}

