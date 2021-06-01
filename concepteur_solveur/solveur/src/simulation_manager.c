#include "simulation.h"
#include "fonctions.h"
#include <math.h>

#define SIZE 16

typedef struct ResultArray {
    char** array;
    int globalSize;
    int stringSize;
    int index;
} ResultArray;

void printAllKLengthRec(ResultArray* arr, char set[SIZE], char prefix[SIZE], int n, int k)
{

    // Base case: k is 0,
    // print prefix
    if (k == 0)
    {
        //printf("%s\n", prefix);
        strncpy(arr->array[arr->index], prefix, arr->stringSize);
        arr->index++;
        return;
    }

    // One by one add all characters
    // from set and recursively
    // call for k equals to k-1
    for (int i = 0; i < n; i++)
    {
        char newPrefix[SIZE] = {0};

        // Next character of input added
        strcat(newPrefix, prefix);
        strncat(newPrefix, &set[i], 1);


        // k is decreased, because
        // we have added a new character
        printAllKLengthRec(arr, set, newPrefix, n, k - 1);
    }

}

void printAllKLength(ResultArray *arr, char set[SIZE], int k,int n)
{
    return printAllKLengthRec(arr, set, "", n, k);
}

void displayArray(char* arr[], int size) {
    for (int i = 0; i < size; ++i) {
        //printf("%s\n", arr[i]);
    }
}

int manage(char grid[GridSize][GridSize], Wave wave) {
    ResultArray* arr = malloc(sizeof(ResultArray));
    char set[] = {'R', 'H', 'I', 'F'};
    int nbTypes = sizeof(set);
    int nbLocations = wave.gold/10;
    int nbTurrets = wave.gold/10;
    arr->globalSize = (int) pow((double) nbTypes, (double) nbTurrets);
    arr->array = malloc(arr->globalSize * sizeof(char*));
    arr->stringSize = nbTurrets + 1;
    arr->index = 0;
    for (int i = 0; i < arr->globalSize; ++i) {
        arr->array[i] = malloc(arr->stringSize);
        for (int j = 0; j < arr->stringSize; ++j) {
            arr->array[i][j] = 0;
        }
    }
    printAllKLength(arr, set, nbTurrets, nbTypes);
    //displayArray(arr->array, arr->globalSize);
    //printf("\nHi?\n");

    int successCount = 0;
    for (int i = 0; i < arr->globalSize; ++i) {
        bool res = simulate(grid, wave, false, arr->array[i]);
        if (res) {
            printf("%s - %s\n", arr->array[i], res ? "SUCCESS" : "FAILURE");
            successCount++;
        }
    }

    printf("COUNT = %d", successCount);
    return successCount;

}