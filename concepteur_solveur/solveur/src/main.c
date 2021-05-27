#include "fonctions.h"
#include "TerminalUtils.h"
#include "simulation.h"

#define WaveAmount 64
Wave Waves[WaveAmount] = {};

Cursor *getOptimalTurretPositions(char grid[GridSize][GridSize], int gold)
{
    const int turretsTotal = gold / 10;
    int turretIndex = 0;
    Cursor *turretPositions = malloc(sizeof(Cursor) * turretsTotal);
    int count = 0;
    for (int k = 7; k > 0; k--)
    {
        for (int i = 0; i < GridSize; i++)
        {
            for (int j = 0; j < GridSize; j++)
            {
                if (grid[i][j] == PATH)
                { // Ne pas prendre en compte les cases du chemin
                    //printf(" x = %d | y = %d | count = %d \n", i, j, count);
                    continue;
                }

                count = 0;
                for (int x = i - 1; x <= i + 1; x++)
                {
                    for (int y = j - 1; y <= j + 1; y++)
                    {
                        if (x >= GridSize)
                            continue;
                        if (x < 0)
                            continue;
                        if (y >= GridSize)
                            continue;
                        if (y < 0)
                            continue;
                        if (grid[x][y] == PATH)
                        {
                            count++;
                        }
                    }
                }

                //printf(" x = %d | y = %d | count = %d \n", i, j, count);

                if (count == k)
                {
                    //for (int index = 0; index < )
                    Cursor c;
                    c.x = i;
                    c.y = j;
                    c.reach = k;
                    grid[c.x][c.y] = TURRET;

                    turretPositions[turretIndex] = c;
                    turretIndex++;
                    if (turretIndex == turretsTotal)
                        return turretPositions;
                }
            }
        }
    }
    return turretPositions;
}

void displayPositions(Cursor *cursors, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        Cursor cursor = cursors[i];
        printf("Position %d - [%d/%d]\n", i, cursor.x, cursor.y);
    }
}

Cursor *TurretPositionInOrder(char grid[GridSize][GridSize], int gold)
{
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
    while (pos.x != GridSize - 1 && pos.y != GridSize - 1)
    {
        for (int i = pos.x - 1; i <= pos.x + 1; i++)
        {
            for (int j = pos.y - 1; j <= pos.y + 1; j++)
            {
                if (grid[i][j] == grid[pos.x][pos.y] || grid[i][j] == grid[previousPos.x][previousPos.y]) // Check for currentPosition or previous position
                {
                    continue;
                }
                if (grid[i][j] == TURRET)
                { // Check for nearby turret position and put it in array
                    turretPos.x = i;
                    turretPos.y = j;
                    turretOrder[turretIndex] = turretPos;
                }
                if (i != pos.x && j != pos.y)
                {
                    continue;
                }
                if (grid[i][j] == PATH)
                {
                    previousPos.x = pos.x;
                    previousPos.y = pos.y;
                    pos.x = i;
                    pos.y = j;
                }

                if (turretIndex == turretTotalNumber - 1)
                {
                    return turretOrder;
                }
            }
        }
    }
}

void displayGrid(char grid[GridSize][GridSize])
{
    puts("hi");
    for (int i = 0; i < GridSize; i++)
    {
        for (int j = 0; j < GridSize; j++)
        {
            printf("%c", grid[j][i]);
        }
        printf("\n");
    }
    puts("bye");
}


void updateGrid(char grid[GridSize][GridSize])
{
    for (int i = 0; i < GridSize; i++)
    {
        for (int j = 0; j < GridSize; j++)
        {
            if (grid[j][i] == '#')
                grid[j][i] = NOTHING;
            if (grid[j][i] == '@')
                grid[j][i] = PATH;
            if (grid[j][i] == '$')
                grid[j][i] = TURRET;
        }
    }
}

void initWaves()
{
    FILE *file;
    file = fopen("waves.txt", "r");

    if (file == NULL)
    {
        return;
    }

    char txt[stringSIZE] = {0};

    int waveIndex = 0;
    while (!feof(file))
    {
        char lineBuffer[stringSIZE] = {0};
        fgets(lineBuffer, stringSIZE, file);
        strcat(txt, lineBuffer);
        if (ferror(file))
        {
            fprintf(stderr, "Reading error\n");
            break;
        }
        //printf(lineBuffer);

        bool goldParsed = false;
        char goldStr[8] = {0};
        int i = 0;
        char tmpEnemies[256] = {0};
        int enemyIndex = 0;
        while (*(lineBuffer + i) != 0)
        {
            if (lineBuffer[i] == ',')
            {
                if (!goldParsed)
                {
                    goldParsed = true;
                }
                i++;
                continue;
            }
            if (!goldParsed)
            {
                goldStr[i] = lineBuffer[i];
            }
            else if (lineBuffer[i] != '\n' && lineBuffer[i] != '\r')
            {
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

void displayWaves()
{
    for (size_t i = 0; i < WaveAmount; i++)
    {
        const Wave w = Waves[i];
        if (w.gold == 0)
            break;
        printf("Wave %d | Gold = %d | Enemies = %s\n", w.index, w.gold, w.enemies);
    }
}

int main()
{
    puts("Starting solver");
    FILE *file;
    file = fopen("grid.txt", "r");

    if (file == NULL)
    {
        puts("Fatal error, grid.txt not found, returning...");
        return -1;
    }
    char txt[stringSIZE] = {0};

    while (!feof(file))
    {
        char lineBuffer[stringSIZE] = {0};
        fgets(lineBuffer, stringSIZE, file);
        strcat(txt, lineBuffer);
        if (ferror(file))
        {
            fprintf(stderr, "Reading error\n");
            break;
        }
    }

    printf("\n\n%s\n", txt);

    int h = 0;
    char grid[GridSize][GridSize];

    int i, j = 0;

    while (txt[h] != '\0')
    {

        if (txt[h] == '\n')
        {
            j++;
            i = 0;
        }
        else
        {
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
    int gold = 70;
    Cursor *cursors = getOptimalTurretPositions(grid, gold);
    displayGrid(grid);
    displayPositions(cursors, gold / 10);

    initWaves();
    displayWaves();
    //printf(" x = %d | y = %d \n", pos.x, pos.y);

    displayGrid(grid);

    //simulate(grid, Waves[0]);
}