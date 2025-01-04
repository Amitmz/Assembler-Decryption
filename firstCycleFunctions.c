#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "objects.h"
#include "helpfulFunctions.h"
#include "firstCycleFunctions.h"
#include "binaryHelpfulFunctions.h"
#include "freeHelperFunctions.h"

int checkIfDefineIsValid(char* line, int lineNum, char** symbolName, Symbol* symbols, int numOfSymbols)
{
    /*Check if there is label before*/
    if(strncmp(line, ".define", strlen(".define")) != 0)
    {
        printf("\nIn line %d: for define - you must not define a label!\n", lineNum);
        return 0;
    }
    else if(findNextString(line, 1, symbolName) == 0)
    {
        return 0;
    }
    /*Check if there is a name for define*/
    else if(*symbolName == NULL)
    {
        printf("In line %d: forget to give a name for define!\n",lineNum);
        return 0;
    }
    else if(!checkIfTheNameValid(*symbolName) || checkIfTheSymbolExists(symbols, numOfSymbols, *symbolName) != -1)
    {
        printf("In line %d: the define name is not a valid name!\n", lineNum);
        return 0;
    }
    return 1;
}

int createSymbolList(Symbol** symbols, int* numOfSymbols, char* symbolName, char* symbolFeature, int symbolVal)
{
    if(*symbols == NULL)
    {
        *symbols = (Symbol*)malloc(sizeof(Symbol));
        if(*symbols == NULL)
        {
            printf("Allocation failed! (symbols)");
            return 0;
        }
    }
    else
    {
        *symbols = (Symbol*)realloc(*symbols, (*numOfSymbols+1) * sizeof(Symbol));
        if(*symbols == NULL)
        {
            printf("Allocation failed! (symbols)");
            return 0;
        }
    }
    (*symbols)[*numOfSymbols].symbolName = (char*)malloc((strlen(symbolName)+1) * sizeof(char));
    if((*symbols)[*numOfSymbols].symbolName == NULL)
    {
        printf("Allocation failed! (symbolName)");
        return 0;
    }
    strcpy((*symbols)[*numOfSymbols].symbolName,symbolName);
    (*symbols)[*numOfSymbols].symbolFeature = (char*)malloc((strlen(symbolFeature)+1) * sizeof(char));
    if((*symbols)[*numOfSymbols].symbolFeature == NULL)
    {
        printf("Allocation failed! (symbolFeature)");
        return 0;
    }
    strcpy((*symbols)[*numOfSymbols].symbolFeature,symbolFeature);
    (*symbols)[*numOfSymbols].symbolValue = symbolVal;
    *numOfSymbols = *numOfSymbols + 1;
    return 1;
}

int CheckCorrectnessOfCommands(char* line, char** command, char*** dataParams, int* sizeOfParams, int lineNum)
{
    char* paramStr = NULL;
    int numOfOperand = 0;

    findNextString(line,0,command);
    /*Check if the command is exists*/
    numOfOperand = numberOfOperandsPerAction(*command);
    if(numOfOperand == -1)
    {
        printf("In line %d: the action %s is not a valid action!\n",lineNum, *command);
        return 0;
    }
    /*build params array*/
    paramStr = strstr(line, *command) + strlen(*command) + 1;
    splitBySep(paramStr,",", dataParams, sizeOfParams);

    /*Check if the number of params is same to definition of the command*/
    if(numOfOperand != *sizeOfParams)
    {
        printf("In line %d: the expected number of operands for action %s is %d but you gave %d!\n",lineNum, *command, numOfOperand, *sizeOfParams);
        return 0;
    }
    return 1;
}

char* calculateCommand(int lineNum, char* command, char** params, int paramSize, Symbol* symbols, int numOfSymbols)
{
    char* binaryCode = NULL;
    int i, typeAddressSource = 0, typeAddressDist = 0;

    binaryCode = (char*)malloc((BINARY_CODE_SIZE+1) * sizeof(char*));
    if(binaryCode == NULL)
    {
        printf("Allocation failed! (Create Command)\n");
        return NULL;
    }
    for(i = 0; i< BINARY_CODE_SIZE; i++)
        binaryCode[i] = '0';
    binaryCode[i] = '\0';
    if(paramSize == 1)
        typeAddressDist = checkTypeOfAddress(params[0],symbols,numOfSymbols);
    else if(paramSize == 2)
    {
        typeAddressSource = checkTypeOfAddress(params[0],symbols, numOfSymbols);
        typeAddressDist = checkTypeOfAddress(params[1],symbols, numOfSymbols);
    }
    buildOpcode(command, binaryCode);
    buildAddressing(typeAddressSource,binaryCode,8,9);
    buildAddressing(typeAddressDist,binaryCode,10,11);
    return binaryCode;
}

void buildOpcode(char* command, char* binaryCode)
{
    char* opcode = NULL;
    opcode = returnOpcodePerAction(command);
    strncpy(binaryCode+4,opcode,OPCODE_SIZE);
}

void buildAddressing(int addressing, char* binaryCode, int from, int to)
{
    char* binaryAddressingCode = NULL;
    char* toBinary = decimalToBinary(addressing);
    binaryAddressingCode = (char*)malloc((to-from+2) * sizeof(char));
    if(binaryAddressingCode == NULL)
    {
        printf("Allocation failed! (Create Address)\n");
        return;
    }
    strcpy(binaryAddressingCode,toBinary + BINARY_CODE_SIZE - 2);
    strncpy(binaryCode+from,binaryAddressingCode,(to-from+1));
    free(binaryAddressingCode);
}

int createCommandLines(char** binary, Symbol* symbols, int numOfSymbols, char* line, int lineNum, int isSymbol, char*** params, int* numOfParams)
{
    char *command = NULL;
    int isError = 0;
    if(isSymbol)
        isError = !CheckCorrectnessOfCommands(strstr(line, " "),&command, params, numOfParams, lineNum);
    else
        isError = !CheckCorrectnessOfCommands(line,&command, params, numOfParams, lineNum);
    *binary = calculateCommand(lineNum, command, *params, *numOfParams, symbols, numOfSymbols);
    return isError;
}
int createBinaryList(char*** binaryList, int size, int value, int startPos, int from, int to)
{
    int i;
    char* binaryValue;
    if(*binaryList == NULL)
    {
        *binaryList = (char**)malloc(sizeof(char*));
        if(*binaryList == NULL)
        {
            printf("Allocation failed! (Create Binary List)\n");
            return 0;
        }
    }
    else
    {
        *binaryList = (char**)realloc(*binaryList, (size+1)*sizeof(char*));
        if(*binaryList == NULL)
        {
            printf("Allocation failed! (Create Binary List)\n");
            return 0;
        }
    }
    (*binaryList)[size-1] = (char*)malloc((BINARY_CODE_SIZE+1)*sizeof(char));
    if((*binaryList)[size-1] == NULL)
    {
        printf("Allocation failed! (Create Binary List)\n");
        return 0;
    }
    for(i = 0; i< BINARY_CODE_SIZE; i++)
       (*binaryList)[size-1][i] = '0';
    (*binaryList)[size-1][i] = '\0';
    binaryValue = decimalToBinary(value);
    strncpy((*binaryList)[size-1]+from,binaryValue + startPos,to-from+1);
    return 1;
}

int createMissingParamsList(char* param, char*** paramList, int* numOfMissingParams)
{
    if(*paramList == NULL)
    {
        *paramList = (char**)malloc(sizeof(char*));
        if(*paramList == NULL)
        {
            printf("Allocation failed! (Create Param List)\n");
            return 0;
        }
    }
    else
    {
        *paramList = (char**)realloc(*paramList, ((*numOfMissingParams)+1)*sizeof(char*));
        if(*paramList == NULL)
        {
            printf("Allocation failed! (Create Param List)\n");
            return 0;
        }
    }
    (*paramList)[(*numOfMissingParams)] = (char*)malloc((MAX_WORD_SIZE+1)*sizeof(char));
    if((*paramList)[(*numOfMissingParams)] == NULL)
    {
        printf("Allocation failed! (Create Param List)\n");
        return 0;
    }
    strcpy((*paramList)[(*numOfMissingParams)],param);
    (*numOfMissingParams)++;
    return 1;
}
int createParamsLines(char** params, int numOfParams, Symbol* symbols, int numOfSymbols, int lineNum, Line** lines, int* numOfLines, int address, char*** missingParams, int* numOfMissingParams)
{
    int i=0, paramLoc, paramLineNumber = 1;;
    char** binResults = NULL, *binary = NULL;
    /*Check if there are 2 registers*/
    if(numOfParams == 2)
    {
        int reg1Val, reg2Val;
        reg1Val = checkIfRegister(params[0]);
        reg2Val = checkIfRegister(params[1]);
        if(reg1Val !=-1 && reg2Val !=-1)
        {
            createBinaryList(&binResults,paramLineNumber,reg1Val, 11, 6, 8);
            binary = decimalToBinary(reg2Val);
            strncpy(binResults[i]+9,binary + 11,3);
            createLinesList(lines,numOfLines,address, NULL, binResults[i]);
            address++;
            return paramLineNumber;
        }
    }
    for(i=0;i<numOfParams;i++)
    {
        /*Check if the parameter is register*/
        paramLoc = checkIfRegister(params[i]);
        if(paramLoc !=-1)
        {
            if(i==0)
                createBinaryList(&binResults,paramLineNumber,paramLoc,11, 6, 8);
            else
                createBinaryList(&binResults,paramLineNumber,paramLoc, 11, 9, 11);
            createLinesList(lines,numOfLines,address, NULL, binResults[paramLineNumber-1]);
            address++;
            paramLineNumber++;
        }
        /*Check if this is number*/
        else if(strncmp(params[i],"#",1) == 0)
        {
            int paramVal = atoi((params)[i]+1);
            if((paramVal !=0) || (strcmp((params)[i]+1, "0") == 0))
                createBinaryList(&binResults,paramLineNumber, paramVal,2, 0, 11);
            else
            {
                paramVal = getSymbolDefineValue((params)[i]+1,symbols,numOfSymbols, "mdefine");
                if(paramVal == -1)
                {
                    printf("\nIn line %d: the label %s is not exists!\n", lineNum, (params)[i]+1);
                    return 0;
                }
                createBinaryList(&binResults, paramLineNumber, symbols[paramVal].symbolValue, 2, 0, 11);
            }
            createLinesList(lines,numOfLines,address, NULL ,binResults[paramLineNumber-1]);
            address++;
            paramLineNumber++;

        }
        /*Handle data value*/
        else if(strstr(params[i],"[") != NULL)
        {
            char *index;
            char symbolName[MAX_WORD_SIZE];
            int j = 0, paramVal;
            while(params[i][j] != '[')
                j++;
            strncpy(symbolName,params[i],j);
            symbolName[j] = '\0';
            index = strstr(params[i],"[")+1;
            index[strlen(index)-1] = '\0';
            paramVal = getSymbolDefineValue(symbolName, symbols, numOfSymbols, "data");
            if(paramVal != -1)
            {
                createBinaryList(&binResults, paramLineNumber, symbols[paramVal].symbolValue, 2, 0, 11);
                if(strcmp(symbols[paramVal].symbolFeature, "external") == 0)
                    strcpy(binResults[paramLineNumber-1]+12,"01");
                else
                    strcpy(binResults[paramLineNumber-1]+12,"10");
            }
            else
            {
                createBinaryList(&binResults, paramLineNumber, 0, 2, 0, 11);
                binResults[paramLineNumber-1] = NULL;
                createMissingParamsList(symbolName, missingParams,numOfMissingParams);
            }
            createLinesList(lines,numOfLines,address, symbolName, binResults[paramLineNumber-1]);
            address++;
            paramLineNumber++;
            paramVal = atoi(index);
            if((paramVal !=0) || (strcmp(index, "0") == 0))
                createBinaryList(&binResults,paramLineNumber,paramVal,2, 0, 11);
            else
            {
                paramVal = getSymbolDefineValue(index,symbols,numOfSymbols, "mdefine");
                if(paramVal == -1)
                {
                    printf("\nIn line %d: the label %s is not exists!\n", lineNum, index);
                    return 0;
                }
                createBinaryList(&binResults, paramLineNumber, symbols[paramVal].symbolValue, 2, 0, 11);
            }
            createLinesList(lines,numOfLines,address, NULL, binResults[paramLineNumber-1]);
            address++;
            paramLineNumber++;
        }
        else
        {
            createBinaryList(&binResults, paramLineNumber, 0, 2, 0, 11);
            binResults[paramLineNumber-1] = NULL;
            createMissingParamsList(params[i], missingParams,numOfMissingParams);
            createLinesList(lines,numOfLines,address, params[i], binResults[paramLineNumber-1]);
            address++;
            paramLineNumber++;
        }
    }
    free(binResults);
    return paramLineNumber-1;
}
int checkTypeOfAddress(char* param, Symbol* symbols, int numOfSymbols)
{
    int indexOfSymbol;
    char* dataParam = NULL;
    removeSpaces(param, param);
    /*For data array option*/
    int i = 0;
    while(param[i] != '\0' && param[i] != '[')
        i++;
    if(param[i] == '[')
    {
        dataParam = (char*)malloc(i * sizeof(char));
        if(dataParam == NULL)
        {
            printf("Allocation failed! (Create Data Param)\n");
            return -1;
        }
        strncpy(dataParam, param, i);
        dataParam[i] = '\0';
    }
    indexOfSymbol = (dataParam == NULL) ? checkIfTheSymbolExists(symbols, numOfSymbols, param) : checkIfTheSymbolExists(symbols, numOfSymbols, dataParam);
    free(dataParam);
    /*CheckIf this is type 0 - Immediate addressing*/
    if((indexOfSymbol != -1 && strcmp(symbols[indexOfSymbol].symbolFeature, "define") == 0) || strncmp(param, "#",1) == 0)
        return 0;
    /*CheckIf this is type 2 - fixed index addressing*/
    if(strstr(param,"[") != NULL && strstr(param,"]") != NULL)
        return 2;
    /*CheckIf this is type 3 - Direct register addressing*/
    if(checkIfRegister(param) != -1)
        return 3;
    /*This is type 1 - Direct addressing*/
    return 1;
}
void changeValueForDataSymbols(Symbol* symbols, int symbolsSize, int addValue)
{
    int i = 0;
    while(i < symbolsSize)
    {
        if(strcmp(symbols[i].symbolFeature, "data") == 0)
            symbols[i].symbolValue += addValue;
        i++;
    }
}
int doFirstCycle(char* assemblerFileName, Symbol** symbols, int* numOfSymbols, Line** lines, int* numOfLines, char*** missingParams, int* numOfMissingParams, int* IC, int* DC)
{
    FILE *filep;
    char fileLine[MAX_LINE_SIZE];
    char line[MAX_LINE_SIZE];
    int fullNameSize = 0, L = 0, lineNum = 0, address = 100;
    char *fullName = NULL, *symbolName = NULL, *symbolValue = NULL;
    int thereIsErrors = 0;
    fullNameSize = strlen(assemblerFileName)+3; /*fileName +".am" length*/
    createNameWithExt(&fullName, assemblerFileName, fullNameSize, ".am");
    printf("FileName: %s\n",fullName);
    filep = fopen(fullName, "r");
    if (filep == NULL) {
        printf("File allocation failed!\n");
        exit(0);
    }

    /*Read the file line by line*/
    while (fgets(fileLine, sizeof(fileLine), filep)) {
        L = 0;
        arrangeStr(fileLine,line,0);
        /*Check if the line is mdefine define*/
        if(strstr(line, ".define") != NULL)
        {
            if(checkIfDefineIsValid(line,lineNum, &symbolName, *symbols, *numOfSymbols) == 0)
            {
                thereIsErrors = 1;
            }
            else
            {
                if(findSubString(line, '=', 0, 1, 1, &symbolValue) && createSymbolList(symbols, numOfSymbols, symbolName, "mdefine", atoi(symbolValue)) == 0)
                {
                    printf("Allocation failed! (First Cycle)\n");
                    freeInFirstCycle(&filep, &fullName, &symbolName, &symbolValue);
                    exit(0);
                }
            }
        }
        else
        {
            if(symbolName != NULL){
                free(symbolName);
                symbolName = NULL; // Set the pointer to NULL after freeing
            }
            symbolName = (char*)malloc((MAX_LABEL_SIZE + 1) * sizeof(char));
            if(symbolName == NULL)
            {
                printf("Allocation failed! (First Cycle)\n");
                freeInFirstCycle(&filep, &fullName, &symbolName, &symbolValue);
                exit(0);
            }
            /*Check if symbol is exists*/
            if(findSubString(line,':', 0, 1, 0, &symbolName))
            {
                if(strstr(line, ".string") != NULL || strstr(line, ".data") != NULL)
                {
                    int* dataValues = NULL;
                    int iter = 0, symbolLoc = -1;
                    symbolLoc = checkIfTheSymbolExists(*symbols, *numOfSymbols, symbolName);
                    if(symbolLoc != -1 && strcmp((*symbols)[symbolLoc].symbolFeature, "entry") == 1)
                    {
                        printf("\nIn line %d: The symbol %s is exists!\n", lineNum, symbolName);
                        thereIsErrors = 1;
                    }
                    else if(symbolLoc != -1 && strcmp((*symbols)[symbolLoc].symbolFeature, "entry") == 0)
                    {
                        (*symbols)[symbolLoc].symbolValue = *DC;
                    }
                    else
                    {
                        /*Add .string / .data to symbol list*/
                        if(createSymbolList(symbols, numOfSymbols, symbolName, "data", *DC) == 0)
                        {
                            printf("Allocation failed! (First Cycle)\n");
                            freeInFirstCycle(&filep, &fullName, &symbolName, &symbolValue);
                            exit(0);
                        }
                        if(strstr(line, ".string") != NULL)
                        {
                            if(handleStringStatement(line, &dataValues, &L) == 0)
                            {
                                printf("Allocation failed! (First Cycle)\n");
                                freeInFirstCycle(&filep, &fullName, &symbolName, &symbolValue);
                                exit(0);
                            }
                        }
                        else
                        {
                            if(handleDataStatement(line, lineNum, &thereIsErrors, &dataValues, &L, *symbols, *numOfSymbols) == 0)
                            {
                                printf("Allocation failed! (First Cycle)\n");
                                freeInFirstCycle(&filep, &fullName, &symbolName, &symbolValue);
                                exit(0);
                            }
                        }
                        while(iter<L)
                        {
                            char* binResult;
                            binResult = decimalToBinary(dataValues[iter]);
                            createLinesList(lines, numOfLines, address, NULL, binResult);
                            iter++;
                            address++;
                        }
                        *DC = *DC + L;
                        free(dataValues);
                    }
                }
                else
                {
                    char* binResult, **params = NULL;
                    int numOfParams, symbolLoc = -1;
                    symbolLoc = checkIfTheSymbolExists(*symbols, *numOfSymbols, symbolName);
                    if(symbolLoc != -1 && strcmp((*symbols)[symbolLoc].symbolFeature, "entry") == 1)
                    {
                        printf("\nIn line %d: The symbol %s is exists!\n", lineNum, symbolName);
                        thereIsErrors = 1;
                    }
                    else if(symbolLoc != -1 && strcmp((*symbols)[symbolLoc].symbolFeature, "entry") == 0)
                    {
                        (*symbols)[symbolLoc].symbolValue = *IC;
                    }
                    else
                    {
                        if(createSymbolList(symbols, numOfSymbols, symbolName, "code", *IC) == 0)
                        {
                            printf("Allocation failed! (First Cycle)\n");
                            freeInFirstCycle(&filep, &fullName, &symbolName, &symbolValue);
                            exit(0);
                        }
                        if(createCommandLines(&binResult, *symbols, *numOfSymbols, line, lineNum, 1, &params, &numOfParams))
                            thereIsErrors = 1;
                        createLinesList(lines, numOfLines, address, symbolName, binResult);
                        address++;
                        (*IC)++;
                        L = createParamsLines(params,numOfParams,*symbols,*numOfSymbols, lineNum, lines, numOfLines, address, missingParams, numOfMissingParams);
                        if(L == 0 && numOfParams != 0)
                        {
                            thereIsErrors = 1;
                        }
                        address = address + L;
                        *IC = *IC + L;
                        free(params);
                    }
                }
            }
            else if(strstr(line,".extern") != NULL) /*This is command from external file*/
            {
                findSubString(line,' ', 0, 1, 0, &symbolName);
                if(createSymbolList(symbols, numOfSymbols, symbolName, "external", -1) == 0)
                {
                    printf("Allocation failed! (First Cycle)\n");
                    freeInFirstCycle(&filep, &fullName, &symbolName, &symbolValue);
                    exit(0);
                }
            }
            else if(strstr(line, ".entry") != NULL) /*This is command from entry file*/
            {
                findSubString(line,' ', 0, 1, 0, &symbolName);
                if(createSymbolList(symbols, numOfSymbols, symbolName, "entry", -1) == 0)
                {
                    printf("Allocation failed! (First Cycle)\n");
                    freeInFirstCycle(&filep, &fullName, &symbolName, &symbolValue);
                    exit(0);
                }
            }
            else /*This is command!*/
            {
                char* binResult, **params = NULL;
                int numOfParams;
                if(createCommandLines(&binResult, *symbols, *numOfSymbols, line, lineNum, 0, &params, &numOfParams))
                    thereIsErrors = 1;
                createLinesList(lines, numOfLines, address, NULL, binResult);
                address++;
                (*IC)++;
                L = createParamsLines(params,numOfParams,*symbols,*numOfSymbols, lineNum, lines, numOfLines, address, missingParams, numOfMissingParams);
                if(L == 0 && numOfParams != 0)
                {
                    thereIsErrors = 1;
                }
                address = address + L;
                *IC = *IC + L;
                free(params);
            }
        }
        lineNum++;
    }
    if(thereIsErrors)
    {
        printf("There AM file contains incorrect words - the program will end at the Assembler first scanning!\n");
        return 0;
    }
    return 1;
}



