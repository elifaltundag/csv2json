#include <stdio.h>
#include <stdbool.h>

#include "definitions.h"
#include "Parameters.h"

#include "commandLineParser.h"
#include "csvParser.h"
#include "jsonWriter.h"



int main(int argc, char* argv[])
{
	Parameters params;
	Parameters* pParams = &params;
	initializeParameters(pParams);

	if (parseCommandLineParameters(argc, argv, pParams) != SUCCESS)
	{
		return CMDLINE_PARSING_ERROR;
	}

	


#if 0
	if (jsonWriter(pfOutput, pfInput, &delimiter, &numCols, &hasHeaders, &ppHeaderList))
	{
		return JSON_WRITING_ERROR;
	}
	if (parseCSV(pfInput, &delimiter, &numCols, &numValueLines, &numCommentLines, &hasHeaders, &ppHeaderList) != SUCCESS)
	{								   
		return CSV_PARSING_ERROR;	   
	}


	// FIX: If you cant parse CSV you dont close the files!!
	if (pfInput != NULL) {
		fclose(pfInput);
	}

	if (pfOutput != NULL) {
		fclose(pfOutput);
	}
#endif
		
	return SUCCESS;
}