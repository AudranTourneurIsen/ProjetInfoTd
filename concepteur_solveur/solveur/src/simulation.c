#include "fonctions.h"
#include "TerminalUtils.h"
#include "simulation.h"

#define ARRAYSIZE 256
#define GAMETICK 200

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Enemy {
    char name;
    int index;
    int hp;
    int maxHp;
    Position currentPosition;
    Position lastPosition;
    bool spawned;
    bool dead;
} Enemy;

typedef struct Turret {
    char name;
    int damage;
    int attackCooldown;
    Position position;
    int cooldownRemaining;
} Turret;

typedef struct SimulationData {
    bool isFinished;
    bool won;
    int gameTick;
    int enemiesLeftToSpawn;
    //Queue* enemiesRemainingToSpawn;
    Enemy enemies[ARRAYSIZE];
    char grid[GridSize][GridSize];
    int turretsSize;
    Turret* turretsArray;
    int enemiesLeftToWin;
} SimulationData;

Position spawn = {0, 1};
Position end = {13, 12};

Position getNextAvailablePosition(char grid[GridSize][GridSize], Position currentPosition, Position lastPosition) {
    for (int i = currentPosition.x - 1; i <= currentPosition.x + 1; i++) {
        for (int j = currentPosition.y - 1; j <= currentPosition.y + 1; j++) {
            if (i < 0) continue;
            if (i >= GridSize) continue;
            if (j < 0) continue;
            if (j >= GridSize) continue;
            /*
             if (grid[i][j] == grid[currentPosition.x][currentPosition.y]
                || grid[i][j] == grid[lastPosition.x][lastPosition.y]
                //|| (i != currentPosition.x && j != currentPosition.y)
                ) // Check for currentPosition or previous position or corners
            {
             */
            if (
                (i == currentPosition.x && j == currentPosition.y)
                || (i == lastPosition.x && j == lastPosition.y)
                || (i != currentPosition.x && j != currentPosition.y)
                ) {
                continue;
            }
            if (grid[i][j] == PATH) {
                Position newPosition;
                newPosition.x = i;
                newPosition.y = j;
                return newPosition;
            }
        }
    }
    Position defaultPosition = spawn;
    return defaultPosition;
}

int enemyNameToHp(char name) {
    switch (name) {
        case 'w':
            return 3;
        case 'n':
            return 5;
        case 't':
            return 15;
        default:
            return 1;

    }
}

void initializeEnemies(SimulationData *sim, const char waveEnemies[ARRAYSIZE]) {
    int index = 0;
    while (waveEnemies[index] != 0) {
        sim->enemies[index].spawned = false;
        sim->enemies[index].dead = false;
        sim->enemies[index].index = index + 1;
        sim->enemies[index].name = waveEnemies[index];
        sim->enemies[index].hp = enemyNameToHp(sim->enemies[index].name);
        sim->enemies[index].maxHp = enemyNameToHp(sim->enemies[index].name);
        sim->enemiesLeftToSpawn++;
        sim->enemiesLeftToWin++;
        index++;
    }
}

void moveEnemy(SimulationData *sim, Enemy *enemy) {
    Position currentPosition = enemy->currentPosition;
    Position newPosition = getNextAvailablePosition(sim->grid, enemy->currentPosition, enemy->lastPosition);
    enemy->lastPosition = currentPosition;
    enemy->currentPosition = newPosition;
}

Enemy* getEnemyAtPositionOrNull(SimulationData* sim, Position position) {
    for (int i = 0; i < ARRAYSIZE; ++i) {
        Enemy* candidate = &sim->enemies[i];
        if (candidate->name == 0) break;
        if (candidate->currentPosition.x == position.x && candidate->currentPosition.y == position.y) {
            return &sim->enemies[i];
        }
    }
    return NULL;
}

void updateTurrets(SimulationData *sim) {
    for (int t = 0; t < sim->turretsSize; ++t) {
        Turret turret = sim->turretsArray[t];

        for (int i = turret.position.x-1; i <= turret.position.x+1; i++) {
            for (int j = turret.position.y-1; j <= turret.position.y+1; j++) {
                if (i < 0) continue;
                if (i >= GridSize) continue;
                if (j < 0) continue;
                if (j >= GridSize) continue;
                Position targetPosition = {i, j};
                Enemy* enemyRef = getEnemyAtPositionOrNull(sim, targetPosition);
                if (enemyRef != NULL) {
                    if (enemyRef->dead) continue;
                    enemyRef->hp--;
                    if (enemyRef->hp <= 0) {
                        enemyRef->dead = true;
                        sim->enemiesLeftToWin--;
                    }
                }
            }
        }

    }
}

void updateSimulation(SimulationData *simulationData) {

    // Spawning enemies
    if (simulationData->gameTick % 2 == 0 && simulationData->enemiesLeftToSpawn > 0) {
        for (int i = 0; i < ARRAYSIZE; ++i) {
            Enemy enemy = simulationData->enemies[i];
            if (enemy.spawned) continue;
            if (enemy.name != 0) {
                // Spawning enemy
                simulationData->enemies[i].currentPosition = spawn;
                simulationData->enemies[i].lastPosition = spawn;
                simulationData->enemies[i].spawned = true;
                simulationData->enemiesLeftToSpawn--;
                break;
            }
        }
    }

    // Main loop
    int index = 0;
    while (simulationData->enemies[index].spawned == true) {
        if (simulationData->enemies[index].hp != 0) {
            moveEnemy(simulationData, &simulationData->enemies[index]);
            if (simulationData->enemies[index].currentPosition.x == end.x && simulationData->enemies[index].currentPosition.y == end.y) {
                simulationData->isFinished = true;
            }
        }
        index++;
    }

    updateTurrets(simulationData);

    if (simulationData->enemiesLeftToWin <= 0) {
        simulationData->isFinished = true;
        simulationData->won = true;
    }

    simulationData->gameTick++;
}

void drawSimulation(SimulationData *simulationData) {
    Clear();
    for (int i = 0; i < GridSize; i++) {
        for (int j = 0; j < GridSize; j++) {
            Draw(j + 1, i + 1, simulationData->grid[j][i]);
        }
        char line[6];
        sprintf(line, "L%d", i);
        Write(20 + 1, i + 1, line);
    }
    char str[32] = "";
    sprintf(str, "Game Tick %d", simulationData->gameTick);
    Write(0 + 1, GridSize + 1, str);

    for (int i = 0; i < ARRAYSIZE; ++i) {
        Enemy enemy = simulationData->enemies[i];
        if (enemy.name == 0) break;
        if (enemy.name != 0) {
            if (!enemy.spawned || enemy.dead) continue;
            Draw(enemy.currentPosition.x + 1, enemy.currentPosition.y + 1, enemy.name);
        }
    }

    for (int i = 0; i < simulationData->turretsSize; ++i) {
        Turret turret = simulationData->turretsArray[i];
        Draw(turret.position.x + 1, turret.position.y + 1, turret.name);
    }

    int posY = GridSize+2;
    for (int i = 0; i < ARRAYSIZE; ++i) {
        Enemy enemy = simulationData->enemies[i];
        if (enemy.name == 0) break;
        if (enemy.name != 0) {
            sprintf(str, "E %c %d/%d %s", enemy.name, enemy.hp, enemy.maxHp, enemy.dead ? "DEAD" : "ALIVE");
            Write(1, posY+1, str);
            posY++;
        }
    }

    Refresh();
    Wait(GAMETICK);

    //End();
}


void initializeTurrets(SimulationData *sim, int turretAmount) {
    sim->turretsSize = turretAmount;
    sim->turretsArray = malloc(sizeof(Turret) * turretAmount);
    int count = 0;
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            if (sim->grid[i][j] == TURRET) {
                Turret turret;
                turret.name = 'F';
                turret.damage = 1;
                turret.attackCooldown = 2;
                turret.position.x = i;
                turret.position.y = j;
                sim->turretsArray[count] = turret;
                count++;
            }
        }
    }
}

// Returns true if the simulation is successful, false otherwise
bool simulate(char grid[GridSize][GridSize], Wave wave, bool graphics) {
    SimulationData sim = {false, false, 0};
    for (int i = 0; i < GridSize; ++i)
        for (int j = 0; j < GridSize; ++j)
            sim.grid[i][j] = grid[i][j];
    initializeEnemies(&sim, wave.enemies);
    initializeTurrets(&sim, wave.gold/10);
    if (graphics)
        Init();
    while (!sim.isFinished) {
        updateSimulation(&sim);
        if (graphics)
            drawSimulation(&sim);
    }
    if (graphics)
        End();
    if (graphics) {
        puts(sim.won ? "SUCCESS" : "FAILURE");
        printf("Simulation finished after %d game ticks\n", sim.gameTick);
    }
    return sim.won;
}