#include <stdio.h>
#include <string.h>
#include "definitions.h"
#include "commandLineParser.h"

int parseCommandLineParameters(int argc, char* argv[], FILE** ppfInput, FILE** ppfOutput, char* pDelimiter)
{
	
	if (argc == 1)
	{
		printf("ERROR! Missing input and output paths\n");
	}


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

	if (argc >= 3) {
		
		/* Validate file paths
		1. Check extension
		2. Access 
		*/
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

		// Assign new delimiter if provided
		// What happens with whitespace?
		if (argc > 3) *pDelimiter = *(argv[3]);

		printf("Valid input and output paths\nCSV delimiter: %c\n", *pDelimiter);
		return SUCCESS;

#if 0
		if (errInput == SUCCESS && errOutput == SUCCESS)
		{

			// Assign new delimiter if provided
			// What happens with whitespace?
			if (argc > 3)
			{
				*pDelimiter = *(argv[3]);
			}

			printf("Valid input and output paths\nCSV delimiter: %c\n", *pDelimiter);
			return SUCCESS;
		}
		else if (errInput == SUCCESS) printf("ERROR! Invalid output path\n");
		else if (errOutput == SUCCESS) printf("ERROR! Invalid input path\n");
		else printf("ERROR! Invalid input and output paths\n");
#endif
	}
	
	return displayExampleUsageQuit();
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

