#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

int hash_Course_day(char *course, char *day);

typedef struct CRDH * CRDH;

struct CRDH
{
	char *Course;
	char *room;
	char *day;
	char *hour;
	CRDH next;
};

CRDH new_CRDH(char *c, char *r, char *d, char *h);

void print_CRDH(CRDH c);

void printTable_CRDH(CRDH *database);

int equals_CRDH(CRDH s, char *course, char *room, char *day, char *hour);

LinkedList lookup_CRDH(char *course, char *room, char *day, char *hour, CRDH *database);

void insert_CRDH(char *course, char *room, char *day, char *hour, CRDH *database);