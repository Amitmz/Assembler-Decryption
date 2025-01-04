#ifndef OBJECTS_H
#define OBJECTS_H
#include "constants.h"

typedef struct macro{
  char* McrName;
  char* Content;
} Macro;

typedef struct symbol{
  char* symbolName;
  char* symbolFeature;
  int symbolValue;
} Symbol;

typedef struct line{
    int address;
    char* param;
    char* binaryResult;
}Line;

#endif
