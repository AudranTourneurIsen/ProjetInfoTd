#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conio.h"
#include <stdbool.h>

#define gridSIZE 14
#define stringSIZE 2000

typedef struct cursor {
	int x;
	int y;
}cursor;

void print_tab(int tab[gridSIZE][gridSIZE]);
char* cleanTab_tostring(int tab[gridSIZE][gridSIZE]);
// char* tabtostring(char tab[gridSIZE][gridSIZE]);
void nettoyagefinstring(char* string, int size);