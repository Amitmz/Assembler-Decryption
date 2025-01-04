#include "objects.h"
#ifndef PRE_ASSEMBLER_FUNCTIONS_H
#define PRE_ASSEMBLER_FUNCTIONS_H

/*void freeAll(FILE** filep, char** asFullName, char** amFullName, Macro** macros);*/

void createMacro(Macro* macro, char* macroName, int macroLoc);

int findMacroName(char* macroLine, char** macroName);

int checkMacroNameExists(Macro* macros, int macrosSize, char* macroName, Macro* thisMacro);

int hundleMacro(char* line, Macro** macros, int macrosSize);

int addContentToMacro(Macro* macros, int macroLoc, char* content);

int addContentToFileContent(char** fileContent, char* content);

void createFile(char* fileName, char* content);

void createAMFile(char* assemblerFileName, char* contentFile);

#endif
