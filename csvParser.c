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

		for (size_t i = 0; i < *pNumCols; ++i)
		{
			// TODO NOW: separete header
			// get each title's length
			// create dynamic char arrays based on these lengths 
			// get each title
			// put its address in pHeaderList


		}
		
		
		// TOOD: getHeaders()


		destroyCharArr(&header);
		

#if 0
		IntArr numHeaderLetters;
		createIntArr(&numHeaderLetters, *pNumCols);
		setNumHeaderLetters(&numHeaderLetters, pfInput, pDelimiter);
		// TODO: getNumHeadersLetters

		// setHeaders(pfInput, pHeaderList, pNumCols);
		// TODO: getHeaders

		destroyIntArr(&numHeaderLetters);
#endif
		
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

void getToFirstNonCommentLine(FILE* pfInput)
{
	/*
	^: current location
	|: desired location

	^# This is a CSV file
	|Name,Age,Location
	John,25,New York

	^# This is a CSV file
	# Contains headers
	|Name,Age,Location
	John,25,New York

	Name,Age,Location
	^# Here begins data
	|John,25,New York
	*/


}

void setNumHeaderLetters(IntArr* pNumHeaderLetters, FILE* pfInput, char* pDelimiter)
{
	if (!pfInput)
	{
		printf("ERROR: Cannot open file\n");
		return;
	}

	int letter;
	int numLetters = 0;
	
	while ((letter = getc(pfInput)) != EOF)
	{
		// TODO: escape sequences 

		if ((char) letter == *pDelimiter)
		{
			addNewElement(pNumHeaderLetters, numLetters);
			numLetters = 0;
			continue;
		}

		if ((char)letter == '\n')
		{
			addNewElement(pNumHeaderLetters, numLetters);
			break;
		}

		numLetters++;
	}

	rewind(pfInput);
}

int getNumHeaderLetters(IntArr* pNumHeaderLetters, int* pNumCols)
{
	int totalNumHeadersLine = *pNumCols - 1;
	
	for (size_t i = 0; i < pNumHeaderLetters->size; i++)
	{
		totalNumHeadersLine += pNumHeaderLetters->pData[i];
	}
	
	return totalNumHeadersLine;
};

void setHeaders(FILE* pfInput, CharArr* pHeaderList, IntArr* pNumHeaderLetters)
{
	for (int i = 0; i < pNumHeaderLetters->size; ++i)
	{
		CharArr header;
		createCharArr(&header, pNumHeaderLetters->pData[i]);
		fread(header.pData, sizeof(char), pNumHeaderLetters->pData[i], pfInput);
	}
}

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


