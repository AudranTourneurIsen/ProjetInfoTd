#include "fonctions.h"
#include "TerminalUtils.h"
#include "simulation.h"
#include "simulation_manager.h"

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

void importGridFromInputString(char *input, char gridToEdit[GridSize][GridSize]) {
    for (int i = 0; i < GridSize; i++) {
        for (int j = 0; j < GridSize; j++) {
            char ch = input[i * GridSize + j];
            if (ch == '1')
                gridToEdit[j][i] = PATH;
            else
                gridToEdit[j][i] = NOTHING;
        }
    }

}

void displayWave(Wave w) {
    printf("Wave %d | Gold = %d | Enemies = %s\n", w.index, w.gold, w.enemies);
}

void test(char grid[GridSize][GridSize], Wave customWave, bool graphics) {

    char *a = "RRH...";
    int locations = (int) strlen(a);

    Battlefield *bf = getOptimalTurretPositions(grid, locations);
    SimulationResult res = simulate(bf->grid, customWave, graphics, a);
    displayGridWithTurrets(bf->grid, a);

    displayGrid(grid);
    displayWave(customWave);
    displayGridWithTurrets(bf->grid, a);
    printf("res sim = %s", res.success ? "OK" : "FAIL");

}

int main(int argc, char *argv[]) {
    puts("Starting solver");

    char grid[GridSize][GridSize];
    Wave customWave;
    customWave.index = 0;
    for (int k = 0; k < ARRAYSIZE; ++k) {
        customWave.enemies[k] = 0;
    }
    if (argc <= 2) {
        puts("ERROR: Please provide arguments");
        return 1;
    }
    if (argc >= 2) {
        if (strcmp("solve", argv[1]) == 0) {
            if (argc >= 4) {
                importGridFromInputString(argv[2], grid);
                customWave.gold = atoi(argv[3]);
                int argIndex = 0;
                int argWaveIndex = 0;
                while (argv[4][argIndex] != 0) {
                    if (argv[4][argIndex] != ',') {
                        customWave.enemies[argWaveIndex] = argv[4][argIndex];
                        argWaveIndex++;
                    }
                    argIndex++;
                }
            }
        }
    }

   manage(grid, customWave);

   //test(grid, customWave, true);

    /*
    for (int k = 0; k < repeat; ++k) {
        simulate(grid, Waves[0], graphics);
        printf("Simulation %d\n", k);
        //printf("Simulation %d - %s", k, simulate(grid, Waves[0], graphics) ? "SUCCESS" : "FAILURE");
    }
    printf("%d simulation(s) finished\n", repeat);
    */

    //getEquivalenceClass(grid, 5);

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