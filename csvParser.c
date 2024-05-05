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
	determineNumCols(pParams);

	if (validateData(pParams) != SUCCESS) return CSV_PARSING_ERROR;
	if (bufferCSVContents(pParams) != SUCCESS) return CSV_PARSING_ERROR;

	// -----------------------------------------------------------
	// Save each header if the CSV file has headers
	// -----------------------------------------------------------
	if (pParams->hasHeaders)
	{
		// Buffer headers ---------------------------------------------
		CharArr allHeaders;
		int allHeadersBuffered = bufferHeaders(pParams->pfInput, &allHeaders);

		if (allHeadersBuffered != SUCCESS)
		{
			return CSV_PARSING_ERROR;
		}

		IntArr numHeaderChars;
		createIntArr(&numHeaderChars, pParams->numCols);

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

			if (allHeaders.pData[i] != pParams->delimiter) numChar++;
			else {
				addNewInt(&numHeaderChars, numChar);
				numChar = 0;
			}
		}
		addNewInt(&numHeaderChars, numChar);
	

		// Get each header and store their address in *pppHeaderList
		pParams->ppHeaderList = (char**)malloc(pParams->numCols * sizeof(char*));
		if (!pParams->ppHeaderList) {
			return CSV_PARSING_ERROR;
		}

		int numAddedHeader = 0;
		int iStart = 0;
		for (int iHeader = 0; iHeader < pParams->numCols; iHeader++) {
			int sizeHeader = numHeaderChars.pData[iHeader];

			char* header = (char*)malloc((sizeHeader + 1) * sizeof(char)); // +1 for null termination
			if (!header) {
				return CSV_PARSING_ERROR;
			}

			for (int i = 0; i < sizeHeader; i++) {
				header[i] = allHeaders.pData[i + iStart];
			}
			header[sizeHeader] = '\0';

			(pParams->ppHeaderList)[numAddedHeader] = header;
			numAddedHeader++;


			iStart += sizeHeader + 1; // +1 to skip delimiter in allHeaders 
		}
		destroyIntArr(&numHeaderChars);
		destroyCharArr(&allHeaders);
	}
	
	return SUCCESS;
}

int validateData(Parameters* pParams)
{
	// -----------------------------------------------------------
	// Count value and comment lines: DONE
	// Incorrect comma count per entry: DONE
	// 
	// TODO
	// Get each data type: String || Number
	// Do all entries match these data types or null?
	// -----------------------------------------------------------

	countParameters(pParams);
	int validNumEntries = eachLineHasSameNumCols(pParams);
#
	if (validNumEntries != SUCCESS) {
		printf("ERROR! Invalid number of entries at line %d\n", validNumEntries);
		return CSV_PARSING_ERROR;
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

	pParams->csvContents = (char*)malloc((pParams->numChars + 1) * sizeof(char)); // +1 for null termination
	if (!pParams->csvContents) {
		printf("Cannot allocate enough memory to buffer CSV contents\n");
		return CSV_PARSING_ERROR;
	}
	
	size_t numCharsRead = fread(pParams->csvContents, pParams->numChars, 1, pParams->pfInput);
	pParams->csvContents[pParams->numChars] = '\0';
	
	
	fclose(pParams->pfInput);
	pParams->pfInput = NULL;

	
	return SUCCESS;
}

void countParameters(Parameters* pParams)
{
	// Comment line specifiers: '//', '#'  
	// if the file has headers go to next line
	
	rewind(pParams->pfInput);


	int numValueLines = 1;
	int numCommentLines = 0;
	size_t numChars = 0;
	int cur;
	bool isPrevCharSlash = false;
	bool isNewLine = true;

	while ((cur = getc(pParams->pfInput)) != EOF)
	{
		char curChar = (char)cur;
		numChars++;
		
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

	if (pParams->hasHeaders) numValueLines--;
	pParams->numValueLines = numValueLines;
	pParams->numCommentLines = numCommentLines;
	pParams->numChars = numChars;
}

// If each non-comment line has the same number of entries (columns) as pNumCol returns 0 (SUCCESS)
// Else returns the first invalid line's order
int eachLineHasSameNumCols(Parameters* pParams)
{
	rewind(pParams->pfInput);

	int cur, numLineDelimiters = 0, curLine = 1;
	bool isPrevCharSlash = 0;

	while ((cur = getc(pParams->pfInput)) != EOF) {
		char curChar = (char)cur;
		
		if (curChar == pParams->delimiter) numLineDelimiters++;
		
		else if (curChar == '\n') {
			if (numLineDelimiters + 1 < pParams->numCols) return curLine;
			curLine++;
			numLineDelimiters = 0;
		}
		
		if (numLineDelimiters >= pParams->numCols) return curLine;
		

		// TODO: skip comment lines
		/* 
		else if (curChar == '#') continue;
		else if (isPrevCharSlash && curChar == '/') continue;
		else if (!isPrevCharSlash && curChar == '/') isPrevCharSlash = !isPrevCharSlash;
		*/
	}

	
	
	return SUCCESS;
}

void determineNumCols(Parameters* pParams) {
	int numDelimiters = 0;

	int currentChar;
	while ((currentChar = getc(pParams->pfInput)) != EOF)
	{
		char strCh = (char)currentChar;

		if (strCh == '\n')
			break;

		if (strCh == pParams->delimiter)
			numDelimiters++;
	}

	pParams->numCols = numDelimiters + 1;
	rewind(pParams->pfInput);
};

int bufferHeaders(FILE* pfInput, CharArr* pArrBufferHeader)
{
	if (!pfInput)
	{
		printf("ERROR: Cannot open CSV file\n");
		return CSV_PARSING_ERROR;
	}
	rewind(pfInput);

	// Get number of characters of the first line of CSV file as size of buffer (char array)
	size_t numCharsHeader = 0;
	int ch;

	while ((ch = getc(pfInput)) != EOF)
	{
		char curCh = (char)ch;
		if (curCh == '\n') break;
		if (curCh == '\"') continue;
		numCharsHeader++;
	}
 
	// +1 for null termination
	createCharArr(pArrBufferHeader, numCharsHeader + 1);

	// Buffer headers 
	rewind(pfInput);
	size_t numCharsRead = fread(pArrBufferHeader->pData, sizeof(char), pArrBufferHeader->capacity - 1, pfInput);
	pArrBufferHeader->size = numCharsRead;
	
	pArrBufferHeader->pData[pArrBufferHeader->size] = '\0';
	pArrBufferHeader->size++;
	// ------------------------------------------------------------------------
	// Current file position indicator: at the beginning of the second line
	// ------------------------------------------------------------------------
	
	if (numCharsRead + 1 != pArrBufferHeader->size)
	{
		printf("ERROR: Cannot buffer header!\n");
		return CSV_PARSING_ERROR;
	}

	return SUCCESS;
}


