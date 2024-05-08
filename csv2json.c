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
		if (!pParams->headers) {
			free(pParams->headers);
			pParams->headers = NULL;
		}
		
		if (!pParams->ppHeaderList) 
		{
			free(pParams->ppHeaderList);
			pParams->ppHeaderList = NULL;
		}

		if (!pParams->csvContents) {
			free(pParams->csvContents);
			pParams->csvContents = NULL;
		}
		
		
		closeFiles(pParams);
		return CSV_PARSING_ERROR;	   
	}


	if (jsonWriter(pParams) != SUCCESS)
	{
		if (!pParams->headers) {
			free(pParams->headers);
			pParams->headers = NULL;
		}

		if (!pParams->ppHeaderList)
		{
			free(pParams->ppHeaderList);
			pParams->ppHeaderList = NULL;
		}

		if (!pParams->csvContents) {
			free(pParams->csvContents);
			pParams->csvContents = NULL;
		}

		if (!pParams->jsonContents) {
			free(pParams->jsonContents);
			pParams->jsonContents = NULL;
		}
		
		closeFiles(pParams);
		return JSON_WRITING_ERROR;
	}

	if (!pParams->headers) {
		free(pParams->headers);
		pParams->headers = NULL;
	}

	if (!pParams->ppHeaderList)
	{
		free(pParams->ppHeaderList);
		pParams->ppHeaderList = NULL;
	}

	if (!pParams->csvContents) {
		free(pParams->csvContents);
		pParams->csvContents = NULL;
	}

	if (!pParams->jsonContents) {
		free(pParams->jsonContents);
		pParams->jsonContents = NULL;
	}
	
	closeFiles(pParams);
	return SUCCESS;
}

