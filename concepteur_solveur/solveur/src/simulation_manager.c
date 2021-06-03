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

/*
void printAllKLengthRec(ResultArray *arr, char set[SIZE], char prefix[SIZE],
                        int nbTurrets, int nbTypes, int restriction, bool verbose) {


    // Base case: k is 0,
    // print prefix
    if (nbTypes == 0) {
        //printf("maybe %s\n", prefix);
        //printf("%d\n",countLetters((prefix)));
        //if (countLetters(prefix) != (restriction)) return;
        if (hasElementInArray(arr->array, arr->index, prefix))
            return;
        if (verbose) {
            printf("%s\n", prefix);
            //printf("nbTurrets = %d | cl(prefix) = %d\n", nbTurrets, countLetters(prefix));
        }
        strncpy(arr->array[arr->index], prefix, arr->stringSize);
        arr->index++;
        return;
    }

    // One by one add all characters
    // from set and recursively
    // call for k equals to k-1
    for (int i = 0; i < (nbTurrets); i++) {
        char newPrefix[SIZE] = {0};

        // Next character of input added
        strcat(newPrefix, prefix);
        strncat(newPrefix, &set[i], 1);


        // k is decreased, because
        // we have added a new character
        printAllKLengthRec(arr, set, newPrefix, nbTurrets, nbTypes - 1, restriction, verbose);
    }

}

void printAllKLength(ResultArray *arr, char set[SIZE], int nbTurrets, int nbTypes, int restriction, bool verbose) {
    return printAllKLengthRec(arr, set, "", nbTurrets, nbTypes, restriction, verbose);
}
*/

void printAllKLengthRec(ResultArray *arr, char set[SIZE], char prefix[SIZE], int k, int n, int restriction, bool verbose) {
    if (k == 0) {
        if (hasElementInArray(arr->array, arr->index, prefix))
            return;
        if (countLetters(prefix) != restriction)
            return;
        if (verbose) {
            printf("%s\n", prefix);
            //printf("nbTurrets = %d | cl(prefix) = %d\n", nbTurrets, countLetters(prefix));
        }
        strncpy(arr->array[arr->index], prefix, arr->stringSize);
        arr->index++;
        return;
    }

    for (int i = 0; i < n; i++) {
        char newPrefix[SIZE] = {0};

        // Next character of input added
        strcat(newPrefix, prefix);
        strncat(newPrefix, &set[i], 1);


        // k is decreased, because
        // we have added a new character
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

    /*
    for (int i = 0; i < blankSpaces; ++i) {
        turretTypesSet[4 + i] = '.';
    }
    */

    Battlefield *bf = getOptimalTurretPositions(grid, locations);

    displayGrid(grid);
    displayGrid(bf->grid);

    ResultArray combinationArray;
    combinationArray.globalSize = (int) pow(locations, locations);
    combinationArray.array = malloc(combinationArray.globalSize * sizeof(char*));
    combinationArray.stringSize = locations + 1;
    for (int i = 0; i < combinationArray.globalSize; ++i) {
        combinationArray.array[i] = malloc(combinationArray.stringSize);
    }
    //printAllKLength(&combinationArray, turretTypesSet, locations, strlen(turretTypesSet), turrets, true);
    int restriction = turrets;
    printAllKLength(&combinationArray, turretTypesSet, locations, strlen(turretTypesSet), restriction, false);

    for (int i = combinationArray.index; i < combinationArray.globalSize; ++i) {
        free(combinationArray.array[i]);
    }
    int successCount = 0;
    int minTicks = -1;
    for (int i = 0; i < combinationArray.index; ++i) {
        SimulationResult res = simulate(bf->grid, wave, false, combinationArray.array[i]);
        if (res.success) {
            printf("Success %s %d\n", combinationArray.array[i], res.ticks);
            if (minTicks == -1 || res.ticks < minTicks)
                minTicks = res.ticks;
            successCount++;
            int x = 12;
            char tmpGrid[GridSize][GridSize];
            //copyGrid(bf->grid, tmpGrid);
            //displayGridWithTurrets(tmpGrid, combinationArray.array[i]);
        }
    }
    printf("%d simulations successful over %d total simulations, (minimum ticks found: %d)", successCount, combinationArray.index, minTicks);

    return 0;
}









/*
int manage(char grid[GridSize][GridSize], Wave wave) {
    int nbTurrets = wave.gold / 10;
    displayGrid(grid);
    int blankSpaces = 2;
    if (nbTurrets <= 2)
        blankSpaces = 1;
    int locations = nbTurrets + blankSpaces;
    Battlefield *battlefield = getOptimalTurretPositions(grid, locations);
    puts("Hello");
    displayGrid(battlefield->grid);

    ResultArray *arr = malloc(sizeof(ResultArray));
    // do not change blankSpaces, otherwise restriction will never be reached
    // this bug cost 3 hours of debug
    const int nbTypes = 4 + blankSpaces;
    char *set = malloc(nbTypes+1);
    set[0] = 'R';
    set[1] = 'H';
    set[2] = 'I';
    set[3] = 'F';
    for (int i = 0; i < blankSpaces; ++i) {
        set[i + 4] = '.';
    }
    //char set[] = {'R', 'H', 'I', 'F', '.', '.', '.'};
    arr->globalSize = (int) pow((double) nbTypes, (double) (nbTypes));
    arr->array = malloc(arr->globalSize * sizeof(char *));
    arr->stringSize = nbTurrets + blankSpaces + 1;
    arr->index = 0;
    for (int i = 0; i < arr->globalSize; ++i) {
        arr->array[i] = malloc(arr->stringSize);
        for (int j = 0; j < arr->stringSize; ++j) {
            arr->array[i][j] = 0;
        }
    }

    bool verbose = true;
    printAllKLength(arr, set, nbTypes, nbTypes, nbTurrets, verbose);

    printf("%d possible combinations found\n", arr->index);

    //return;

    //return -1;
    //displayArray(arr->array, arr->globalSize);
    //printf("\nHi?\n");

    int successCount = 0;
    for (int i = 0; i < arr->index; ++i) {
        SimulationResult res = simulate(battlefield->grid, wave, false, arr->array[i]);
        if (res.success) {
            char* arrangement = arr->array[i];
            printf("%s - %s - %d\n", arrangement, res.success ? "SUCCESS" : "FAILURE", res.ticks);

            char tmpGrid[GridSize][GridSize];
            copyGrid(battlefield->grid, tmpGrid);
            for (int j = 0; j < battlefield->turretAmount; ++j) {
                Cursor c = battlefield->turretPositions[j];
                tmpGrid[c.x][c.y] = arr->array[i][j];
            }

            //displayGrid(battlefield->grid);
            displayGridWithTurrets(tmpGrid, arrangement);
            successCount++;
        }
    }

    printf("SUCCESS COUNT = %d", successCount);
    return successCount;

}
 */