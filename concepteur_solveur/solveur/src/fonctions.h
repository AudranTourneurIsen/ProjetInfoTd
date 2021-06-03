#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conio.h"
#include <stdbool.h>

#define GridSize 14
#define stringSIZE 2000

#define NOTHING '.'
#define PATH '+'
#define TURRET '$'

typedef struct Cursor {
	int x;
	int y;
	int reach;
} Cursor;

typedef struct Wave {
	int index;
	int gold;
	char enemies[256];
	//Enemy* enemies;
	//char* enemies;
} Wave;

void print_tab(int tab[GridSize][GridSize]);
char* cleanTab_tostring(int tab[GridSize][GridSize]);
void nettoyagefinstring(char* string, int size);

void displayGrid(char grid[GridSize][GridSize]);

typedef struct SimulationResult {
    bool success;
    int ticks;
} SimulationResult;

Cursor *getTurretPositionsInOrder(char grid[GridSize][GridSize], int gold);

typedef struct Battlefield {
    char grid[GridSize][GridSize];
    Cursor *turretPositions;
    int turretAmount;
} Battlefield;

Battlefield *getOptimalTurretPositions(char grid[GridSize][GridSize], int turretsTotal);

void displayGridWithTurrets(Battlefield* bf, char* turretArrangment);


#define ARRAYSIZE 256
#define GAMETICK 500

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
    Turret *turretsArray;
    int enemiesLeftToWin;
    char turretsArrangment[ARRAYSIZE];
    bool graphics;
} SimulationData;

typedef struct TurretType {
    char name;
    int damage;
    int attackCooldown;
} TurretType;

Position getNextAvailablePosition(char grid[GridSize][GridSize], Position currentPosition, Position lastPosition);

char* getEquivalenceClass(char grid[GridSize][GridSize], int turretAmount);
