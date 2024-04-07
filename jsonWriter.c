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
// WRITE:
// data = [\n
// John, 25, New York
// ^ isNewLine start value: true --> !isNewLine, write ["J
//     ^ : delimiter --> write ", "
//					 ^: \n --> isNewLine, write "],
// 
// Eve, 35, San Francisco
//						 ^ EOF --> write "]\n]
// 
// 
// -----------------------------------------------------------------
// // Example CSV input
// Name,Age,Location
// John, 25, New York
// Alice, 30, Los Angeles
// Bob, 22, Chicago
// Eve, 35, San Francisco
// 
// Expected JSON output: array of objects
// data = [
//		{
//			"Name": "John" ,
//			"Age": "25" ,
//			"Location": "New York" 
//		},
//		{
//			"Name": "Alice" ,
//			"Age": "30",
//			"Location": "Los Angeles" 
//		},
//		{
//			"Name": "Bob",
//			"Age": "22",
//			"Location": "Chicago"
//		},
//		{
//			"Name": "Eve",
//			"Age": "35",
//			"Location": "San Francisco"
//		}
// ]
// 
// WRITE
// data = [\n
//  
// 
// 
// 
// -----------------------------------------------------------------

int jsonWriter(FILE* pfOutput, FILE* pfInput, char* pDelimiter, bool* pHasHeaders) {
	if (pfInput == NULL) return JSON_WRITING_ERROR;
	if (pfOutput == NULL) return JSON_WRITING_ERROR;

	bool hasHeaders = *pHasHeaders;
	const char startJSON[] = "[\n";
	const char endJSON[]   = "\n]";   
	
	fprintf(pfOutput, "%s", startJSON);
	
	if (hasHeaders) writeArrOfObjs(pfOutput, pfInput, pDelimiter, hasHeaders);
	else writeArrOfArrs(pfOutput, pfInput, pDelimiter);

	fprintf(pfOutput, "%s", endJSON);
	
	printJSON(pfOutput);

	return SUCCESS;
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

int writeArrOfArrs(FILE* pfOutput, FILE* pfInput, char* pDelimiter)
{
	const char arrOpening[] = "[\""; // ["
	const char arrClosing[] = "\"]"; // "] 
	const char newElm[] = "\", \"";  // ", "
	
	rewind(pfInput);

	bool isNewLine = true;
	int cur;
	int numTabs = 1;

	while ((cur = getc(pfInput)) != EOF) {
		char curChar = (char)cur;

		// if isNewLine: open array, switch isNewLine 
		// if \n: close array, switch isNewLine
		// if delimiter: add that

		if (curChar == *pDelimiter) {
			fprintf(pfOutput, "%s", newElm);
			continue;
		}

		if (isNewLine) {
			for (int i = 0; i < numTabs; i++) {
				fprintf(pfOutput, "\t");
			}

			fprintf(pfOutput, "%s", arrOpening);
			isNewLine = false;
		}

		if (curChar == '\n') {
			fprintf(pfOutput, "%s", arrClosing);
			fprintf(pfOutput, ",");
			isNewLine = true;
		}

		fprintf(pfOutput, "%c", curChar);
	}
	
	fprintf(pfOutput, "%s", arrClosing);


	return SUCCESS;
}

int writeArrOfObjs(FILE* pfOutput, FILE* pfInput, char* pDelimiter, const bool hasHeaders)
{
	const char objOpening[] = "{\n"; 
	const char objClosing[] = "\n}";
	const char newElm[]     = ",\n"; 

	rewind(pfInput);

	

	return SUCCESS;
}

