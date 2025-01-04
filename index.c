/* this code is written by Amit Mizrahi (207063876) the code opens a file includes text,
   it reads the text and sort it by words, after that it check the amount of the words appears on the text and which lines.
   the code allocated memory according to the text, row numbers, word amount and lines amount (struct wordIndex).
   if the file is not open beacuse of memory allocation failed or file is not exists it print an error message to the user and exit from the program*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1000

/* Structure to hold word and its row numbers */
struct wordIndex {
    char word[MAX_LENGTH];
    int *row_numbers;
    int count;
};

/* Function to compare WordIndex structures (for qsort later use) */
int checkWords(const void *first, const void *second) {
    return strcmp(((struct wordIndex *)first)->word, ((struct wordIndex *)second)->word);
}

/* Function to add a word to the index */
void addWordToIndex(struct wordIndex *index, int *indexSize, char *word, int rowNum) {
    /* Check if the word is already in the array of indexs */
    int i;
    for (i = 0; i < *indexSize; i++) {  /*loop to check if the word exists*/
        if (strcmp(index[i].word, word) == 0) {
            /* Resize the row numbers array */
            index[i].row_numbers = (int *)realloc(index[i].row_numbers, (index[i].count + 1) * sizeof(int));
            index[i].row_numbers[index[i].count++] = rowNum;
            return;
        }
    }

    /* Word not found in index, add it */
    strcpy(index[*indexSize].word, word);
    index[*indexSize].row_numbers = (int *)malloc(sizeof(int));/*allocates memory according to the word and row number*/
    index[*indexSize].row_numbers[0] = rowNum;
    index[*indexSize].count = 1;
    (*indexSize)++;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);/*prints file name*/
        return 1;
    }

    FILE *file = fopen(argv[1], "r");/*using FILE struct to perform activities on the required file*/
    if (file == NULL) {
        printf("Error opening file.\n");/*prints to user screen if the file is not appear on the folder or there are no access (w/r)*/
        return 1;
    }

    /* Allocate memory for the index */
    struct wordIndex *index = (struct wordIndex *)malloc(sizeof(struct wordIndex) * 10000);
    int indexSize = 0;

    char line[MAX_LENGTH];
    int rowNum = 1;

    /* Read file line by line */
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            /* Convert word to lowercase to avoid from multiple words identification */
            int i;
            for (i = 0; token[i]; i++) {
                if (token[i] >= 'A' && token[i] <= 'Z') {
                    token[i] = token[i] - 'A' + 'a'; 
                }
            }
            addWordToIndex(index, &indexSize, token, rowNum);
            token = strtok(NULL, " \t\n");
        }
        rowNum++;
    }

    fclose(file);

    /* Sort words in the index */
    qsort(index, indexSize, sizeof(struct wordIndex), checkWords);/*using qsort function from stdlib*/

    /* Output the index */
    int i, j;
    for (i = 0; i < indexSize; i++) {
        printf("%s:  ", index[i].word);
        if (index[i].count == 1) { /*checks the amount of count to identify which printf line to print single or multiple lines*/
            printf("appears in line %d", index[i].row_numbers[0]);
        } else {
            printf("appears in lines ");     
            for (j = 0; j < index[i].count - 1; j++) {
                printf("%d, ", index[i].row_numbers[j]);
            }
            printf("%d", index[i].row_numbers[j]);
        }
        printf("\n");
        free(index[i].row_numbers); /* Free memory allocated for row numbers */
    }

    free(index); /* Free memory allocated for the index */

    return 0;
}

