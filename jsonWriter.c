#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "definitions.h"
#include "Parameters.h"

#include "jsonWriter.h"

int jsonWriter(Parameters* pParams) {
	if (pParams->csvContents == NULL) return JSON_WRITING_ERROR;
	if (pParams->pfOutput == NULL) return JSON_WRITING_ERROR;

	generateJsonContents(pParams);
	

#if 0 
	const char startJSON[] = "[";
	const char endJSON[]   = "\n]";   
	
	fprintf(pParams->pfOutput, "%s", startJSON);
	
	if (pParams->hasHeaders) writeArrOfObjs(pParams);
	else writeArrOfArrs(pParams);

	fprintf(pParams->pfOutput, "%s", endJSON);
	
	printJSON(pParams->pfOutput);
#endif
	return SUCCESS;
}


void generateJsonContents(Parameters* pParams)
{
	// Allocate memory
	size_t numCharsDbl = 2 * pParams->numChars;
	pParams->jsonContents = (char*)malloc(numCharsDbl * sizeof(char));
	if (pParams->jsonContents == NULL) return JSON_WRITING_ERROR;

	// Add null terminated strings to jsonContents by concatenation
	pParams->jsonContents[0] = '\0'; // must begin with null termination
	strcat_s(pParams->jsonContents, numCharsDbl, "["); // start JSON




	strcat_s(pParams->jsonContents, numCharsDbl, "\n]"); // end JSOn

#if 0
	// string concatenation 
	size_t sStr1 = 10 * sizeof(char);
	char* str1 = (char*)malloc(sStr1);
	str1[0] = '\0'; 
	char* str2 = "456";
	strcat_s(str1, sStr1, str2);
#endif
}


// -----------------------------------------------------------------
// Example CSV input
// John,25,New York
// Alice,30,Los Angeles
// Bob,22,Chicago
// Eve,35,San Francisco
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
// John,25,New York
// ^ isNewLine start value: true --> !isNewLine, write ["J
// 
// John,25,New York
//     ^ : delimiter --> write ", "
// 
// John,25,New York
//				   ^: \n --> isNewLine, write "],
// 
// Eve,35,San Francisco
//					   ^ EOF --> write "]\n]
//
// -----------------------------------------------------------------

int writeArrOfArrs(Parameters* pParams)
{
	const char arrOpening[] = "\n\t[\""; // tab + ["
	const char arrClosing[] = "\"]";   // "] 
	const char newElm[] = "\", \"";    // ", "
	
	rewind(pParams->pfInput);

	bool isNewLine = true;
	int cur;
	int numTabs = 1;

	while ((cur = getc(pParams->pfInput)) != EOF) {
		char curChar = (char) cur;

		// if isNewLine: open array, switch isNewLine 
		// if \n: close array, switch isNewLine
		// if delimiter: add that

		if (curChar == pParams->delimiter) {
			fprintf(pParams->pfOutput, "%s", newElm);
			continue;
		}

		if (isNewLine) {
			fprintf(pParams->pfOutput, "%s", arrOpening);
			isNewLine = false;
		}

		if (curChar == '\n') {
			fprintf(pParams->pfOutput, "%s,", arrClosing);
			isNewLine = true;
			continue;
		}

		fprintf(pParams->pfOutput, "%c", curChar);
	}
	
	fprintf(pParams->pfOutput, "%s", arrClosing);


	return SUCCESS;
}


// -----------------------------------------------------------------
// Example CSV input
// Name,Age,Location
// John,25,New York
// Alice,30,Los Angeles
// Bob,22,Chicago
// Eve,35,San Francisco
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
// John,25,New York
// ^^^^ get each character
// 
// John,25,New York
//	   ^ delimiter: close quotes, get to new entry, write header[1] in quotes, colon : and open new quotes
// 
// John,25,New York
//				   ^ newLine: close quotes, close object, reset iHeader = 0, open new object, write header[0] in quotes, colon : and open new quotes
//  
// 
// Alice,30,Los Angeles
// Bob,22,Chicago
// Eve,35,San Francisco
//					   ^ EOF: close quotes, close object close arr
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

int writeArrOfObjs(Parameters* pParams)
{
	const char objOpening[] = "\n\t{\n"; 
	const char objClosing[] = "\n\t}"; 

	const char quotes[] = "\"";

	bool isFirstLine = true; 
	int cur; 
	int iHeader = 0;

	while ((cur = getc(pParams->pfInput)) != EOF) {
		char curChar = (char) cur;
		
		if (curChar == '\n') {
			if (pParams->numCols = iHeader) {
				fprintf(pParams->pfOutput, "%s", quotes);
			}

			if (!isFirstLine) {
				fprintf(pParams->pfOutput, "%s,", objClosing);
			} 
			else {
				isFirstLine = false;
			}
			
			fprintf(pParams->pfOutput, "%s", objOpening);

			// Write first header
			iHeader = 0;
			fprintf(pParams->pfOutput, "\t\t%s%s%s: %s", quotes, (pParams->ppHeaderList)[iHeader], quotes, quotes);
			iHeader++;
 	
			continue;
		}

		// Skip the first line
		if (isFirstLine) continue;

		if (curChar == pParams->delimiter) {
			fprintf(pParams->pfOutput, "%s,\n\t\t%s%s%s: %s", quotes, quotes, (pParams->ppHeaderList)[iHeader], quotes, quotes);
			iHeader++;
			continue;
		}

		

		fprintf(pParams->pfOutput, "%c", curChar);
	}

	fprintf(pParams->pfOutput, "%s%s", quotes, objClosing);

	rewind(pParams->pfInput);

	return SUCCESS;
}





// -----------------------------------------------------------------
// IRREGULAR COLUMNS
// 
// Example CSV input
// Name,Age,Location
// John,25,
//		   ^^ : delimiter + new line
// 
// Alice,,Los Angeles
//		^^ delimiter + delimiter
//		
// 
// Expected JSON output: null where necessary
// 
// [
//		{
//			"Name": "John",
//			"Age" : "25",
//			"Location" : ""
//		},
//		{
//			"Name": "Alice",
//			"Age" : "",
//			"Location" : "Los Angeles"
//		}
// ]
// -----------------------------------------------------------------


// if a new line or delimiter is followed by a delimiter, it is null 
// bool isEntryNull(FILE* pfOutput, FILE* pfInput, char* pDelimiter)
bool isEntryNull(Parameters* pParams)
{
	bool isNull = false;



	return isNull;
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