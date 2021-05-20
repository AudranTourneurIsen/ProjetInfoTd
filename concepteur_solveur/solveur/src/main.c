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
                count = 0;
                if (grid[i][j]== '@'){          // Ne pas prendre en compte les cases du chemin
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                }

                // On teste pour les 4 coins
                
                if (i == 0 && j == 0){          // Coin en haut à gauche
                    if (grid[0][1] == '@'){
                        count++;
                    }
                    if (grid[1][1] == '@'){
                        count++;
                    }
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                }
                
                if (i == 13 && j == 0){         // Coin en bas à gauche
                    if (grid[13][1] == '@'){
                        count++;
                    }
                    if (grid[12][1] == '@'){
                        count++;
                    }
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                }
                
                if (i == 0 && j == 13){         // Coin en haut à droite
                
                    if (grid[1][12] == '@'){
                        count++;
                    }
                    
                    if (grid[0][12] == '@'){
                        count++;
                    }
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                    
                }
                
                if (i == 13 && j == 13){        // Coin en bas à droite

                    if (grid[12][12] == '@'){
                        count++;
                    }
                    
                    if (grid[13][12] == '@'){
                        count++;
                    }
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                }
                
                // Condition pour la ligne du haut

                if (j == 0){
                    for (int x = i - 1; x <= i + 1; x++){

                            if (grid[x][j+1] == '@'){
                                count++;
                            }
                    }
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                }

                // Condition pour la ligne du bas

                 if (j == 13){
                    for (int x = i - 1; x <= i + 1; x++){

                        if (grid[x][j-1] == '@'){
                            count++;
                        }
                    }
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                }

                // Condition pour la colonne de gauche

                if (i == 0){
                    for (int x = i; x <= i + 1; x++){
                    
                        for (int y = j - 1; y <= j + 1; y++){
                        
                            if (grid[x][y] == '@'){
                                count++;
                            }
                        }
                    }
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                }

                // Condition pour la colonne de droite

                if (i == 13){
                    for (int x = i-1; x <= i; x++){
                    
                        for (int y = j - 1; y <= j + 1; y++){
                        
                            if (grid[x][y] == '@'){
                            count++;
                            }
                        }
                    }
                    printf(" x = %d | y = %d | count = %d \n", i,j,count);
                    continue;
                }

                // Condition générale
               
                for (int x = i - 1; x <= i + 1; x++){
                    
                    for (int y = j - 1; y <= j + 1; y++){
                        
                        if (grid[x][y] == '@'){
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

            }
        }
    }
}


int main(){

    FILE *file;
    file = fopen("grid.txt", "r");

    if (file == NULL)
    {
        return -1;
    } 
    char txt[stringSIZE+13] = {0};
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
        //printf("loop-txt %d %s", l, txt);
        l++;
    }
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

    for (int i = 0; i < gridSIZE; i++) {
        for(int j = 0; j < gridSIZE; j++){
                 printf("%c", grid[j][i]);
            }
    }


    printf("\ntest, %c %c %c", grid[1][1], grid[1][2], grid[2][1]);
}