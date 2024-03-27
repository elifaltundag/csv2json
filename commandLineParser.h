int parseCommandLineParameters(int argc, char* argv[], FILE** ppfInput, FILE** ppfOutput, char* pDelimiter, bool* pHasHeaders);
void findMissingFilePath(char* pPath);
int validateFileExtension(char* pPath, char extn[]);
int displayExampleUsageQuit();
void displayHelpText();
int argHeadersOrDelimiter(char* pArg, char* pDelimiter, bool* hasHeaders);