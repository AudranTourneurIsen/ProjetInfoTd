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

        for (int i = 0; i < gridSIZE; i++)
        {
            for (int j = 0; j < gridSIZE; j++)
            {
                 if (grid[i][j]== '@'){
                    break;
                }
                if (i == 0 && j == 0){
                    
                }
               
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

                printf(" x = %d | y = %d | count = %d \n", i,j,count);

                if (count == k){
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
    char txt[stringSIZE] = {0};
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
        //printf("loop-txt %d %s", l, txt);
        l++;
    }
    //txt[0] = 'B';
    printf("\n\n%s\n", txt);
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
    }

    fclose(file);

    cursor pos = selection(grid);
    printf(" x = %d | y = %d \n",pos.x,pos.y);
}