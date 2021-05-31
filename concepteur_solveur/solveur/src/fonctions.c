#include "fonctions.h"

void displayGrid(char grid[GridSize][GridSize])
{
    for (int i = 0; i < GridSize; i++)
    {
        for (int j = 0; j < GridSize; j++)
        {
            printf("%c", grid[j][i]);
        }
        printf("\n");
    }
}
