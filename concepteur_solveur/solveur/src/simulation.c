#include "fonctions.h"
#include "TerminalUtils.h"

int GameTick = 0;

char enemiesRemainingToSpawn[] = {};

//Cursor getNextAvailablePosition(Cursor currentPosition, char grid[gridSIZE][gridSIZE]) {
//    return NULL;
//}

void spawnEnemy(Enemy enemy, Cursor position) {

}

void updateSimulation(char grid[gridSIZE][gridSIZE], Wave wave) {
    Cursor spawn = {0, 1};
    Cursor end = {13, 12};

    GameTick++;
}

void drawSimulation(char grid[gridSIZE][gridSIZE], Wave wave)
{
    Init();

    for (int i = 0; i < gridSIZE; i++)
    {
        for (int j = 0; j < gridSIZE; j++)
        {
            Draw(i, j, grid[j][i]);
        }
    }
    Refresh();
    Wait(1000);

    End();
}

bool isFinished = false;

void simulate(char grid[gridSIZE][gridSIZE], Wave wave)
{
    while (!isFinished) {
        updateSimulation(grid, wave);
        drawSimulation(grid, wave);
    }
}