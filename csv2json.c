#include <stdio.h>
#include <stdbool.h>

#include "definitions.h"
#include "Parameters.h"

#include "commandLineParser.h"
#include "csvParser.h"
#include "jsonWriter.h"


void closeFiles(Parameters* pParams) {
	if (pParams->pfInput != NULL) fclose(pParams->pfInput);
	if (pParams->pfOutput != NULL) fclose(pParams->pfOutput);
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

#if 0
	if (jsonWriter(pfOutput, pfInput, &delimiter, &numCols, &hasHeaders, &ppHeaderList))
	{
		closeFiles(pParams);
		return JSON_WRITING_ERROR;
	}
#endif
	
	closeFiles(pParams);
	return SUCCESS;
}

