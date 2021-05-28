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