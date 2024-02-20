int parseCommandLineParameters(int argc, char* argv[], FILE** ppfInput, FILE** ppfOutput, char* pDelimiter);
void findMissingFilePath(char* pPath);
int validateFileExtension(char* pPath, char extn[]);
int displayExampleUsageQuit();
void displayHelpText();
