#include "random.h"


void print_tab(int tab[gridSIZE][gridSIZE]) {
	for (int i = 0; i < gridSIZE; i++) {
		for (int j = 0; j < gridSIZE; j++) {
			printf("%d\t", tab[j][i]);
		}
		printf("\n");
	}
}



void nettoyagefinstring(char* string, int size) {
	for (int i = 0; i < size; i++) {
		if ((string[i] != '#') && (string[i] != '@') && (string[i] != '\n')) {
			string[i] = 0;

		}
		printf("%c", string[i]);
	}
}

char* cleanTab_tostring(int tab[gridSIZE][gridSIZE]) {
	char tab2[gridSIZE][gridSIZE];
	char* string = malloc(stringSIZE);
	if (string == NULL) return EXIT_FAILURE;

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
	// on affiche le tableau de caract�re
	for (int i = 0; i < gridSIZE; i++) {
		for (int j = 0; j < gridSIZE; j++) {
			printf("%c", tab2[j][i]);
		}
		printf("\n");
	}


	// on le transcrit en chaine de caract�re
	int h = 0;
	int j = 0;
	for (int i = 0; i < gridSIZE; i++) {
		for (j = 0; j < gridSIZE; j++) {
			printf("%c", tab2[j][i]);
			string[h] = tab2[j][i];
			h++;
		}
		if (j != gridSIZE - 1) {
			string[h] = '\n';
			h++;
		}
		if (string[h] == 0) break;

	}



	printf("\n\n\n\n");
	printf("%s", string);

	// on printf la chaine


	// on renvoit la chaine de caract�re
	return(string);
}




// conception random
int main() {

	/* une case = 0 => herbe*/
	/* une case = 1 => chemin*/
	/* une case = 2 => tourelle*/
	cursor path;
	bool arrivee = false;
	path.x = 1;
	path.y = 1;

	// la grille en elle m�me
	int grid[gridSIZE][gridSIZE] = { 0 };

	//on trace le chemin manuellement, du moins l'arriv�e et le d�part
	grid[0][1] = 2;
	grid[1][1] = 1;
	grid[13][12] = 2;


	srand(time(NULL));
	print_tab(grid);

	while (arrivee == false) {

		/* On commence par d�finir un int qui prendra la valeur de la direction
		 on pose :
			0 => haut
			1 => gauche
			2 => bas
			3 => droite
		*/
		int direction = 0;


		/* quand la direction est choisie, il faut maintenant d�terminer de combien de cases on avance,
		pour �a on pose un nouvel int qui prendra une valeur pseudo-random entre 1 et 3*/

		int avancement = 0;


		direction = rand() % 4;
		avancement = rand() % 3 + 1;
		/* penser aux virages trop serr�*/

		switch (direction) {
			case(0):
				
				// cas ou il ne faut pas aller vers le haut : 
					// quand on est sur les lignes 1 ou moins
					// si en se d�pla�ant de 2 cases vers le haut on retombe sur le chemin
					// quand on est sur les colonnes 11 ou plus
					// quand le virage est trop serr�
				if ((path.y < 2) || (grid[path.x][path.y - 2] == 1) || (path.x > 10) || (grid[path.x][path.y - 1] == 1)||(grid[path.x+1][path.y-1]==1)||(grid[path.x-1][path.y-1]==1)) {
					while (direction == 0) {
						direction = rand() % 4;
					}
				}

				

				//on d�passe pas de la grille
				while (path.x - avancement < 1) {
					avancement--;
				}		
				// on avance vers le haut
				for (int i = 0; i < avancement; i++) {
					path.x --;
					grid[path.x][path.y] = 1;
				}
				break;
			
			
			case(1):
				// cas o� il ne faut pas tourner � gauche : 
					// quand on est sur la colonne la plus � gauche,
					// si en se d�pla�ant de 2 cases vers la gauche on retombe sur le chemin
					// quand on est sur les lignes : 13, 12 (ligne de l'arriv�e) et 11
					// en remontant si l'on est sur les lignes 2,1 ou 0
					// si la position pr�c�dente est � gauche
					// quand le virage est trop serr�
				if ((path.x <2) || (grid[path.x - 2][path.y] == 1) || (path.y > 10) || (path.y < 3) || (grid[path.x - 1][path.y] == 1)||(grid[path.x-1][path.y-1]==1)||(grid[path.x-1][path.y+1]==1)) {
					while (direction == 1) {
						direction = rand() % 4;
					}
				}

				
					//on d�passe pas de la grille
				while (path.x - avancement < 1) {
					avancement--;
				}
				// on avance vers la gauche
				for (int i = 0; i < avancement; i++) {
					path.x --;
					grid[path.x][path.y] = 1;
				}
				break;

			case(2):
				// cas o� il ne faut pas aller vers le bas :
				// si on est sur les lignes 12 ou 13
				// si en se d�pla�ant de 2 cases vers le bas on retombe sur le chemin
				// quand le virage est trop serr�
				if (direction == 2) {
					if ((path.y > 11) || (grid[path.x][path.y + 2] == 1) || (grid[path.x][path.y + 1] == 1)||(grid[path.x + 1][path.y + 1] == 1) || (grid[path.x - 1][path.y + 1] == 1)) {
						while (direction == 2) {
							direction = rand() % 4;
						}
					}
				}

				while (path.y + avancement > 12) {
					avancement--;
				}

				for (int i = 0; i < avancement; i++) {
					path.y++;
					grid[path.x][path.y] = 1;
				}
				break;
			case(3):
				// cas o� il ne faut pas tourner � droite:
				// quand on est sur la derni�re colonne (path.x>11)
				// si en se d�pla�ant de 2 cases vers la droite on retombe sur le chemin
				// si l'on vient de la droite
				// quand le virage est trop serr�
				if (direction == 3) {
					if ((path.x > 11) || (grid[path.x + 2][path.y] == 1) || (grid[path.x + 1][path.y] == 1) || (grid[path.x +1][path.y+1] == 1) || (grid[path.x + 1][path.y-1] == 1)) {
						while (direction == 3) {
							direction = rand() % 4;
						}
					}
				}
				
				//on d�passe pas de la grille
				while (path.x + avancement > 12) {
					avancement--;
				}
				// on avance vers la droite
				for (int i = 0; i < avancement; i++) {
					path.x = path.x + i;
					grid[path.x][path.y] = 1;
				}
				break;
		}
	

		if (((path.x) == gridSIZE - 2) && ((path.y) == gridSIZE - 2)) {
			arrivee = true;
		}
	}


	return EXIT_SUCCESS;
}