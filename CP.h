#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

int hash_Course_Prereq(char *course, char *prereq);

typedef struct CP * CP;

struct CP
{
	char *Course;
	char *prereq;
	CP next;
};

CP new_CP(char *c, char *p);

void print_CP(CP c);

void printTable_CP(CP *database);

int equals_CP(CP c, char *course, char *prereq);

LinkedList lookup_CP(char *course, char *prereq, CP *database);

void insert_CP(char *course, char *prereq, CP *database);

void delete_CP(char *course, char *prereq, CP *database);

