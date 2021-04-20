#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

int hash_Id(int id);
typedef struct SNAP * SNAP;

struct SNAP
{
	int StudentId;
	char *name;
	char *address;
	char *phone;
	SNAP next;
};

SNAP new_SNAP(int s, char *n, char *a, char *p);

void print_SNAP(SNAP s);

void printTable_SNAP(SNAP *database);

int equals_SNAP(SNAP s, char *stId, char *name, char *address, char *phone);

LinkedList lookup_SNAP(char *stId, char *name, char *address, char *phone, SNAP *database);

void insert_SNAP(char *stId, char *name, char *address, char *phone, SNAP *database);

void delete_SNAP(char *stId, char *name, char *address, char *phone, SNAP *database);

extern void printFile_SNAP(char *fileName, SNAP *database);

#pragma once