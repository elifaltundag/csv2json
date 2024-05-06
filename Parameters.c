#include "Parameters.h"

void initializeParameters(Parameters* pParams)
{
	pParams->pfInput = NULL; 
	pParams->pfOutput = NULL;

	pParams->delimiter = ','; // default delimiter: comma

	pParams->hasHeaders = false;
	pParams->ppHeaderList = NULL;
	
	pParams->numChars = 0;
	pParams->csvContents = NULL;

	pParams->numCols = 0;
	pParams->numValueLines = 0;
	pParams->numCommentLines = 0;
}

