#include "Parameters.h"

int parseCommandLineParameters(int argc, char* argv[], Parameters* pParams);
void findMissingFilePath(char* pPath);
int validateFileExtension(char* pPath, char extn[]);
int argHeadersOrDelimiter(char* pArg, Parameters* pParams);
int displayExampleUsageQuit();
void displayHelpText();

