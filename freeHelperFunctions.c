#include <stdio.h>
#include <stdlib.h>
#include "freeHelperFunctions.h"
void freeInPreCycle(FILE** filep, char** asFullName, char** amFullName, Macro** macros)
{
    fclose(*filep);
    free(*asFullName);
    free(*amFullName);
    free(*macros);
}

void freeInFirstCycle(FILE** filep, char** amFullName, char** symbolName, char** symbolValue)
{
    fclose(*filep);
    free(*amFullName);
    free(*symbolName);
    free(*symbolValue);
}
