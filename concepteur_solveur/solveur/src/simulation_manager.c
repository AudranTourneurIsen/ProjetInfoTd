#include "simulation.h"
#include "fonctions.h"
#include <math.h>

#define SIZE 16

typedef struct ResultArray {
    char **array;
    int globalSize;
    int stringSize;
    int index;
} ResultArray;

int countLetters(char* str) {
    int count = 0;
    int i = 0;
    while (*(str + i) != 0) {
        char currentChar = *(str + i);
        if (currentChar >= 'A' && currentChar <= 'Z') {
            count++;
        }
        i++;
    }
    return count;
 }

 bool hasElementInArray(char* array[], int size, char* testStr) {
     for (int i = 0; i < size; ++i) {
         if (strcmp(array[i], testStr) == 0) {
            return true;
         }
     }
     return false;
}

void printAllKLengthRec(ResultArray *arr, char set[SIZE], char prefix[SIZE], int k, int n, int restriction, bool verbose) {
    if (k == 0) {
        if (hasElementInArray(arr->array, arr->index, prefix))
            return;
        if (countLetters(prefix) != restriction)
            return;
        if (verbose) {
            printf("%s\n", prefix);
        }
        strncpy(arr->array[arr->index], prefix, arr->stringSize);
        arr->index++;
        return;
    }

    for (int i = 0; i < n; i++) {
        char newPrefix[SIZE] = {0};

        strcat(newPrefix, prefix);
        strncat(newPrefix, &set[i], 1);

        printAllKLengthRec(arr, set, newPrefix, k - 1 , n, restriction, verbose);
    }

}

void printAllKLength(ResultArray *arr, char set[SIZE], int k, int n, int restriction, bool verbose) {
    return printAllKLengthRec(arr, set, "", k, n, restriction, verbose);
}


void displayArray(char *arr[], int size) {
    for (int i = 0; i < size; ++i) {
        //printf("%s\n", arr[i]);
    }
}

int manage(char grid[GridSize][GridSize], Wave wave) {
    int turrets = wave.gold / 10;
    int blankSpaces = 3;
    if (turrets >= 5) {
        blankSpaces = 2;
    }

    int locations = turrets + blankSpaces;

    char* turretTypesSet = malloc(4 + blankSpaces + 1);
    turretTypesSet[0] = 'R';
    turretTypesSet[1] = 'H';
    turretTypesSet[2] = 'I';
    turretTypesSet[3] = 'F';
    turretTypesSet[4] = '.';

    Battlefield *bf = getOptimalTurretPositions(grid, locations);

    //displayGrid(grid);
    //displayGrid(bf->grid);

    ResultArray combinationArray;
    combinationArray.globalSize = (int) pow(locations, locations);
    combinationArray.array = malloc(combinationArray.globalSize * sizeof(char*));
    combinationArray.stringSize = locations + 1;
    for (int i = 0; i < combinationArray.globalSize; ++i) {
        combinationArray.array[i] = malloc(combinationArray.stringSize);
    }
    int restriction = turrets;
    printAllKLength(&combinationArray, turretTypesSet, locations, strlen(turretTypesSet), restriction, false);

    for (int i = combinationArray.index; i < combinationArray.globalSize; ++i) {
        free(combinationArray.array[i]);
    }
    int successCount = 0;
    int minTicks = -1;
    char* minSol = malloc(combinationArray.stringSize);
    for (int i = 0; i < combinationArray.index; ++i) {
        SimulationResult res = simulate(bf->grid, wave, false, combinationArray.array[i]);
        if (res.success) {
            //printf("Success %s %d\n", combinationArray.array[i], res.ticks);
            if (minTicks == -1 || res.ticks < minTicks) {
                minTicks = res.ticks;
                minSol = combinationArray.array[i];
            }
            successCount++;
        }
    }

    if (successCount == 0) {
        printf("NOTHING");
    }
    else {
        displayGridWithTurrets(bf->grid, minSol);
        printf("%d simulations successful over %d total simulations, (minimum ticks found: %d) (Solution = %s)\n", successCount, combinationArray.index, minTicks, minSol);
    }

    return 0;
}