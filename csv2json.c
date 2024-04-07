#include <stdio.h>
#include <stdbool.h>
#include "definitions.h"

#include "CharArr.h"

#include "commandLineParser.h"
#include "csvParser.h"
#include "jsonWriter.h"

int main(int argc, char* argv[])
{
	FILE* pfInput = NULL;
	FILE* pfOutput = NULL;
	char delimiter = ',';
	bool hasHeaders = false;

	if (parseCommandLineParameters(argc, argv, &pfInput, &pfOutput, &delimiter, &hasHeaders) != SUCCESS)
	{
		return CMDLINE_PARSING_ERROR;
	}

	int numCols = 0;
	int numValueLines = 0;
	int numCommentLines = 0;
	char** ppHeaderList = NULL;


	if (parseCSV(pfInput, &delimiter, &numCols, &numValueLines, &numCommentLines, &hasHeaders, &ppHeaderList) != SUCCESS)
	{								   
		return CSV_PARSING_ERROR;	   
	}

	if (jsonWriter(pfOutput, pfInput, &delimiter, &hasHeaders))
	{
		return JSON_WRITING_ERROR;
	}


	// FIX: If you cant parse CSV you dont close the files!!
	if (pfInput != NULL) {
		fclose(pfInput);
	}

	if (pfOutput != NULL) {
		fclose(pfOutput);
	}
		
	return SUCCESS;
}