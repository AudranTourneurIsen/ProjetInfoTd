#include "fonctions.h"
#include "TerminalUtils.h"

Cursor *getOptimalTurretPositions(char grid[gridSIZE][gridSIZE], int gold)
{
    const int turretsTotal = gold / 10;
    int turretIndex = 0;
    Cursor *turretPositions = malloc(sizeof(Cursor) * turretsTotal);
    int count = 0;
    for (int k = 7; k > 0; k--)
    {
        for (int i = 0; i < gridSIZE; i++)
        {
            for (int j = 0; j < gridSIZE; j++)
            {
                count = 0;
                if (grid[i][j] == PATH)
                { // Ne pas prendre en compte les cases du chemin
                    //printf(" x = %d | y = %d | count = %d \n", i, j, count);
                    continue;
                }

                for (int x = i - 1; x <= i + 1; x++)
                {
                    for (int y = j - 1; y <= j + 1; y++)
                    {
                        if (x > gridSIZE - 1)
                            continue;
                        if (x < 0)
                            continue;
                        if (y > gridSIZE - 1)
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
        printf("[%d/%d] -> ", cursor.x, cursor.y);
    }
}

Cursor *TurretPositionInOrder(char grid[gridSIZE][gridSIZE], int gold)
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
    while (pos.x != gridSIZE - 1 && pos.y != gridSIZE - 1)
    {
        for (int i = pos.x - 1; i <= pos.x + 1; i++)
        {
            if (grid[i][pos.y] == grid[pos.x][pos.y] || grid[i][pos.y] == grid[previousPos.x][previousPos.y]) // Check for current or previous position
            {
                continue;
            }
            if (grid[i][pos.y] == PATH)
            {
                previousPos.x = pos.x;
                previousPos.y = pos.y;
                pos.x = i;
            }
        }

        for (int j = pos.y - 1; j <= pos.y + 1; j++)
        {
            if (grid[pos.x][j] == grid[pos.x][pos.y] || grid[pos.x][j] == grid[previousPos.x][previousPos.y]) // Check for current or previous position
            {
                continue;
            }
            if (grid[pos.x][j] == PATH)
            {
                previousPos.x = pos.x;
                previousPos.y = pos.y;
                pos.y = j;
            }
        }

        for (int i = pos.x - 1; i <= pos.x + 1; i++)
        {
            for (int j = pos.y - 1; j <= pos.y + 1; j++)
            {
                if (grid[i][j] == grid[pos.x][pos.y] || grid[i][j] == grid[previousPos.x][previousPos.y]) // Check for current or previous position
                {
                continue;
                }
                if (grid[i][j]== TURRET){   // Check for nearby turret position and put it in array
                    turretPos.x=i;
                    turretPos.y=j;
                    turretOrder[turretIndex]=turretPos;
                }
                if (i != pos.x && j != pos.y){
                    continue;
                }
                if (grid[i][j] == PATH){
                    
                }
            }
        }
    }
}

void displayGrid(char grid[gridSIZE][gridSIZE])
{
    puts("hi");
    for (int i = 0; i < gridSIZE; i++)
    {
        for (int j = 0; j < gridSIZE; j++)
        {
            printf("%c", grid[j][i]);
        }
        printf("\n");
    }
    puts("bye");
}

void getTurretTypes()
{
}

void simulate() {
    Init();
    for (size_t i = 0; i < 10; i++)
    {
        Wait(200);
        Draw(i, i, 'A');
        Refresh();
    }
    Refresh();
    End();
}

void updateGrid(char grid[gridSIZE][gridSIZE])
{
    for (int i = 0; i < gridSIZE; i++)
    {
        for (int j = 0; j < gridSIZE; j++)
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

int main()
{
    FILE *file;
    file = fopen("grid.txt", "r");

    if (file == NULL)
    {
        return -1;
    }
    char txt[stringSIZE] = {0};
    int l = 0;

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
        l++;
    }

    printf("\n\n%s\n", txt);

    int h = 0;
    char grid[gridSIZE][gridSIZE];

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

    simulate();

    //printf(" x = %d | y = %d \n", pos.x, pos.y);
}