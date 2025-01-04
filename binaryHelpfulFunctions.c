#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "constants.h"
#include "helpfulFunctions.h"
void MSBForNegativeNumbers(char* binaryArr)
{
    int i=0;
    while(i < BINARY_CODE_SIZE)
    {
        binaryArr[i] = (binaryArr[i] == '0') ? '1' : '0';
        i++;
    }
}
char* decimalToBinary(int decimalNum)
{
    int decimal = decimalNum;
    int i = 0, j = 0, negative = 0;
    char* binaryArr = (char*)malloc((BINARY_CODE_SIZE + 1) * sizeof(char)); // Allocate memory for the binary array
    if (binaryArr != NULL && decimal<= 16383) {
        // Initialize the binary array with zeros
        for (j = 0; j < BINARY_CODE_SIZE; j++) {
            binaryArr[j] = '0';
        }
        if(decimal < 0)
        {
            decimal = decimal * (-1);
            negative = 1;
        }
        while (decimal > 0) {
            binaryArr[i] = (decimal % 2) + '0'; // Convert the remainder to character '0' or '1'
            decimal = decimal / 2;
            i++;
        }
        if(negative)
            MSBForNegativeNumbers(binaryArr);
        swipe(binaryArr, BINARY_CODE_SIZE);
        binaryArr[BINARY_CODE_SIZE] = '\0'; // Add null terminator
    } else {
        printf("Allocation failed! (Decimal to Binary)\n");
        return NULL;
    }
    return binaryArr;
}

char* fourToCode(char* binaryArray)
{
    // Allocate memory for the output string
    char* code = (char*)malloc((FOUR_CODE_SIZE + 1) * sizeof(char));

    // Check if memory allocation was successful
    if (code == NULL) {
        printf("Allocation failed (Four to Code)\n");
        exit(1);
    }

    // Convert binary array to a decimal number
    int decimal = 0;
    for (int i = 0; i < BINARY_CODE_SIZE; i++) {
        decimal = (decimal << 1) | (binaryArray[i]-'0');
    }

    // Convert decimal number to base-4 representation
    for (int i = FOUR_CODE_SIZE - 1; i >= 0; i--) {
        int digit = decimal % 4;
        switch (digit) {
            case 0:
                code[i] = '*';
                break;
            case 1:
                code[i] = '#';
                break;
            case 2:
                code[i] = '%';
                break;
            case 3:
                code[i] = '!';
                break;
        }
        decimal /= 4;
    }

    // Add null terminator to the output string
    code[FOUR_CODE_SIZE] = '\0';

    // Return the dynamically allocated string
    return code;
}
