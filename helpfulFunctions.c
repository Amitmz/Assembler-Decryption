#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <ctype.h>
#include "helpfulFunctions.h"
#include "constants.h"
#include "Objects.h"
void createNameWithExt(char** fullName, char* fileName, int nameSize, char* extention)
{
    *fullName = (char*)malloc(nameSize*sizeof(char));
    if(fullName == NULL)
    {
        printf("Allocation failed! (Create File Name)\n");
        exit(0);
    }
    strcpy(*fullName,fileName);
    strcat(*fullName,extention);
}

void arrangeStr(char *input, char *output, int removeAll)
{
    int firstTime = removeAll;
    while (*input) {
        if (*input != ' ' && *input != '\t' && *input != '\n') {
            *output = *input;
            output++;
            firstTime = 1;
        }
        else if(*input == ' ' && firstTime)
        {
            *output = *input;
            output++;
            firstTime = removeAll;
        }
        else if(*input == '\t' && firstTime)
        {
            *output = ' ';
            output++;
            firstTime = removeAll;
        }
        input++;
    }
    *output = '\0';
}
void removeSpaces(char *input, char *output)
{
    int i = 0, j=0;
    while(input[i] !='\0')
    {
        if(input[i] != ' ')
        {
            input[j] = input[i];
            j++;
        }
        i++;
    }
    input[j] = '\0';
}
char* returnOpcodePerAction(char* action)
{
    if(strcmp("mov",action) == 0)
        return "0000";
    if(strcmp("cmp",action) == 0)
        return "0001";
    if(strcmp("add",action) == 0)
        return "0010";
    if(strcmp("sub",action) == 0)
        return "0011";
    if(strcmp("not",action) == 0)
        return "0100";
    if(strcmp("clr",action) == 0)
        return "0101";
    if(strcmp("lea",action) == 0)
        return "0110";
    if(strcmp("inc",action) == 0)
        return "0111";
    if(strcmp("dec",action) == 0)
        return "1000";
    if(strcmp("jmp",action) == 0)
        return "1001";
    if(strcmp("bne",action) == 0)
        return "1010";
    if(strcmp("red",action) == 0)
        return "1011";
    if(strcmp("prn",action) == 0)
        return "1100";
    if(strcmp("jsr",action) == 0)
        return "1101";
    if(strcmp("rts",action) == 0)
        return "1110";
    if(strcmp("hlt",action) == 0)
        return "1111";
    printf("\nThe action %s is not a valid action!", action);
    return "";
}
int checkIfTheSymbolExists(Symbol* symbols, int amountOfSymbols, char* symbol)
{
    int i;
    for(i=0;i<amountOfSymbols; i++)
    {
        if(!strcmp(symbols[i].symbolName, symbol))
           return i;
    }
    return -1;
}

int numberOfOperandsPerAction(char* action)
{
    if(strcmp("mov", action) == 0 || strcmp("cmp", action) == 0 || strcmp("add", action) == 0
       || strcmp("sub", action) == 0 || strcmp("lea", action) == 0)
        return 2;
    if(strcmp("not", action) == 0 || strcmp("clr", action) == 0 || strcmp("inc", action) == 0
       || strcmp("dec", action) == 0 || strcmp("jmp", action) == 0 || strcmp("bne", action) == 0
       || strcmp("red", action) == 0 || strcmp("prn", action) == 0 || strcmp("jsr", action) == 0)
        return 1;
    if(strcmp("rts", action) == 0 || strcmp("hlt", action) == 0)
        return 0;
    return -1;
}

int checkIfCommand(char* name)
{
    if(!strcmp(name, "mov") || !strcmp(name, "cmp") || !strcmp(name, "add") || !strcmp(name, "sub")
       || !strcmp(name, "not") || !strcmp(name, "clr") || !strcmp(name, "lea") || !strcmp(name, "inc")
       || !strcmp(name, "dec") || !strcmp(name, "jmp") || !strcmp(name, "bne") || !strcmp(name, "red")
       || !strcmp(name, "prn") || !strcmp(name, "jsr") || !strcmp(name, "rts") || !strcmp(name, "hlt"))
    {
        return 1;
    }
    return 0;
}

int checkIfRegister(char* name)
{
        if(strcmp("r0", name) == 0)
            return 0;
        if(strcmp("r1", name) == 0)
            return 1;
        if(strcmp("r2", name) == 0)
            return 2;
        if(strcmp("r3", name) == 0)
            return 3;
        if(strcmp("r4", name) == 0)
            return 4;
        if(strcmp("r5", name) == 0)
            return 5;
        if(strcmp("r6", name) == 0)
            return 6;
        if(strcmp("r7", name) == 0)
            return 7;
        return -1;
}

int checkIfTheNameValid(char* name)
{
    if((checkIfRegister(name) == -1) && checkIfCommand(name) == 0)
        return 1;
    return 0;
}

int findNextString(char* line, int isDefine, char** symbolName)
{
    int start, end;
    start = (isDefine ? strlen(".%define") : 0);
    while(isspace(line[start]))
        start++;
    end = start;
    while(line[end] != '\0' && line[end] != '\n' && !isspace(line[end]))
    {
        end++;
    }
    if(line[end] == '\0' || line[end] == '\n')
    {
         *symbolName = NULL;
    }
    else
    {
        if(*symbolName != NULL){
            free(*symbolName);
            *symbolName = NULL; // Set the pointer to NULL after freeing
        }
        *symbolName = (char*)malloc((end-start+1)*sizeof(char));
        if(*symbolName == NULL)
        {
            printf("Allocation failed!\n");
            return 0;
        }
        strncpy(*symbolName, line+start, end-start);
        (*symbolName)[end-start]='\0';
    }
    return 1;
}

int findSubString(char* str, char sep, int start, int last, int after, char** result)
{
    int i = 0;
    if(after)
    {
        *result = strchr(str + start, sep);
        if(**result == '\0')
            return 0;
        *result = *result+1;
    }
    else
    {
       while(str[i] != sep && str[i] != '\0')
       {
           i++;
       }
       if(str[i] == '\0')
       {
            return 0;
       }
       strncpy(*result,str,(i+1));
       (*result)[i+1] = '\0';
    }
    arrangeStr(*result,*result,0);
    (*result)[strlen(*result)-1] = '\0';
    if(!last)
        findNextString(*result, 0, result);
    return 1;
}
int getSymbolDefineValue(char* symbol, Symbol* symbols, int numOfSymbols, char* type)
{
    int i;
    for(i=0;i<numOfSymbols; i++)
    {
        if(!strcmp(symbols[i].symbolName, symbol) && !strcmp(symbols[i].symbolFeature, type))
        {
            return i;
        }
    }
    return -1;
}
void createFile(char* fileName, char* content)
{
    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(0);
    }

    if (fputs(content, file) == EOF) {
        perror("Error writing to file");
        exit(0);
    }

    fclose(file);

    printf("File '%s' created and content saved successfully.\n", fileName);
}

int createContentForFiles(char** content, int address, char* symbolName, int addressFirst)
{
    char* strAddress;
    int size, locForCopy = 0;
    strAddress = intToString(address);
    size = (strlen(strAddress) + strlen(" ") + strlen(symbolName) + strlen("\n"));
    if(*content == NULL)
    {
        *content = (char*)malloc(size * sizeof(char));
        if(*content == NULL)
        {
            printf("Allocation failed! (Create Content for Files)\n");
            return 0;
        }
    }
    else
    {
        locForCopy = strlen(*content)+1;
        size+=locForCopy;
        *content = (char*)realloc(*content, (size + strlen(*content))*sizeof(char));
        if(*content == NULL)
        {
             printf("Allocation failed! (Create Content for Files)\n");
            return 0;
        }
    }
    if(addressFirst)
    {
        strcat((*content), strAddress);
        strcat((*content), " ");
        strcat((*content), symbolName);
    }
    else
    {
        strcat((*content), symbolName);
        strcat((*content), " ");
        strcat((*content), strAddress);
    }
    strcat((*content), "\n");
    return 1;
}

char* intToString(int num)
{
    int i, length = 0, tempNum = num;
    char* strNum = NULL;
    if (num == 0)
        length =  1;

    while (tempNum != 0) {
        length++;
        tempNum /= 10;
    }

    strNum = (char*)malloc((length+1) * sizeof(char));
    if(strNum == NULL)
    {
        printf("Allocation failed! (Convert int to string)\n");
        return NULL;
    }

    tempNum = num;
    for(i=0; i<length+1; i++)
    {
        strNum[i] = tempNum%10+'0';
        tempNum/=10;
    }
    swipe(strNum, length);
    strNum[length] = '\0';
    return strNum;
}

int splitBySep(char* str, char* sep, char*** dataValues, int* dataSize)
{
    char* token;
    (*dataSize) = 0;
    token = strtok(str, sep);
    /* walk through other tokens */
    while( token != NULL ) {
        (*dataSize)++;
        arrangeStr(token,token,1);
        if(*dataValues == NULL)
        {
            *dataValues = (char**)malloc((*dataSize)*sizeof(char*));
            if(*dataValues == NULL)
            {
                printf("Allocation failed!\n");
                return 0;
            }
        }
        else
        {
            *dataValues = (char**)realloc(*dataValues,(*dataSize)*sizeof(char*));
            if(*dataValues == NULL)
            {
                printf("Allocation failed!\n");
                return 0;
            }
        }
        (*dataValues)[*dataSize-1] = (char*)malloc((strlen(token))*sizeof(char));
        if((*dataValues)[*dataSize-1] == NULL)
        {
            printf("Allocation failed!\n");
            return 0;
        }
        strcpy((*dataValues)[*dataSize-1],token);
        token = strtok(NULL, sep);
    }
    return 1;
}
int handleStringStatement(char* line, int** dataValues, int* dataSize)
{
    int i=1;
    char* strVal = strstr(line, ".string");
    strVal = strVal + strlen(".string");
    arrangeStr(strVal, strVal, 0);
    while(strVal[i] != '\0')
    {
        if(strVal[i] >= 0 && strVal[i] <= 127)
        {
            (*dataSize)++;
            if(*dataValues == NULL)
            {
                *dataValues = (int*)malloc((*dataSize)*sizeof(int));
                if(*dataValues == NULL)
                {
                    printf("Allocation failed!\n");
                    return 0;
                }
            }
            else
            {
                *dataValues = (int*)realloc(*dataValues,(*dataSize)*sizeof(int));
                if(*dataValues == NULL)
                {
                    printf("Allocation failed!\n");
                    return 0;
                }
            }
            (*dataValues)[*dataSize-1] = (strVal[i] == ' ')? 0 : strVal[i];
        }
        i++;
    }
    return 1;
}
int handleDataStatement(char* line, int lineNum, int* errors, int** dataValues, int* dataSize, Symbol* symbols, int symbolSize)
{
    char** dataCharValues = NULL;
    int i = 0, j = 0;
    char* strVal = strstr(line, ".data");
    strVal = strVal + strlen(".data");
    arrangeStr(strVal, strVal, 0);

    if(splitBySep(strVal, ",", &dataCharValues, dataSize) == 0)
        return 0;

    *dataValues = (int*)malloc((*dataSize)*sizeof(int));
    if(*dataValues == NULL)
    {
        printf("Allocation failed!\n");
        return 0;
    }

    for(i=0; i<(*dataSize); i++)
    {
        int atoiVal = atoi(dataCharValues[i]);
        if((atoiVal !=0) || (strcmp(dataCharValues[i], "0") == 0))
        {
            (*dataValues)[i] = atoiVal;
        }
        else
        {
            removeSpaces(dataCharValues[i], dataCharValues[i]);
            for(j=0;j<symbolSize; j++)
            {
                if(!strcmp(symbols[j].symbolName, dataCharValues[i]) && !strcmp(symbols[j].symbolFeature, "mdefine"))
                {
                    (*dataValues)[i] = symbols[j].symbolValue;
                    break;
                }
            }
            if(j == symbolSize)
            {
                printf("\nIn line %d: the label %s is not exists!\n", lineNum, dataCharValues[i]);
                (*errors) = 1;
            }
        }
    }
    return 1;
}

int createLinesList(Line** lines, int* numOfLines, int address, char* param, char* binValue)
{
    if(*lines == NULL)
    {
        *lines = (Line*)malloc(sizeof(Line));
        if(*lines == NULL)
        {
            printf("Allocation failed! (Create Line List)\n");
            return 0;
        }
    }
    else
    {
        *lines = (Line*)realloc(*lines, (*numOfLines+1) * sizeof(Line));
        if(*lines == NULL)
        {
            printf("Allocation failed! (Create Line List)\n");
            return 0;
        }
    }
    (*lines)[*numOfLines].address = address;
    (*lines)[*numOfLines].param = (char*)malloc((MAX_WORD_SIZE+1) * sizeof(char));
    if((*lines)[*numOfLines].param == NULL)
    {
        printf("Allocation failed! (Create Line List)\n");
        return 0;
    }
    if(param == NULL)
        (*lines)[*numOfLines].param = NULL;
    else
        strcpy((*lines)[*numOfLines].param,param);
    (*lines)[*numOfLines].binaryResult = (char*)malloc((BINARY_CODE_SIZE+1) * sizeof(char));
    if((*lines)[*numOfLines].binaryResult == NULL)
    {
        printf("Allocation failed! (Create Line List)\n");
        return 0;
    }
    if(binValue == NULL)
        (*lines)[*numOfLines].binaryResult = NULL;
    else
        strcpy((*lines)[*numOfLines].binaryResult,binValue);
    *numOfLines = (*numOfLines) + 1;
    return 1;
}
void swipe(char* str, int size) {
    int temp = 0;
    int count = 0;
    int i;
    for (i = size - 1; i >= size / 2; i--) {
        temp = str[i];
        str[i] = str[count];
        str[count] = temp;
        count++;
    }
}
