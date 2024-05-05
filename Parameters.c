#include "Parameters.h"

void initializeParameters(Parameters* pParams)
{
	pParams->pfInput = NULL; 
	pParams->pfOutput = NULL;
	pParams->delimiter = ',';
	pParams->hasHeaders = false;
	pParams->numCols = 0;
	pParams->numValueLines = 0;
	pParams->numCommentLines = 0;
	pParams->ppHeaderList = NULL;
}

