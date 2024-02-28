#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "definitions.h"

#include "csvParser.h"
#include "CharArr.h"
#include "IntArr.h"

int parseCSV(FILE* pfInput, char* pDelimiter, int* pNumCols, int* pNumValueLines, int* pNumCommentLines, bool* pHasHeaders, CharArr* pHeaderList)
{
	determineNumCols(pfInput, pDelimiter, pNumCols);
	checkHeaders(pfInput, pHasHeaders);

	if (*pHasHeaders)
	{
		// Buffer header ---------------------------------------------
		CharArr header;
		int buffer = bufferHeader(pfInput, &header);

		if (buffer != SUCCESS)
		{
			return CSV_PARSING_ERROR;
		}

		IntArr numHeaderChars;
		createIntArr(&numHeaderChars, *pNumCols);
		if (!pHeaderList)
		{
			return CSV_PARSING_ERROR;
		}

		char** pHeaderList = (char**)malloc(*pNumCols * sizeof(char*));
		if (!numHeaderChars.pData)
		{
			destroyIntArr(&numHeaderChars);
			return CSV_PARSING_ERROR;
		}

		// Separete header --------------------------------------------
		// Get each title's length
		int numChar = 0;
		for (size_t i = 0; i < header.capacity; ++i)
		{
			if (header.pData[i] != *pDelimiter) numChar++;
			else {
				addNewInt(&numHeaderChars, numChar);
				numChar = 0;
			}


		}
		addNewInt(&numHeaderChars, numChar);
		
		
		// Create char arrays based on these lengths 
		// Get each title
		// Put its address in pHeaderList
		int numPrevHeader = 0;
		for (int iHeader = 0; iHeader < *pNumCols; iHeader++)
		{
			size_t numChars = (size_t) numHeaderChars.pData[iHeader];
			
			CharArr headerArr;
			createCharArr(&headerArr, numChars);
			
			// Get previous headers char nums sum
			if (iHeader != 0) {
				numPrevHeader += numHeaderChars.pData[iHeader - 1] + 1;
			}
			// numPrevHeader = (iHeader == 0) ? 0 : numHeaderChars.pData[iHeader - 1] + iHeader;
			
			// Add each character to header
			for (int iChar = numPrevHeader; iChar < numChars + numPrevHeader; iChar++)
			{
				addNewChar(&headerArr, header.pData[iChar]);
			}

			// Add CharArr's address to pHeaderList
			pHeaderList[iHeader] = headerArr.pData;
		}
		
		destroyCharArr(&header);
	}
	
	if (validateData != SUCCESS)
	{
		return CSV_PARSING_ERROR;
	}
	

	return ERROR;
}


int validateData(FILE* pfInput, char* pDelimiter)
{
	// Check if each non-comment line has the same number of delimiters
	
	
	// Determine columns data type: number or string
	// Check if each column is of that type or null 
	
	
	return CSV_PARSING_ERROR;
}

void getToFirstNonCommentLine(FILE* pfInput)
{
	return;
}


void checkHeaders(FILE* pfInput, bool* pHasHeaders)
{
	// HOW?!!
	// Compare data types of the first and second non-comment lines columns
}

void determineNumCols(FILE* pfInput, char* pDelimiter, int* pNumCols) {
	// Count number of delimiters in the first non-comment line
	getToFirstNonCommentLine(pfInput);
	int numDelimiters = 0;

#if 0
	char ch;

	while ((ch = getc(pfInput)) != '\n')
	{
		if (ch == *pDelimiter) {
			numDelimiters++;
		}
	}

	*pNumCols = numDelimiters + 1;
#endif

	int ch;
	while ((ch = getc(pfInput)) != EOF)
	{
		char strCh = (char)ch;

		if (strCh == '\n')
			break;

		if (strCh == *pDelimiter)
			numDelimiters++;
	}

	*pNumCols = numDelimiters + 1;
	rewind(pfInput);
};

int bufferHeader(FILE* pfInput, CharArr* pArrBufferHeader)
{
	if (!pfInput)
	{
		printf("ERROR: ERROR: Cannot open file\n");
		return CSV_PARSING_ERROR;
	}

	// Get number of characters to create dynamic char array
	size_t numCharsHeader = 0;
	int ch;

	while ((ch = getc(pfInput)) != EOF)
	{
		if ((char)ch == '\n') break;
		if ((char)ch == '\"') continue;
		// TODO: 'Name', Location's code --> What to do with ' 

		numCharsHeader++;
	}
 
	createCharArr(pArrBufferHeader, numCharsHeader);

	// Buffer header 
	rewind(pfInput);
	size_t numCharsRead = fread(pArrBufferHeader->pData, sizeof(char), pArrBufferHeader->capacity, pfInput);
	
	if (numCharsRead != pArrBufferHeader->capacity)
	{
		printf("ERROR: Cannot buffer header!\n");
		return CSV_PARSING_ERROR;
	}

	pArrBufferHeader->size = numCharsRead;
	// TODO: ııııı????? do i add \0? 

	return SUCCESS;
}


