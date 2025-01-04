#ifndef SECOND_CYCLE_FUNCTIONS_H
#define SECOND_CYCLE_FUNCTIONS_H
int doSecondCycle(char* fileName, Line* lines, int numOfLines, Symbol* symbols, int numOfSymbols, char** missingParams, int IC, int DC);
int createMissingLine(char* param, Symbol* symbols, int numOfSymbols, Line* line);
char* createObjectFileContent(Line* lines, int numOfLines, int IC, int DC);
char* checkIfThereIsExternalParams(Line* lines, int numOfLines, Symbol* symbols, int numOfSymbols);
char* checkIfThereIsEntriesParams(Symbol* symbols, int numOfSymbols);
#endif
