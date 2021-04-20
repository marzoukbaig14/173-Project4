#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

int hash_Course(char *course);
typedef struct CR * CR;

struct CR
{
	char *Course;
	char *room;
	CR next;
};

CR new_CR(char *c, char *r);

void print_CR(CR c);

void printTable_CR(CR *database);

int equals_CR(CR c, char *course, char *room);

LinkedList lookup_CR(char *course, char *room, CR *database);

void insert_CR(char *course, char *room, CR *database);

void delete_CR(char *course, char *room, CR *database);

