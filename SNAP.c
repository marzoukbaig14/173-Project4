#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "SNAP.h"

int hash_Id(int id)
{
	return (id % 1009);
}

typedef struct SNAP * SNAP;

SNAP new_SNAP(int s, char *n, char *a, char *p)
{
	SNAP this = (SNAP) malloc(sizeof(struct SNAP));
	this->StudentId = s;
	this->name = n;
	this->address = a;
	this->phone = p;
	this->next = NULL;
	return this;
}

void print_SNAP(SNAP this)
{
	printf("%d\t %s \t%s \t%s\n", this->StudentId, this->name, this->address, this->phone);
}

void printTable_SNAP(SNAP *database)
{
	printf("SNAP Table:\n");
	for (int i = 0; i < 1009; i++)
	{
		SNAP temp = database[i];
		while (temp != NULL)
		{
			print_SNAP(temp);
			temp = temp->next;
		}
	}
}

int equals_SNAP(SNAP this, char *stId, char *name, char *address, char *phone)
{
	if (this->StudentId == atoi(stId) || strcmp(stId, "*") == 0)
	{
		if (strcmp(this->name, name) == 0 || strcmp(name, "*") == 0)
		{
			if (strcmp(this->address, address) == 0 || strcmp(address, "*") == 0)
			{
				if (strcmp(this->phone, phone) == 0 || strcmp("*", phone) == 0)
					return 1;
			}
		}
	}

	return 0;
}

LinkedList lookup_SNAP(char *stId, char *name, char *address, char *phone, SNAP *database)
{
	//printf("Items found:\n");
	LinkedList list = new_LinkedList();
	if (strcmp(stId, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			SNAP temp = database[i];
			if (temp != NULL)
			{
				SNAP curr = database[i];
				while (curr != NULL)
				{
					if (equals_SNAP(curr, stId, name, address, phone))
						LinkedList_add_at_end(list, curr);
					curr = curr->next;
				}
			}
		}
	}
	else
	{
		int hash = hash_Id(atoi(stId));
		SNAP curr = database[hash];
		while (curr != NULL)
		{
			if (equals_SNAP(curr, stId, name, address, phone))
				LinkedList_add_at_end(list, curr);
			curr = curr->next;
		}
	}

	return list;
}

void insert_SNAP(char *stId, char *name, char *address, char *phone, SNAP *database)
{
	if (LinkedList_elementAt(lookup_SNAP(stId, name, address, phone, database), 0) != NULL)
	{
		printf("Failed to insert because item is already in list.\n");
		return;
	}

	SNAP this = new_SNAP(atoi(stId), name, address, phone);
	int hash = hash_Id(atoi(stId));
	//printf("Hash: %d", hash);
	SNAP curr = database[hash];
	if (curr == NULL)
	{
		database[hash] = this;
	}
	else
	{
		while (curr->next != NULL)
		{
			curr = curr->next;
		}

		curr->next = this;
	}

	printf("Successfully inserted ");
	print_SNAP(this);
	printf("\n");
}

void delete_SNAP(char *stId, char *name, char *address, char *phone, SNAP *database)
{
	int delCount = 0;
	if (strcmp(stId, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			SNAP temp = database[i];
			if (temp != NULL)
			{
				while (equals_SNAP(temp, stId, name, address, phone) && temp != NULL)
				{
					database[i] = temp->next;
					temp = database[i];
					delCount++;
				}

				if (database[i] != NULL && database[i]->next != NULL)
				{
					SNAP prev = database[i];
					SNAP curr = database[i]->next;
					while (curr != NULL)
					{
						if (equals_SNAP(curr, stId, name, address, phone))
						{
							prev->next = curr->next;
							curr = prev->next;
							delCount++;
						}
						else
						{
							prev = prev->next;
							curr = curr->next;
						}
					}
				}
			}
		}
	}
	else
	{
		int hash = hash_Id(atoi(stId));
		SNAP temp = database[hash];
		if (temp != NULL)
		{
			while (temp != NULL && equals_SNAP(temp, stId, name, address, phone))
			{
				database[hash] = temp->next;
				temp = database[hash];
				delCount++;
			}

			if (database[hash] != NULL && database[hash]->next != NULL)
			{
				SNAP prev = database[hash];
				SNAP curr = database[hash]->next;
				while (curr != NULL)
				{
					if (equals_SNAP(curr, stId, name, address, phone))
					{
						prev->next = curr->next;
						curr = prev->next;
						delCount++;
					}
					else
					{
						prev = prev->next;
						curr = curr->next;
					}
				}
			}
		}
	}

	printf("%d item(s) deleted.\n", delCount);
}

void printFile_SNAP(char *fileName, SNAP *database)
{
	printf("Printing to %s.\n", fileName);
	FILE *file = fopen(fileName, "w+");
	if (file == NULL)
	{
		printf("Can't open %s\n", fileName);
		exit(EXIT_FAILURE);
	}

	fprintf(file, "SNAP Table:\n");
	fprintf(file, "Student ID\tName\t\tAddress\t\tPhone Number\n");
	for (int i = 0; i < 1009; i++)
	{
		SNAP temp = database[i];
		while (temp != NULL)
		{
			fprintf(file, "%d\t\t%s\t%s\t%s\n", temp->StudentId, temp->name, temp->address, temp->phone);
			temp = temp->next;
		}
	}

	fclose(file);
}

SNAP* readFile_SNAP(char *fileName)
{
	FILE *file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("Can't open %s\n", fileName);
		exit(EXIT_FAILURE);
	}

	if (!feof(file))
	{
		fscanf(file, "%*[^\n]\n");	//Skip first 2 lines
		fscanf(file, "%*[^\n]\n");
	}

	SNAP SNAPdata[1009];
	for (int i = 0; i < 1009; i++)
	{
		SNAPdata[i] = (SNAP) malloc(sizeof(struct SNAP));
		SNAPdata[i] = NULL;
	}

	char StudentId[5];
	char *name = (char*) calloc(255, sizeof(char));
	char *address = (char*) calloc(255, sizeof(char));
	char phone[9];
	while (!feof(file))
	{
		fscanf(file, "%s\t\t", StudentId);
		fscanf(file, "%s\t", name);
		fscanf(file, "%s\t", address);
		fscanf(file, "%s\n", phone);
		insert_SNAP(StudentId, name, address, phone, SNAPdata);
	}

	fclose(file);
	SNAP *ptr = SNAPdata;
	return ptr;
}