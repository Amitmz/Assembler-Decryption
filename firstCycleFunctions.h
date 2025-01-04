#ifndef FIRST_CYCLE_FUNCTIONS_H
#define FIRST_CYCLE_FUNCTIONS_H
int checkIfDefineIsValid(char* line, int lineNum, char** symbolName, Symbol* symbols, int numOfSymbols);
int createSymbolList(Symbol** symbols, int* numOfSymbols, char* symbolName, char* symbolFeature, int symbolVal);
int CheckCorrectnessOfCommands(char* line, char** command, char*** dataParams, int* sizeOfParams, int lineNum);
char* calculateCommand(int lineNum, char* command, char** params, int paramSize, Symbol* symbols, int numOfSymbols);
void buildOpcode(char* command, char* binaryCode);
void buildAddressing(int addressing, char* binaryCode, int from, int to);
int createCommandLines(char** binary, Symbol* symbols, int numOfSymbols, char* line, int lineNum, int isSymbol, char*** params, int* numOfParams);
int checkTypeOfAddress(char* param, Symbol* symbols, int numOfSymbols);
void changeValueForDataSymbols(Symbol* symbols, int symbolsSize, int addValue);
int createBinaryList(char*** binaryList, int size, int value, int startPos, int from, int to);
int createMissingParamsList(char* param, char*** paramList, int* numOfMissingParams);
int createParamsLines(char** params, int numOfParams, Symbol* symbols, int numOfSymbols, int lineNum, Line** lines, int* numOfLines, int address, char*** missingParams, int* numOfMissingParams);
int doFirstCycle(char* assemblerFileName, Symbol** symbols, int* numOfSymbols, Line** lines, int* numOfLines, char*** missingParams, int* numOfMissingParams, int* IC, int* DC);
#endif
