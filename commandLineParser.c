#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "definitions.h"
#include "commandLineParser.h"

int parseCommandLineParameters(int argc, char* argv[], FILE** ppfInput, FILE** ppfOutput, char* pDelimiter, bool* pHasHeaders)
{
	// No input or output path
	if (argc == 1)
	{
		printf("ERROR! Missing input and output paths\n");
	}

	// Two possibilities:
	// 1. User needs help
	// 2. User missed input or output path
	if (argc == 2)
	{
		if (strcmp(argv[1], "--help") == SUCCESS || strcmp(argv[1], "-help") == SUCCESS || strcmp(argv[1], "--h") == SUCCESS || strcmp(argv[1], "-h") == SUCCESS)
		{
			displayHelpText();
		}
		else {
			findMissingFilePath(argv[1]);
		}

	}

	// Validate file paths
	// 1. Check extension
	// 2. Access
	if (argc >= 3) {		
		int extnInput = validateFileExtension(argv[1], "csv");
		if (extnInput != SUCCESS)
		{
			printf("ERROR! Invalid input path\n");
			return displayExampleUsageQuit();
		}

		int extnOutput = validateFileExtension(argv[2], "json");
		if (extnOutput != SUCCESS)
		{
			printf("ERROR! Invalid output path\n");
			return displayExampleUsageQuit();
		}

		errno_t errInput  = fopen_s(ppfInput, argv[1], "r");
		if (errInput != SUCCESS)
		{
			printf("ERROR! Invalid input path\n");
			return displayExampleUsageQuit();
		}

		// w: opens to write only, if the file already exists its content is deleted 
		// w+: opens to read+write, if the file already exists its content is deleted 
		errno_t errOutput = fopen_s(ppfOutput, argv[2], "w+");
		if (errOutput != SUCCESS)
		{
			fclose(*ppfInput);
			printf("ERROR! Invalid output path\n");
			return displayExampleUsageQuit();
		}

		printf("Valid input and output paths\n");
		if (argc == 3) {
			printf("Has 3 arguments\n");
			return SUCCESS;
		}
		
		
	}

	// Third argument: headers or delimiter
	if (argc == 4) {
		char* pArg3 = argv[3];
		
		int thirdArgFigured = argHeadersOrDelimiter(pArg3, pDelimiter, pHasHeaders);
		
		if (thirdArgFigured == DELIMITER || thirdArgFigured == HEADERS) return SUCCESS;
		bool goon = true;
	}


	// Possibilities
	//
	// argv[3]: headers, argv[4]: delimiter
	// ... headers ;
	//
	// argv[4]: headers, argv[3]: delimiter
	// ... ; headers

	if (argc == 5) {
		char* pArg3 = argv[3];
		char* pArg4 = argv[4];

		int thirdArgFigured = argHeadersOrDelimiter(pArg3, pDelimiter, pHasHeaders);
		int fourthArgFigured = argHeadersOrDelimiter(pArg4, pDelimiter, pHasHeaders);
		bool cond = ((thirdArgFigured == HEADERS && fourthArgFigured == DELIMITER) || (thirdArgFigured == DELIMITER && fourthArgFigured == HEADERS));

		if (cond) return SUCCESS;

		return CSV_PARSING_ERROR;
	}

	
	return displayExampleUsageQuit();
}

int argHeadersOrDelimiter(char* pArg, char* pDelimiter, bool* hasHeaders) {
	// ASCII code Delimiter
		//         44 , 
		//         59 ;
		//        124 |	
		//        ??? \t // not yet added

	if (*pArg == 44 || *pArg == 59 || *pArg == 124) {
		*pDelimiter = *pArg;
		printf("CSV delimiter: %c\n", *pDelimiter);
		return DELIMITER;
	}

	if (strcmp(pArg, "headers") == SUCCESS) {
		*hasHeaders = true;
		printf("CSV file has headers\n");
		return HEADERS;
	}

	return CSV_PARSING_ERROR;
}



void findMissingFilePath(char* pPath)
{
	if ((validateFileExtension(pPath, "csv") == SUCCESS))
	{
		printf("ERROR! Missing output path\n");
	}
	else if ((validateFileExtension(pPath, "json") == SUCCESS))
	{
		printf("ERROR! Missing input path\n");
	}
	else
	{
		printf("Invalid file extension\n");
	}
}

int validateFileExtension(char* pPath, char extn[])
{
	char* pLastDot = strrchr(pPath, '.');
	if (pLastDot && strcmp(pLastDot + 1, extn) == SUCCESS)
	{
		return SUCCESS;
	}
	return CMDLINE_PARSING_ERROR;
}

int displayExampleUsageQuit()
{
	printf("Example usage:\n\t./csv2json.exe C:/src/data.csv C:/dst/filename.json\n");
	return CMDLINE_PARSING_ERROR;
}

void displayHelpText()
{
	/* TODO: help text */
	printf("\n\n------------  HELP  ------------\n\n");
}

