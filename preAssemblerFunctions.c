#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "objects.h"
#include "helpfulFunctions.h"
#include "preAssemblerFunctions.h"
#include "freeHelperFunctions.h"

void createMacro(Macro* macro, char* macroName, int macroLoc)
{
    macro[macroLoc].McrName = macroName;
    macro[macroLoc].Content = NULL;
}

int findMacroName(char* macroLine, char** macroName)
{
    int i = 0;
    while(macroLine[i] != '\0' && macroLine[i] != '\n' && macroLine[i] != ' ')
    {
        i++;
    }

    if(macroLine[i] == '\0' || macroLine[i] == '\n')
    {
         *macroName = NULL;
    }
    else
    {
        *macroName = (char*)malloc(strlen(macroLine+i+1)*sizeof(char));
        if(*macroName == NULL)
        {
            printf("Allocation failed! (MacroName)");
            return 0;
        }
        strcpy(*macroName, macroLine+i+1);
    }
     return 1;
}

int checkMacroNameExists(Macro* macros, int macrosSize, char* macroName, Macro* thisMacro)
{
        int i;
        for(i=0;i<macrosSize;i++)
        {
            if(strcmp(macros[i].McrName,macroName) == 0)
            {
                *thisMacro = macros[i];
                return 1;
            }
        }
        return 0;
}

int hundleMacro(char* line, Macro** macros, int macrosSize)
{
    Macro thisMacro;
    char* macroName;
    if(findMacroName(line, &macroName) == 0)
        return 0;
    if(*macros == NULL)
    {
        *macros = (Macro*)malloc(sizeof(Macro));
        if(*macros == NULL)
        {
            printf("Allocation failed! (Malloc Macro)");
            return 0;
        }
        createMacro(*macros, macroName, 0);
    }
    else
    {
        if(checkMacroNameExists(*macros,macrosSize,macroName,&thisMacro))
        {
            printf("Macro '%s' Exists!", macroName);
            return 0;
        }
        *macros = realloc(*macros,(1+macrosSize)*sizeof(Macro));
        if(*macros == NULL)
        {
            printf("Allocation failed! (Realloc Macro)");
            return 0;
        }
        createMacro(*macros, macroName, macrosSize);
    }
    return 1;
}

int addContentToMacro(Macro* macros, int macroLoc, char* content)
{
    int contentSize = 0;
    char* mContent;
    mContent = macros[macroLoc].Content;
    if(mContent == NULL)
    {
        macros[macroLoc].Content = (char*)malloc(strlen(content)*sizeof(char));
        if(macros[macroLoc].Content == NULL)
        {
            printf("Allocation failed! (Malloc Macro Content)");
            return 0;
        }
        strcpy(macros[macroLoc].Content, content);
    }
    else
    {
        contentSize = strlen(macros[macroLoc].Content) + strlen(content);
        macros[macroLoc].Content = realloc(macros[macroLoc].Content, contentSize*sizeof(char));
        if(macros[macroLoc].Content == NULL)
        {
            printf("Allocation failed! (realloc Macro Content)");
            return 0;
        }
        strcat(macros[macroLoc].Content, content);
    }
    return 1;
}

int addContentToFileContent(char** fileContent, char* content)
{
    int contentSize, fileSize = 0;
    contentSize = strlen(content);
    if(*fileContent == NULL)
    {
        *fileContent = (char*)malloc(contentSize*sizeof(char));
        if(*fileContent == NULL)
        {
            printf("Allocation failed! (Malloc Content)");
            return 0;
        }
        strcpy(*fileContent,content);
    }
    else
    {
        fileSize = strlen(*fileContent);
        *fileContent = realloc(*fileContent, (fileSize+contentSize) * sizeof(char));
        if(*fileContent == NULL)
        {
            printf("Allocation failed! (Realloc Content)");
            return 0;
        }
        (*fileContent)[fileSize-1] = '\0';
        strcat(*fileContent,"\n");
        strcat(*fileContent,content);
    }
    return 1;
}


/*This function read assembler file, find the macros, replace the macros and add the macros contents and create am file.*/
/*Return the am file content*/
void createAMFile(char* assemblerFileName, char* contentFile)
{
    FILE *filep;
    Macro thisMacro;
    int findMCR = 0, fullNameSize, macrosSize = 0;
    char *asFullName = NULL, *amFullName = NULL;
    Macro* macros = NULL;
    char fileLine[MAX_LINE_SIZE];
    char line[MAX_LINE_SIZE];

    fullNameSize = strlen(assemblerFileName)+3; /*fileName + ".as"/".am" length*/
    createNameWithExt(&asFullName, assemblerFileName, fullNameSize, ".as");

    printf("FileName: %s\n",asFullName);
    filep = fopen(asFullName, "r");
    if (filep == NULL) {
        printf("File allocation failed!");
        exit(0);
    }

    /*Read the file line by line*/
    while (fgets(fileLine, sizeof(fileLine), filep)) {
        arrangeStr(fileLine,line,0);
        /*check if the line start with mcr (THIS IS MACRO)*/
        if(checkMacroNameExists(macros,macrosSize,line, &thisMacro))
        {
            if(addContentToFileContent(&contentFile, thisMacro.Content) == 0)
            {
                freeInPreCycle(&filep,&asFullName,&amFullName,&macros);
                return;
            }
        }
        else if(strstr(line, "endmcr") != NULL)
        {
            findMCR = 0;
            macrosSize++;
        }
        else if(strstr(line, "mcr") != NULL)
        {
            findMCR = 1;
            if(hundleMacro(line,&macros,macrosSize) == 0)
            {
                freeInPreCycle(&filep,&asFullName,&amFullName,&macros);
                return;
            }
        }
        else
        {
            if(findMCR)
            {
                if(addContentToMacro(macros, macrosSize, fileLine) == 0)
                {
                    freeInPreCycle(&filep,&asFullName,&amFullName,&macros);
                    return;
                }
            }
            else
            {
                if(addContentToFileContent(&contentFile, fileLine) == 0)
                {
                    freeInPreCycle(&filep,&asFullName,&amFullName,&macros);
                    return;
                }
            }

        }
    }
    createNameWithExt(&amFullName, assemblerFileName, fullNameSize, ".am");
    createFile(amFullName, contentFile);
    /*Create .am file*/
    freeInPreCycle(&filep,&asFullName,&amFullName,&macros);
}
