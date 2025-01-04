#include <stdio.h>
#include <stdlib.h>
#include "preAssemblerFunctions.h"
#include "firstCycleFunctions.h"
#include "SecondCycleFunctions.h"
void freeParams(char* content, char** missingParams, int numOfMissingParams, Symbol* symbols, Line* lines)
{
    int i;
    free(content);

    for(i=0; i<numOfMissingParams; i++)
    {
        free(missingParams[i]);
    }
    free(missingParams);
    free(symbols);
    free(lines);
}

int main()
{
    char* content = NULL, **missingParams = NULL;
    Symbol* symbols = NULL;
    Line* lines = NULL;
    int numOfSymbols = 0, numOfLines = 0, numOfMissingParams = 0, isSuccess = 1;
    int IC = 100, DC = 0;
    char* fileName = "C:\\Users\\shani\\Desktop\\AssemblerProject\\check"; /*REPLACE TO RELATIVE PATH*/
    printf("Do Pre-assembler Step\n");
    createAMFile(fileName,content);
    printf("Finish with Pre-assembler step!\n");

    printf("Start with Assembler first scanning\n");
    isSuccess = doFirstCycle(fileName, &symbols, &numOfSymbols, &lines, &numOfLines, &missingParams, &numOfMissingParams, &IC, &DC);
    if(!isSuccess)
    {
        return 1;
    }
    printf("Finish with First scanning step!\n");

    printf("Start with Assembler second scanning\n");
    isSuccess = doSecondCycle(fileName, lines,numOfLines, symbols, numOfSymbols, missingParams, IC-100, DC);
    if(!isSuccess)
    {
        return 1;
    }
    printf("Finish with Second scanning step!\n");

    printf("End program!\n");

    freeParams(content, missingParams, numOfMissingParams, symbols, lines);
    return 0;
}
