#include "fonctions.h"
#include "TerminalUtils.h"
#include "simulation.h"
#include "simulation_manager.h"

#define WaveAmount 64
Wave Waves[WaveAmount] = {};

void displayPositions(Cursor *cursors, int size) {
    for (int i = 0; i < size; i++) {
        Cursor cursor = cursors[i];
        printf("Position %d - [%d/%d]\n", i, cursor.x, cursor.y);
    }
}

void updateGrid(char grid[GridSize][GridSize]) {
    for (int i = 0; i < GridSize; i++) {
        for (int j = 0; j < GridSize; j++) {
            if (grid[j][i] == '#')
                grid[j][i] = NOTHING;
            if (grid[j][i] == '@')
                grid[j][i] = PATH;
            if (grid[j][i] == '$')
                grid[j][i] = TURRET;
        }
    }
}

void initWaves() {
    FILE *file;
    file = fopen("waves.txt", "r");

    if (file == NULL) {
        puts("waves.txt not found");
        return;
    }

    char txt[stringSIZE] = {0};

    int waveIndex = 0;
    while (!feof(file)) {
        char lineBuffer[stringSIZE] = {0};
        fgets(lineBuffer, stringSIZE, file);
        strcat(txt, lineBuffer);
        if (ferror(file)) {
            fprintf(stderr, "Reading error\n");
            break;
        }
        //printf(lineBuffer);

        bool goldParsed = false;
        char goldStr[8] = {0};
        int i = 0;
        char tmpEnemies[256] = {0};
        int enemyIndex = 0;
        while (*(lineBuffer + i) != 0) {
            if (lineBuffer[i] == ',') {
                if (!goldParsed) {
                    goldParsed = true;
                }
                i++;
                continue;
            }
            if (!goldParsed) {
                goldStr[i] = lineBuffer[i];
            } else if (lineBuffer[i] != '\n' && lineBuffer[i] != '\r') {
                tmpEnemies[enemyIndex] = lineBuffer[i];
                enemyIndex++;
            }
            i++;
        }
        int goldValue = atoi(goldStr);
        //Wave w = {waveIndex, goldValue, tmpEnemies};
        //printf("%s", tmpEnemies);
        Waves[waveIndex].index = waveIndex;
        Waves[waveIndex].gold = goldValue;
        strcpy(Waves[waveIndex].enemies, tmpEnemies);
        waveIndex++;
        //printf("g %d\n", goldValue);
    }
    //printf(txt);
}

void importGridFromInputString(const char *input, char gridToEdit[GridSize][GridSize]) {
    for (int i = 0; i < GridSize; i++) {
        for (int j = 0; j < GridSize; j++) {
            char ch = input[i * GridSize + j];
            if (ch == 0)
                gridToEdit[i][j] = NOTHING;
            if (ch == 1)
                gridToEdit[i][j] = PATH;
        }
    }

}

void displayWaves() {
    for (size_t i = 0; i < WaveAmount; i++) {
        const Wave w = Waves[i];
        if (w.gold == 0)
            break;
        printf("Wave %d | Gold = %d | Enemies = %s\n", w.index, w.gold, w.enemies);
    }
}

int main(int argc, char *argv[]) {
    puts("Starting solver");
    FILE *file;
    file = fopen("grid_test.txt", "r");
    //file = fopen("grid_overlap.txt", "r");

    if (file == NULL) {
        puts("Fatal error, grid_old.txt not found, returning...");
        return -1;
    }
    char txt[stringSIZE] = {0};

    while (!feof(file)) {
        char lineBuffer[stringSIZE] = {0};
        fgets(lineBuffer, stringSIZE, file);
        strcat(txt, lineBuffer);
        if (ferror(file)) {
            fprintf(stderr, "Reading error\n");
            break;
        }
    }

    printf("\n\n%s\n", txt);

    int h = 0;
    char grid[GridSize][GridSize];

    int i = 0;
    int j = 0;

    while (txt[h] != '\0') {

        if (txt[h] == '\n') {
            j++;
            i = 0;
        } else {
            grid[i][j] = txt[h];
            i++;
        }
        h++;
    }

    updateGrid(grid);
    printf("\n");

    displayGrid(grid);

    fclose(file);
    printf("\n");

    initWaves();
    displayWaves();

    displayGrid(grid);

    /*
    for (int k = 0; k < 1e6; ++k) {
        printf("Simulation %d\n", k);
        simulate(grid, Waves[0], false);
    }
    */

    Wave customWave;
    customWave.index = 0;
    for (int k = 0; k < ARRAYSIZE; ++k) {
        customWave.enemies[k] = 0;
    }
    bool graphics = true;
    int repeat = 1;
    if (argc >= 2) {
        if (strcmp("headless", argv[1]) == 0) {
            graphics = false;
            if (argc >= 3) {
                repeat = atoi(argv[2]);
            }
        }
        if (strcmp("solve", argv[1]) == 0) {
            if (argc >= 4) {
                importGridFromInputString(argv[2], grid);
                customWave.gold = atoi(argv[3]);
                int argIndex = 0;
                int argWaveIndex = 0;
                while (argv[4][argIndex] != 0) {
                   if (argv[4][argIndex] != ',') {
                       customWave.enemies[argWaveIndex] = argv[4][argIndex];
                   }
                   argIndex++;
                }
            }
        }
    }



    /*
    for (int k = 0; k < repeat; ++k) {
        simulate(grid, Waves[0], graphics);
        printf("Simulation %d\n", k);
        //printf("Simulation %d - %s", k, simulate(grid, Waves[0], graphics) ? "SUCCESS" : "FAILURE");
    }
    printf("%d simulation(s) finished\n", repeat);
    */

    //getEquivalenceClass(grid, 5);
    //manage(grid, Waves[0]);

    /*
    //char* a = "....RHH";
    char* a = "....HHH";
    //int locations = Waves[0].gold + (int) strlen(a);
    int locations = 7;

    Battlefield *bf = getOptimalTurretPositions(grid, locations);
    simulate(bf->grid, Waves[0], graphics, a);
    displayGridWithTurrets(bf, a);
     */
}