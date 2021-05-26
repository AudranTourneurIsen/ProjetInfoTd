#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conio.h"
#include <stdbool.h>

#define gridSIZE 14
#define stringSIZE 2000

#define NOTHING '.'
#define PATH 'x'
#define TURRET '$'

typedef struct Cursor {
	int x;
	int y;
	int reach;
} Cursor;

typedef struct Enemy {
	char name;
	int index;
	int hp;
	int property;  // 0 = none | 1 = fire | 2 =  ice
	int x;
	int y;
} Enemy;

typedef struct Turret {
	char name[32];
	int damage;
	int range;
	int property;  // 0 = none | 1 = fire | 2 =  ice
} Turret;

typedef struct Wave {
	int index;
	int gold;
	char enemies[256];
	//Enemy* enemies;
	//char* enemies;
} Wave;

void print_tab(int tab[gridSIZE][gridSIZE]);
char* cleanTab_tostring(int tab[gridSIZE][gridSIZE]);
void nettoyagefinstring(char* string, int size);