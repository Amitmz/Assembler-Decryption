#include "objects.h"
#ifndef FREE_HELPER_FUNCTIONS_H
#define FREE_HELPER_FUNCTIONS_H
void freeInPreCycle(FILE** filep, char** asFullName, char** amFullName, Macro** macros);
void freeInFirstCycle(FILE** filep, char** amFullName, char** symbolName, char** symbolValue);
#endif
