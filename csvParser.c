#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "definitions.h"

#include "csvParser.h"
#include "CharArr.h"
#include "IntArr.h"

int parseCSV(FILE* pfInput, char* pDelimiter, int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders, char*** pppHeaderList)
{
	// TODO: create numCols by dereferencing pNumCols	

	determineNumCols(pfInput, pDelimiter, pNumCols);
	
	int dataValidated = validateData(pfInput, pDelimiter, pNumCols, pNumValueLines, pNumCommentLines, pHasHeaders);
	if (dataValidated != SUCCESS)
	{
		return CSV_PARSING_ERROR;
	}

	// -----------------------------------------------------------
	// Save each header if the CSV file has headers
	// -----------------------------------------------------------
	if (*pHasHeaders)
	{
		// Buffer headers ---------------------------------------------
		CharArr allHeaders;
		int allHeadersBuffered = bufferHeaders(pfInput, &allHeaders);

		if (allHeadersBuffered != SUCCESS)
		{
			return CSV_PARSING_ERROR;
		}

		IntArr numHeaderChars;
		createIntArr(&numHeaderChars, *pNumCols);

		if (!numHeaderChars.pData)
		{
			destroyIntArr(&numHeaderChars);
			return CSV_PARSING_ERROR;
		}

		// Get each header separately ---------------------------------
		// 1. Get each title's length: assign it in numHeaderChars
		// 2. Create numCols many char*
		// 3. Each char* has the corresponding size (step 1)
		// 4. Add each char*'s address to *pppHeaderList
		// 5. Destroy numHeaderChars
		// 6. Destroy headers 
		int numChar = 0;
		
		for (size_t i = 0; i < allHeaders.capacity; ++i)
		{
			// ------------------------------------------------------------
			// TODO: Eliminate header quotes
			// ------------------------------------------------------------

			if (allHeaders.pData[i] != *pDelimiter) numChar++;
			else {
				addNewInt(&numHeaderChars, numChar);
				numChar = 0;
			}
		}
		addNewInt(&numHeaderChars, numChar);
	

		// Get each header and add their address to pppHeaderList
		*pppHeaderList = (char**)malloc(*pNumCols * sizeof(char*));
		if (!*pppHeaderList) {
			return CSV_PARSING_ERROR;
		}

		int numAddedHeader = 0;
		int iStart = 0;
		for (int iHeader = 0; iHeader < *pNumCols; iHeader++) {
			int sizeHeader = numHeaderChars.pData[iHeader];
			
			char* header = (char*)malloc((sizeHeader + 1) * sizeof(char));
			if (!header) {
				return CSV_PARSING_ERROR;
			}

			for (int i = 0; i < sizeHeader; i++) {
				header[i] = allHeaders.pData[i + iStart];
			}
			header[sizeHeader] = '\0';

			(*pppHeaderList)[numAddedHeader] = header;
			numAddedHeader++;

			iStart += sizeHeader + 1;
		}

		destroyIntArr(&numHeaderChars);
		destroyCharArr(&allHeaders);
	}
	
	return SUCCESS;
}

int validateData(FILE* pfInput, char* pDelimiter, const int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders)
{
	// -----------------------------------------------------------
	// Incorrect comma count per entry: DONE
	// Count value and comment lines: DONE
	//
	// TODO: later
	// Get each data type: String || Number
	// Do all entries match these data types or null?
	// -----------------------------------------------------------

	countLines(pfInput, pDelimiter, pNumValueLines, pNumCommentLines, pHasHeaders);
	int validNumEntries = eachLineHasSameNumCols(pfInput, pDelimiter, pNumCols);
	if (validNumEntries != SUCCESS) {
		printf("ERROR! Invalid number of entries at line %d\n", validNumEntries);
		return CSV_PARSING_ERROR;
	}


	return SUCCESS;
}

void countLines(FILE * pfInput, char* pDelimiter, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders)
{
	// Comment line specifiers: '//', '#'  
	// if the file has headers go to next line
	
	rewind(pfInput);

	int numValueLines = 1;
	int numCommentLines = 0;
	int cur;
	bool isPrevCharSlash = false;
	bool isNewLine = true;

	while ((cur = getc(pfInput)) != EOF)
	{
		char curChar = (char)cur;
		
		if (curChar == '\n') {
			numValueLines++;
			isNewLine = true;
		}

		if (isNewLine && curChar != '\n') {
			isNewLine = false;
		}
		
		// Count comment lines
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
	*pNumValueLines = numValueLines;
	*pNumCommentLines = numCommentLines;
}

// If each non-comment line has the same number of entries (columns) as pNumCol returns 0 (SUCCESS)
// Else returns the first invalid line's order
int eachLineHasSameNumCols(FILE* pfInput, char* pDelimiter, const int* pNumCols)
{
	rewind(pfInput);

	int cur, numLineDelimiters = 0, curLine = 1;
	bool isPrevCharSlash = 0;

	while ((cur = getc(pfInput)) != EOF) {
		char curChar = (char)cur;
		
		if (curChar == *pDelimiter) numLineDelimiters++;
		
		else if (curChar == '\n') {
			if (numLineDelimiters + 1 < *pNumCols) return curLine;
			curLine++;
			numLineDelimiters = 0;
		}
		
		if (numLineDelimiters >= *pNumCols) return curLine;
		

		// TODO: skip comment lines
		/* 
		else if (curChar == '#') continue;
		else if (isPrevCharSlash && curChar == '/') continue;
		else if (!isPrevCharSlash && curChar == '/') isPrevCharSlash = !isPrevCharSlash;
		*/
	}

	
	
	return SUCCESS;
}

void determineNumCols(FILE* pfInput, char* pDelimiter, int* pNumCols) {
	int numDelimiters = 0;

	int currentChar;
	while ((currentChar = getc(pfInput)) != EOF)
	{
		char strCh = (char)currentChar;

		if (strCh == '\n')
			break;

		if (strCh == *pDelimiter)
			numDelimiters++;
	}

	*pNumCols = numDelimiters + 1;
	rewind(pfInput);
};

int bufferHeaders(FILE* pfInput, CharArr* pArrBufferHeader)
{
	if (!pfInput)
	{
		printf("ERROR: Cannot open CSV file\n");
		return CSV_PARSING_ERROR;
	}
	rewind(pfInput);

	// Get number of characters to create buffer (char array)
	size_t numCharsHeader = 0;
	int ch;

	while ((ch = getc(pfInput)) != EOF)
	{
		char curCh = (char)ch;
		if (curCh == '\n') break;
		if (curCh == '\"') continue;
		numCharsHeader++;
	}
 
	createCharArr(pArrBufferHeader, numCharsHeader);

	// Buffer headers 
	rewind(pfInput);
	size_t numCharsRead = fread(pArrBufferHeader->pData, sizeof(char), pArrBufferHeader->capacity, pfInput);
	// ------------------------------------------------------------------------
	// CURRENT FILE POSITION INDICATOR: at the beginning of the second line
	// ------------------------------------------------------------------------
	
	if (numCharsRead != pArrBufferHeader->capacity)
	{
		printf("ERROR: Cannot buffer header!\n");
		return CSV_PARSING_ERROR;
	}

	pArrBufferHeader->size = numCharsRead;
	// TODO: ııııı????? do i add \0? 

	return SUCCESS;
}


