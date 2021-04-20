#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

int hash_Course_Day(char *course, char *day);

typedef struct CDH * CDH;

struct CDH
{
	char *Course;
	char *day;
	char *hour;
	CDH next;
};

CDH new_CDH(char *c, char *d, char *h);

void print_CDH(CDH c);

void printTable_CDH(CDH *database);

int equals_CDH(CDH c, char *course, char *day, char *hour);

LinkedList lookup_CDH(char *course, char *day, char *hour, CDH *database);

void insert_CDH(char *course, char *day, char *hour, CDH *database);

void delete_CDH(char *course, char *day, char *hour, CDH *database);


#pragma once