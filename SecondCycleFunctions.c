#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "objects.h"
#include "SecondCycleFunctions.h"
#include "helpfulFunctions.h"
#include "binaryHelpfulFunctions.h"

int doSecondCycle(char* fileName, Line* lines, int numOfLines, Symbol* symbols, int numOfSymbols, char** missingParams, int IC, int DC)
{
    int i = 0, paramIterator = 0, fullNameSize;
    char* entryContent = NULL, *externalContent = NULL, *objectContent = NULL;
    /*Fill missing lines*/
    for(i=0; i<numOfLines; i++)
    {
        if(lines[i].binaryResult == NULL)
        {
            /*Fill the missing line - IF THE DATA SAVED IN SYMBOLS DATA STRUCTURE*/
            if(createMissingLine(missingParams[paramIterator], symbols, numOfSymbols, &(lines[i])))
            {
                printf("There is no information about the param %s!\n", missingParams[paramIterator]);
                return 0;
            }
            paramIterator++;
        }
    }

    /*Create object file*/
    objectContent = createObjectFileContent(lines, numOfLines, IC, DC);
    fullNameSize = strlen(fileName)+3; /*fileName +".am" length*/
    createNameWithExt(&fileName, fileName, fullNameSize, ".ob");
    createFile(fileName, objectContent);

    /*Create externals file*/
    externalContent = checkIfThereIsExternalParams(lines, numOfLines, symbols, numOfSymbols);
    if(externalContent != NULL)
    {
        fullNameSize = strlen(fileName)+4; /*fileName +".ext" length*/
        createNameWithExt(&fileName, fileName, fullNameSize, ".ext");
        createFile(fileName, externalContent);
    }

    /*Create entries file*/
    entryContent = checkIfThereIsEntriesParams(symbols, numOfSymbols);
    if(entryContent != NULL)
    {
        fullNameSize = strlen(fileName)+4; /*fileName +".ent" length*/
        createNameWithExt(&fileName, fileName, fullNameSize, ".ent");
        createFile(fileName, entryContent);
    }
    return 1;
}

int createMissingLine(char* param, Symbol* symbols, int numOfSymbols, Line* line)
{
    char* binaryValue;
    int paramLoc, i;
    paramLoc = checkIfTheSymbolExists(symbols, numOfSymbols, param);
    if(paramLoc != -1)
    {
        (*line).binaryResult = (char*)malloc((BINARY_CODE_SIZE+1)*sizeof(char));
        if((*line).binaryResult == NULL)
        {
            printf("Allocation failed! (Create Missing Lines)\n");
            return 0;
        }
        for(i = 0; i< BINARY_CODE_SIZE; i++)
           (*line).binaryResult[i] = '0';
        (*line).binaryResult[i] = '\0';
        binaryValue = decimalToBinary(symbols[paramLoc].symbolValue);
        strncpy((*line).binaryResult,binaryValue + 2,12);
        /*Check addressing type*/
        if(strcmp(symbols[paramLoc].symbolFeature, "external") == 0)
            strcpy((*line).binaryResult+12,"01");
        else
            strcpy((*line).binaryResult+12,"10");
    }
    return 0;
}

char* createObjectFileContent(Line* lines, int numOfLines, int IC, int DC)
{
    int i;
    char* content = NULL;
    char* fourBase;
    for(i=0; i<numOfLines; i++)
    {
        /*Convert to base 4*/
        fourBase = fourToCode(lines[i].binaryResult);
        createContentForFiles(&content, lines[i].address, fourBase, 1);
        free(fourBase);
    }
    return content;
}

char* checkIfThereIsExternalParams(Line* lines, int numOfLines, Symbol* symbols, int numOfSymbols)
{
    int i;
    char* content = NULL;
    for(i=0; i<numOfLines; i++)
    {
       /*Check if the line contains external symbol*/
       if(lines[i].param !=NULL && getSymbolDefineValue(lines[i].param, symbols, numOfSymbols, "extern") != -1)
       {
            createContentForFiles(&content, lines[i].address, lines[i].param, 0);
       }
    }
    return content;
}

char* checkIfThereIsEntriesParams(Symbol* symbols, int numOfSymbols)
{
    int i;
    char* content = NULL;
    for(i=0; i<numOfSymbols; i++)
    {
        if(strcmp(symbols[i].symbolFeature, "entry") == 0)
        {
            createContentForFiles(&content, symbols[i].symbolValue, symbols[i].symbolName, 0);
        }
    }
    return content;
}

