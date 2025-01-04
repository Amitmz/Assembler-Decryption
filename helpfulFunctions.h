#include "objects.h"
#ifndef HELPFUL_FUNCTIONS_H
#define HELPFUL_FUNCTIONS_H

/*Create file name with chosen extension*/
void createNameWithExt(char** fullName, char* fileName, int nameSize, char* extention);
/*Helpful function - Remove unrelevent characters (multiple spaces, tab, enter) and return a string that contains one space between any word*/
void arrangeStr(char *input, char *output, int removeAll);
/*Helpful function: remove all the spaces from string*/
void removeSpaces(char *input, char *output);
/*Helpful function - according to action, returns the opcode*/
char* returnOpcodePerAction(char* action);
/*Helpful function = Check if str is existing symbol*/
int checkIfTheSymbolExists(Symbol* symbols, int amountOfSymbols, char* symbol);
/*Helpful function - according to action, returns the number of operands*/
int numberOfOperandsPerAction(char* action);
/*The function check if the name is command*/
int checkIfCommand(char* name);
/*The function check if the name is register*/
int checkIfRegister(char* name);
/*The function check if the name is not command or register*/
int checkIfTheNameValid(char* name);
int findNextString(char* line, int isDefine, char** symbolName);
int getSymbolDefineValue(char* symbol, Symbol* symbols, int numOfSymbols, char* type);
/*According to the seperator - returns the string before / after the seperator*/
int findSubString(char* str, char sep, int start, int last, int after, char** result);
/*Function that return a new file*/
void createFile(char* fileName, char* content);
int createContentForFiles(char** content, int address, char* symbolName, int addressFirst);
char* intToString(int num);
int splitBySep(char* str, char* sep, char*** dataValues, int* dataSize);
int handleStringStatement(char* line, int** dataValues, int* dataSize);
int handleDataStatement(char* line, int lineNum, int* errors, int** dataValues, int* dataSize, Symbol* symbols, int symbolSize);
int createLinesList(Line** lines, int* numOfLines, int address, char* param, char* binValue);
void swipe(char* binaryArr, int size);
#endif

