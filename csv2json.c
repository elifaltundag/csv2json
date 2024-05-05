#include <stdio.h>
#include <stdbool.h>

#include "definitions.h"
#include "Parameters.h"

#include "commandLineParser.h"
#include "csvParser.h"
#include "jsonWriter.h"


void closeFiles(Parameters* pParams) {
	if (pParams->pfInput != NULL)
	{
		fclose(pParams->pfInput);
		pParams->pfInput = NULL;
	}
	
	if (pParams->pfOutput != NULL)
	{
		fclose(pParams->pfOutput);
		pParams->pfOutput = NULL;
	}
}


int main(int argc, char* argv[])
{
	Parameters params;
	Parameters* pParams = &params;
	initializeParameters(pParams);

	if (parseCommandLineParameters(argc, argv, pParams) != SUCCESS)
	{
		closeFiles(pParams);
		return CMDLINE_PARSING_ERROR;
	}

	if (parseCSV(pParams) != SUCCESS)
	{							
		closeFiles(pParams);
		return CSV_PARSING_ERROR;	   
	}


	if (jsonWriter(pParams) != SUCCESS)
	{
		closeFiles(pParams);
		return JSON_WRITING_ERROR;
	}

	
	closeFiles(pParams);
	return SUCCESS;
}

