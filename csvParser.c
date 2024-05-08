#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "definitions.h"

#include "csvParser.h"
#include "Parameters.h"
#include "CharArr.h"
#include "IntArr.h"

int parseCSV(Parameters* pParams)
{
	// Save CSV file contents in a string and close input file
	if (bufferCSVContents(pParams) != SUCCESS) return CSV_PARSING_ERROR;
	
	if (pParams->pfInput != NULL) {
		fclose(pParams->pfInput);
		pParams->pfInput = NULL;
	}

	determineNumCols(pParams);
	determineNumLines(pParams);

	if (validateData(pParams) != SUCCESS) return CSV_PARSING_ERROR;
	
	if (pParams->hasHeaders) {
		if (bufferHeaders(pParams) != SUCCESS) return CSV_PARSING_ERROR;

		// Allocate memory for each headers address
		pParams->ppHeaderList = (char**)malloc(pParams->numCols * sizeof(char*));
		if (!pParams->ppHeaderList) return CSV_PARSING_ERROR;

		int iHeader = 0;
		char* pDelimiter = &pParams->delimiter;
		char* token = NULL;
		char* next_token = NULL; // WHY?? 
			
		if (pParams->headers != NULL) {
			token = strtok_s(pParams->headers, pDelimiter, &next_token);
			while (token != NULL) {
				(pParams->ppHeaderList)[iHeader] = token;
				token = strtok_s(NULL, pDelimiter, &next_token);
				iHeader++;
			}
		}
	}
	return SUCCESS;
}

int bufferCSVContents(Parameters* pParams)
{
	if (!pParams->pfInput) {
		printf("ERROR: Cannot open CSV file\n");
		return CSV_PARSING_ERROR;
	}
	rewind(pParams->pfInput);

	// Count total number of characters in the CSV file
	int cur, numChars = 0;
	while ((cur = getc(pParams->pfInput)) != EOF) numChars++;

	pParams->numChars = numChars;

	// Allocate memory
	pParams->csvContents = (char*)malloc((numChars + 1) * sizeof(char)); // +1 for null termination
	if (!pParams->csvContents) {
		printf("Cannot allocate enough memory to buffer CSV contents\n");
		return CSV_PARSING_ERROR;
	}

	// Buffer CSV contents 
	rewind(pParams->pfInput);
	size_t numCharsRead = fread(pParams->csvContents, numChars, 1, pParams->pfInput);
	pParams->csvContents[numChars] = '\0';


	fclose(pParams->pfInput);
	pParams->pfInput = NULL;


	return SUCCESS;
}

void determineNumCols(Parameters* pParams) {
	int numDelimiters = 0;
	int i = 0;
	char delimiter = pParams->delimiter;

	while (i < pParams->numChars)
	{
		char curChar = pParams->csvContents[i];
		i++;

		if (curChar == '\n') break;
		if (curChar == delimiter) numDelimiters++;
	}

	pParams->numCols = numDelimiters + 1;
};

void determineNumLines(Parameters* pParams)
{
	int numValueLines = 1; 
	int numCommentLines = 0;
	int i = 0;
	bool isPrevCharSlash = false;
	bool isNewLine = true;

	while (i < pParams->numChars) 
	{
		char curChar = pParams->csvContents[i];
		i++;

		// Count new lines
		if (curChar == '\n') {
			numValueLines++;
			isNewLine = true;
		}

		if (isNewLine && curChar != '\n') {
			isNewLine = false;
		}

		// Count comment lines
		// Comment line specifiers: '#', '//'  
		else if (isNewLine && curChar == '#')
		{
			numValueLines--;
			numCommentLines++;
		}
		else if (isPrevCharSlash && curChar == '/')
		{
			numValueLines--;
			numCommentLines++;
		}
		else if (isNewLine && !isPrevCharSlash && curChar == '/')
		{
			isPrevCharSlash = !isPrevCharSlash;
		}

	}

	if (pParams->hasHeaders) numValueLines--;
	pParams->numValueLines = numValueLines;
	pParams->numCommentLines = numCommentLines;
}

int validateData(Parameters* pParams)
{
	int invalidLine = eachLineHasSameNumCols(pParams);

	if (invalidLine != SUCCESS) {
		printf("ERROR! Invalid number of entries at line %d\n", invalidLine);
		return CSV_PARSING_ERROR;
	}

	// -----------------------------------------------------------
	// TODO
	// Get each data type: String || Number
	// Do all entries match these data types or null?
	// -----------------------------------------------------------

	return SUCCESS;
}


// If each non-comment line has the same number of entries (columns) as pNumCol returns 0 (SUCCESS)
// Else returns the first invalid line's order
int eachLineHasSameNumCols(Parameters* pParams)
{
	int numLineDelimiters = 0;
	int curLine = 1;
	int i = 0;
	char delimiter = pParams->delimiter;

	while (i < pParams->numChars) 
	{
		char curChar = pParams->csvContents[i];
		i++;
		
		if (curChar == pParams->delimiter) numLineDelimiters++;
		else if (curChar == '\n') {
			if (numLineDelimiters + 1 < pParams->numCols) return curLine;
			curLine++;
			numLineDelimiters = 0;
		}

		if (numLineDelimiters >= pParams->numCols) return curLine;


		// TODO: ignore and skip comment lines
		/*
		else if (curChar == '#') continue;
		else if (isPrevCharSlash && curChar == '/') continue;
		else if (!isPrevCharSlash && curChar == '/') isPrevCharSlash = !isPrevCharSlash;
		*/
	}

	return SUCCESS;
}


int bufferHeaders(Parameters* pParams)
{
	// Get number of characters of the first line of CSV file as size of buffer (char array)
	size_t numCharsHeader = 0;
	int i = 0;

	while (i < pParams->numChars) {
		char curChar = (pParams->csvContents)[i];
		i++;

		if (curChar == '\n') break;
		numCharsHeader++;
	}
 
	// Buffer headers as a whole string
	pParams->headers = (char*)malloc((numCharsHeader + 1) * sizeof(char)); // +1 for null termination
	if (!pParams->headers) return CSV_PARSING_ERROR;

	// Buffer headers 
	i = 0;
	while (i < numCharsHeader) {
		pParams->headers[i] = (pParams->csvContents)[i];
		i++;
	}
	pParams->headers[numCharsHeader] = '\0';

	
	return SUCCESS;
}


