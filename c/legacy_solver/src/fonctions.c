#include "fonctions.h"


Position realSpawn = {-1, 1};
//Position realSpawn = {0, 1};
Position end = {13, 12};

void displayGrid(const char grid[GridSize][GridSize]) {
    for (int i = 0; i < GridSize; i++) {
        for (int j = 0; j < GridSize; j++) {
            printf("%c", grid[j][i]);
        }
        printf("\n");
    }
}


void rotateArray(const char input[GridSize][GridSize], char toRotate[GridSize][GridSize] ) {
    for (int r = 0; r < GridSize; ++r)
    {
        for (int c = 0; c < GridSize; ++c)
        {
            toRotate[r][c] = input[c][GridSize-1-r];
        }
    }

}

void displayGridWithTurrets(const char grid[GridSize][GridSize], const char *turretArrangment) {
    int index = 0;
    char finalGrid[GridSize][GridSize];
    for (int i = 0; i < GridSize; i++) {
        for (int j = 0; j < GridSize; j++) {
            if (grid[i][j] == '$') {
                finalGrid[i][j] = turretArrangment[index];
                index++;
            } else
                finalGrid[i][j] = grid[i][j];
        }
    }

    displayGrid(finalGrid);
}

Cursor *getTurretPositionsInOrder(char grid[GridSize][GridSize], int gold) {
    int turretTotalNumber = gold / 10;
    int turretIndex = 0;
    Cursor *turretOrder = malloc(sizeof(Cursor) * turretTotalNumber);
    Cursor pos;
    pos.x = 1;
    pos.y = 1;
    Cursor previousPos;
    previousPos.x = 0;
    previousPos.y = 1;
    Cursor turretPos;
    while (pos.x != GridSize - 1 && pos.y != GridSize - 1) {
        for (int i = pos.x - 1; i <= pos.x + 1; i++) {
            for (int j = pos.y - 1; j <= pos.y + 1; j++) {
                if (grid[i][j] == grid[pos.x][pos.y] ||
                    grid[i][j] == grid[previousPos.x][previousPos.y]) // Check for currentPosition or previous position
                {
                    continue;
                }
                if (grid[i][j] == TURRET) { // Check for nearby turret position and put it in array
                    turretPos.x = i;
                    turretPos.y = j;
                    turretOrder[turretIndex] = turretPos;
                }
                if (i != pos.x && j != pos.y) {
                    continue;
                }
                if (grid[i][j] == PATH) {
                    previousPos.x = pos.x;
                    previousPos.y = pos.y;
                    pos.x = i;
                    pos.y = j;
                }

                if (turretIndex == turretTotalNumber - 1) {
                    return turretOrder;
                }
            }
        }
    }
    return turretOrder;
}

void copyGrid(const char src[GridSize][GridSize], char dest[GridSize][GridSize]) {
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            dest[i][j] = src[i][j];
        }
    }
}

Battlefield *getOptimalTurretPositions(char grid[GridSize][GridSize], int turretsTotal) {
    Battlefield *resultBattlefield = malloc(sizeof(Battlefield));
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            resultBattlefield->grid[i][j] = grid[i][j];
        }
    }

    int turretIndex = 0;
    int count;
    resultBattlefield->turretPositions = malloc(sizeof(Cursor) * turretsTotal);
    for (int k = 7; k > 0; k--) {
        for (int i = 0; i < GridSize; i++) {
            for (int j = 0; j < GridSize; j++) {
                if (grid[i][j] == PATH) { // Ne pas prendre en compte les cases du chemin
                    //printf(" x = %d | y = %d | count = %d \n", i, j, count);
                    continue;
                }

                count = 0;
                for (int x = i - 1; x <= i + 1; x++) {
                    for (int y = j - 1; y <= j + 1; y++) {
                        if (x >= GridSize)
                            continue;
                        if (x < 0)
                            continue;
                        if (y >= GridSize)
                            continue;
                        if (y < 0)
                            continue;
                        if (grid[x][y] == PATH) {
                            count++;
                        }
                    }
                }

                if (count == k) {
                    Cursor c;
                    c.x = i;
                    c.y = j;
                    c.reach = k;
                    resultBattlefield->grid[c.x][c.y] = TURRET;

                    resultBattlefield->turretPositions[turretIndex] = c;
                    turretIndex++;
                    resultBattlefield->turretAmount = turretIndex;
                    if (turretIndex == turretsTotal)
                        return resultBattlefield;
                }
            }
        }
    }
    return resultBattlefield;
}

/*
char *getEquivalenceClass(char grid[GridSize][GridSize], int turretAmount) {
    char *response = malloc(turretAmount * 4);

    Position lastPosition = {-1, -1};
    Position posInit = {1, 1};
    Position currentPosition = getNextAvailablePosition(grid, posInit, lastPosition);

    while (currentPosition.x != realSpawn.x || currentPosition.y != realSpawn.y) {
        printf("%d/%d\n", currentPosition.x, currentPosition.y);

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                grid[currentPosition.x+i][currentPosition.y+j];
            }
        }

        Position newPosition = getNextAvailablePosition(grid, currentPosition, lastPosition);
        lastPosition = currentPosition;
        currentPosition = newPosition;
        strcat(response, "X");
    }
    return response;
}
*/