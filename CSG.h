#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

int hash_Course_Id(char *course, int id);
typedef struct CSG * CSG;

struct CSG
{
	char *Course;
	int StudentId;
	char *grade;
	CSG next;
};

CSG new_CSG(char *c, int s, char *g);

void print_CSG(CSG c);

void printTable_CSG(CSG *database);

int equals_CSG(CSG c, char *course, char *stId, char *grade);

LinkedList lookup_CSG(char *course, char *stId, char *grade, CSG *database);

void insert_CSG(char *course, char *stId, char *grade, CSG *database);

void delete_CSG(char *course, char *stId, char *grade, CSG *database);

// extern void printFile_CSG(char *fileName, CSG *database);
// extern CSG* readFile_CSG(char *fileName);