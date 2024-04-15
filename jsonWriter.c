#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "definitions.h"

#include "jsonWriter.h"

int jsonWriter(FILE* pfOutput, FILE* pfInput, char* pDelimiter, int* pNumCols, bool* pHasHeaders, char*** pppHeaderList) {
	if (pfInput == NULL) return JSON_WRITING_ERROR;
	if (pfOutput == NULL) return JSON_WRITING_ERROR;

	const char startJSON[] = "[";
	const char endJSON[]   = "\n]";   
	
	fprintf(pfOutput, "%s", startJSON);
	
	if (*pHasHeaders) writeArrOfObjs(pfOutput, pfInput, pDelimiter, pNumCols, pppHeaderList);
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

// -----------------------------------------------------------------
// Example CSV input
// John, 25, New York
// Alice, 30, Los Angeles
// Bob, 22, Chicago
// Eve, 35, San Francisco
//
// Expected JSON output: array of arrays
// [
//		["John", "25", "New York"],
//		["Alice", "30", "Los Angeles"],
//		["Bob", "22", "Chicago"],
//		["Eve", "35", "San Francisco"]	
// ]
// 
// WRITE:
// [\n
// John, 25, New York
// ^ isNewLine start value: true --> !isNewLine, write ["J
//     ^ : delimiter --> write ", "
//					 ^: \n --> isNewLine, write "],
// 
// Eve, 35, San Francisco
//						 ^ EOF --> write "]\n]
//
// -----------------------------------------------------------------

int writeArrOfArrs(FILE* pfOutput, FILE* pfInput, char* pDelimiter)
{
	const char arrOpening[] = "\n\t[\""; // tab + ["
	const char arrClosing[] = "\"]";   // "] 
	const char newElm[] = "\", \"";    // ", "
	
	rewind(pfInput);

	bool isNewLine = true;
	int cur;
	int numTabs = 1;

	while ((cur = getc(pfInput)) != EOF) {
		char curChar = (char) cur;

		// if isNewLine: open array, switch isNewLine 
		// if \n: close array, switch isNewLine
		// if delimiter: add that

		if (curChar == *pDelimiter) {
			fprintf(pfOutput, "%s", newElm);
			continue;
		}

		if (isNewLine) {
			fprintf(pfOutput, "%s", arrOpening);
			isNewLine = false;
		}

		if (curChar == '\n') {
			fprintf(pfOutput, "%s,", arrClosing);
			isNewLine = true;
			continue;
		}

		fprintf(pfOutput, "%c", curChar);
	}
	
	fprintf(pfOutput, "%s", arrClosing);


	return SUCCESS;
}


// -----------------------------------------------------------------
// // Example CSV input
// Name,Age,Location
// John, 25, New York
// Alice, 30, Los Angeles
// Bob, 22, Chicago
// Eve, 35, San Francisco
// 
// Expected JSON output: array of objects
// [
//		{
//			"Name": "John",
//			"Age": "25",
//			"Location": "New York" 
//		},
//		{
//			"Name": "Alice",
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
// Name,Age,Location
//					^ first \n: skip till here - open new object, write header[0] in quotes, colon : and open new quotes
// 
// John, 25, New York
// ^^^^ get each character
// 
// John, 25, New York
//	   ^ delimiter: close quotes, get to new entry, write header[1] in quotes, colon : and open new quotes
// 
// John, 25, New York
//					 ^ newLine: close quotes, close object, reset iHeader = 0, open new object, write header[0] in quotes, colon : and open new quotes
//  
// 
// Alice, 30, Los Angeles
// Bob, 22, Chicago
// Eve, 35, San Francisco
//						 ^ EOF: close quotes, close object close arr
// 
// WRITE
// [
// \n	\t{
// \n		\t\t\"Name\": \"John\",
// \n		\t\t\"Age\": \"25\",
// \n		\t\t\"Location\": \"New York\"
// \n	\t},
// \n	\t{
// \n		\t\t\"Name\": \"Alice\",
// \n		\t\t\"Age\": \"30\",
// \n		\t\t\"Location\": \"Los Angeles\"
// \n	\t},
// \n	\t{
// \n		\t\t\"Name\": \"Bob\",
// \n		\t\t\"Age\": \"22\",
// \n		\t\t\"Location\": \"Chicago"
// \n	\t},
// \n   \t{
// \n	\t\t\"Name\": \"Eve\",
// \n	\t\t\"Age\": \"35\",
// \n	\t\t\"Location\": \"San Francisco\"
// \n	\t}
// \n] 
// -----------------------------------------------------------------

int writeArrOfObjs(FILE* pfOutput, FILE* pfInput, char* pDelimiter, int* pNumCols, char*** pppHeaderList)
{
	const char objOpening[] = "\n\t{\n"; 
	const char objClosing[] = "\n\t}"; 

	const char quotes[] = "\"";

	bool isFirstLine = true; 
	int cur; 
	int iHeader = 0;

	while ((cur = getc(pfInput)) != EOF) {
		char curChar = (char) cur;
		
		if (curChar == '\n') {
			if (*pNumCols = iHeader) {
				fprintf(pfOutput, "%s", quotes);
			}

			if (!isFirstLine) {
				fprintf(pfOutput, "%s,", objClosing);
			} 
			else {
				isFirstLine = false;
			}
			
			fprintf(pfOutput, "%s", objOpening);

			// Write first header
			iHeader = 0;
			fprintf(pfOutput, "\t\t%s%s%s: %s", quotes, (*pppHeaderList)[iHeader], quotes, quotes);
			iHeader++;
 	
			continue;
		}

		// Skip the first line
		if (isFirstLine) continue;

		if (curChar == *pDelimiter) {
			fprintf(pfOutput, "%s,\n\t\t%s%s%s: %s", quotes, quotes, (*pppHeaderList)[iHeader], quotes, quotes);
			iHeader++;
			continue;
		}

		fprintf(pfOutput, "%c", curChar);
	}

	fprintf(pfOutput, "%s%s", quotes, objClosing);

	rewind(pfInput);

	return SUCCESS;
}

