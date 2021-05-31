#include "fonctions.h"


void print_tab(int tab[gridSIZE][gridSIZE]) {
	for (int i = 0; i < gridSIZE; i++) {
		for (int j = 0; j < gridSIZE; j++) {
			printf("%d\t", tab[j][i]);
		}
		printf("\n");
	}
}



void nettoyagefinstring(char* string,int size) {
	for (int i = 0; i < size; i++) {
		if ((string[i] != '#') && (string[i] != '@') && (string[i] != '\n')) {
			string[i] = 0;

		}
		//printf("%c", string[i]);
	}
}

char* cleanTab_tostring(int tab[gridSIZE][gridSIZE]) {
	char tab2[gridSIZE][gridSIZE];
	char* string = malloc(stringSIZE);
	if (string == NULL) return NULL;
	//puts("Affichage cleanTab 1");
	// on transforme le tableau de signe en tableau de caract�re
	for (int i = 0; i < gridSIZE; i++) {
		for (int j = 0; j < gridSIZE; j++) {
			if (tab[i][j] != 0) {
				tab2[i][j] = '@';
			}
			else {
				tab2[i][j] = '#';
			}
		}
	}
	/*// on affiche le tableau de caractère
	for (int i = 0; i < gridSIZE; i++) {
		for (int j = 0; j < gridSIZE; j++) {
			printf("%c", tab2[j][i]);
		}
		printf("\n");
	}*/


	// on le transcrit en chaine de caractère
	int h = 0;
	int j = 0;
	for (int i = 0; i < gridSIZE; i++) {
		for ( j = 0; j < gridSIZE; j++) {
			//printf("%c", tab2[j][i]);
			string[h] = tab2[j][i];
			h++;
		}
		if (j != gridSIZE - 1) {
			string[h] = '\n';
			h++;
		}
	}
	printf("\n\n\n\n");
	//printf("%s", string);
	
	// on printf la chaine

	//puts("Affichage cleanTab 2");

	// on renvoit la chaine de caract�re
	return(string);
}


int main() {
	

	/* une case = 0 => herbe*/
	/* une case = 1 => chemin*/
	/* une case = 2 => tourelle*/
	cursor path;
	bool arrivee= false;
	path.x = 1;
	path.y = 1;

	// la grille en elle m�me
	int grid[gridSIZE][gridSIZE] = { 0 };

	//on trace le chemin manuellement
	grid[0][1] = 2;
	grid[1][1] = 1;
	grid[13][12] = 2;

	print_tab(grid);

	while (arrivee == false) {
		
		// l'utilisateur choisi par o� le chemin va se prolonger

		printf(" Ou continuer le chemin ? \n (z) - vers le haut \n (q) - vers la gauche\n (s) - vers le bas\n (d) - vers la droite\n (r) - reset la grille\n");
		char c = getch();
		switch(c) {
			case('q'):
				if ((path.x == 1)||(grid[path.x-1][path.y]==1)||/*on interdit de revenir sur nos pas*/
				(grid[path.x-2][path.y]==1)||( grid[path.x - 1][path.y+1] == 1)||( grid[path.x -1][path.y-1] == 1)) {
					printf("impossible d'aller vers la gauche\n");
				}
				else {
					path.x -= 1;
				}
				break;
			case('d'):
				if ((path.x == 12)|| (grid[path.x + 1][path.y] == 1)||
					(grid[path.x + 2][path.y] == 1) || (grid[path.x + 1][path.y + 1] == 1) || (grid[path.x + 1][path.y - 1] == 1)) {
					printf("impossible d'aller vers la droite\n");
				}
				else {
					path.x += 1;
				}
				break;
			case('z'):
				if ((path.y == 1)|| (grid[path.x][path.y-1] == 1)||
					(grid[path.x][path.y-2] == 1) || (grid[path.x - 1][path.y - 1] == 1) || (grid[path.x + 1][path.y - 1] == 1)) {
					printf("impossible d'aller en haut\n");
				}
				else {
					path.y -= 1;
				}
				break;
			case('s'):
				if ((path.y == 12)|| (grid[path.x][path.y+1] == 1)||
					(grid[path.x][path.y+2] == 1) || (grid[path.x - 1][path.y + 1] == 1) || (grid[path.x + 1][path.y + 1] == 1)) {
					printf("impossible d'aller en bas\n");
				}
				else {
					path.y += 1;
				}
				break;
			case('r'):
				//reinitialiser le tableau, chemin et curseur
				for (int i = 1; i < gridSIZE-1; i++) {
					for (int j = 1; j < gridSIZE; j++) {
						grid[j][i] = 0;
					}
				}
				path.x = 1;
				path.y = 1;
				break;
		}
		grid[path.x][path.y] = 1;
		print_tab(grid);
		if (((path.x) == gridSIZE-2) && ((path.y) == gridSIZE-2)) {
			arrivee=true;
		}
		printf("\n\n");

	}
	print_tab(grid);
	char d = getch();

	//transcription du tableau en chaine de caract�re
	char* string = cleanTab_tostring(grid);


	printf("\n\n\n");

	printf("\nAffichage de votre grille :\n\n%s\n", string);
	
	printf("\n\n\n");

	//nettoyagefinstring(string, stringSIZE);


	//on met la chaine dans le fichier texte
	FILE* f = fopen("grid_old.txt", "w+");

	if (f == NULL) {
		printf("Erreur lors de l'ouverture d'un fichier");
		exit(1);
	}

	fputs(string, f);

	fclose(f);
	char e = getch();
	return EXIT_SUCCESS;
}
