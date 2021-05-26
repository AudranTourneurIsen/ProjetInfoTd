#include "fonctions.h"
#include "TerminalUtils.h"
#include "stack.h"
#include "queue.h"

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct SimulationData {
    bool isFinished;
    int gameTick;
    Queue enemiesRemainingToSpawn;
} SimulationData;

char enemiesRemainingToSpawn[] = {};

Position* getNextAvailablePosition(Position currentPosition, char grid[gridSIZE][gridSIZE]) {
    return NULL;
}

void spawnEnemy(Enemy enemy, Position position) {

}

void updateSimulation(char grid[gridSIZE][gridSIZE], Wave wave, SimulationData* simultationData) {
    Position spawn = {0, 1};
    Position end = {13, 12};

    while (simultationData->gameTick % 2 == 0 && isQueueEmpty(simultationData->enemiesRemainingToSpawn)) {

    }

    GameTick++;
}

void drawSimulation(char grid[gridSIZE][gridSIZE], Wave wave) {
    Init();

    for (int i = 0; i < gridSIZE; i++) {
        for (int j = 0; j < gridSIZE; j++) {
            Draw(i, j, grid[j][i]);
        }
    }
    Write(0, 14, "TEST");
    Refresh();
    Wait(1000);

    End();
}

bool isFinished = false;

// Returns true if the simulation is successful, false otherwise
bool simulate(char grid[gridSIZE][gridSIZE], Wave wave) {
    SimulationData sim = {false, 0};
    //strcpy(sim.enemiesRemainingToSpawn, wave.enemies);
    int index = 0;
    while (wave.enemies[index] != 0) {

        index++;
    }
    Queue *enemiesRemaining = NULL;
    NewQueue(&enemiesRemaining, 256);
    while (!isFinished) {
        updateSimulation(grid, wave, &sim);
        drawSimulation(grid, wave);
    }
    return false;
}