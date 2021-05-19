#include "fonctions.h"

/*      ##############
        @@############
        #@############
        #@@@@@####@@@#
        #####@####@#@#
        #####@@@@@@#@#
        ############@#
        ############@#
        ############@#
        ############@#
        ############@#
        ############@#
        ############@@
        ##############  */

cursor selection(char grid[gridSIZE][gridSIZE])
{
    int count = 0;
    for (int k = 7; k > 0; k--)
    {

        for (int i = 1; i < gridSIZE - 1; i++)
        {
            for (int j = 1; j < gridSIZE - 1; j++)
            {

                for (int x = i - 1; x <= i + 1; x++)
                {
                    for (int y = j - 1; y <= j + 1; y++)
                    {

                        if (grid[x][y] == '@')
                        {
                            count++;
                        }
                    }
                }

                if (count == k)
                {
                    cursor curseur;
                    curseur.x = i;
                    curseur.y = j;

                    return curseur;
                }

                count = 0;
            }
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
    char txt[stringSIZE];
    int l = 0;
    while (!feof(file))
    {
        char lineBuffer[stringSIZE] =  {0};
        fgets(lineBuffer, stringSIZE, file);
        strcat(txt, lineBuffer);
        if (ferror(file))
        {
            fprintf(stderr, "Reading error with code %d\n");
            break;
        }
        
        printf("loop %d %s", l, lineBuffer);
        l++;
    }
    printf("txt\n\n\ntest ???%s\ntxt end\n", txt);
    int h = 0;
    char grid[gridSIZE][gridSIZE];

    for (int i = 0; i < gridSIZE; i++)
    {
        for (int j = 0; j < gridSIZE; j++)
        {
            grid[j][i] = txt[h];
            h++;
        }
    }
    
    for (int i = 0; i < gridSIZE; i++) {
        for(int j = 0; j < gridSIZE; j++){
                 printf("%c", grid[j][i]);
            }
        printf("\n");
    }

    fclose(file);
}