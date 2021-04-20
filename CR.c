#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "CR.h"

//Hashes based on Course
int hash_Course(char *course)
{
	int hashVal = 0;
	for (int i = 0; i < 5; i++)
	{
		hashVal += course[i];
	}

	return (hashVal % 1009);
}

typedef struct CR * CR;

//Creates new CR tuple
CR new_CR(char *c, char *r)
{
	CR this = (CR) malloc(sizeof(struct CR));
	this->Course = c;
	this->room = r;
	this->next = NULL;
	return this;
}

//Prints CR tuple
void print_CR(CR this)
{
	printf("%s \t%s\n", this->Course, this->room);
}

//Prints CR table
void printTable_CR(CR *database)
{
	printf("Course\t Room\n");
	for (int i = 0; i < 1009; i++)
	{
		CR temp = database[i];
		while (temp != NULL)
		{
			print_CR(temp);
			temp = temp->next;
		}
	}
}

//Checks equality between tuples
int equals_CR(CR this, char *course, char *room)
{
	if (strcmp(this->Course, course) == 0 || strcmp(course, "*") == 0)
	{
		if (strcmp(this->room, room) == 0 || strcmp(room, "*") == 0)
		{
			return 1;
		}
	}

	return 0;
}

//Looks for a tuple containing the specified information
LinkedList lookup_CR(char *course, char *room, CR *database)
{
	//printf("Items found:\n");
	LinkedList list = new_LinkedList();
	if (strcmp(course, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CR temp = database[i];
			if (temp != NULL)
			{
				CR curr = database[i];
				while (curr != NULL)
				{
					if (equals_CR(curr, course, room))
						LinkedList_add_at_end(list, curr);
					curr = curr->next;
				}
			}
		}
	}
	else
	{
		int hash = hash_Course(course);
		CR curr = database[hash];
		while (curr != NULL)
		{
			if (equals_CR(curr, course, room))
				LinkedList_add_at_end(list, curr);
			curr = curr->next;
		}
	}

	return list;
}

//Inserts tuple into relation
void insert_CR(char *course, char *room, CR *database)
{
	if (LinkedList_elementAt(lookup_CR(course, room, database), 0) != NULL)
	{
		printf("Failed to insert because item is already in list.\n");
		return;
	}

	CR this = new_CR(course, room);
	int hash = hash_Course(course);
	CR curr = database[hash];
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
	print_CR(this);
	printf("\n");
}

//Deletes tuple matching specified information
void delete_CR(char *course, char *room, CR *database)
{
	int delCount = 0;
	if (strcmp(course, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CR temp = database[i];
			if (temp != NULL)
			{
				while (temp != NULL && equals_CR(temp, course, room))
				{
					database[i] = temp->next;
					temp = database[i];
					delCount++;
				}

				if (database[i] != NULL && database[i]->next != NULL)
				{
					CR prev = database[i];
					CR curr = database[i]->next;
					while (curr != NULL)
					{
						if (equals_CR(curr, course, room))
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
		int hash = hash_Course(course);
		CR temp = database[hash];
		if (temp != NULL)
		{
			while (temp != NULL && equals_CR(temp, course, room))
			{
				database[hash] = temp->next;
				temp = database[hash];
				delCount++;
			}

			if (database[hash] != NULL && database[hash]->next != NULL)
			{
				CR prev = database[hash];
				CR curr = database[hash]->next;
				while (curr != NULL)
				{
					if (equals_CR(curr, course, room))
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

//Prints table to file
void printFile_CR(char *fileName, CR *database)
{
	printf("Printing to %s.\n", fileName);
	FILE *file = fopen(fileName, "w+");
	if (file == NULL)
	{
		printf("Can't open %s\n", fileName);
		exit(EXIT_FAILURE);
	}

	fprintf(file, "CR Table:\n");
	fprintf(file, "Course\tRoom\n");
	for (int i = 0; i < 1009; i++)
	{
		CR temp = database[i];
		while (temp != NULL)
		{
			fprintf(file, "%s\t%s\n", temp->Course, temp->room);
			temp = temp->next;
		}
	}

	fclose(file);
}

//Reads table from file
CR* readFile_CR(char *fileName)
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

	CR CRdata[1009];
	for (int i = 0; i < 1009; i++)
	{
		CRdata[i] = (CR) malloc(sizeof(struct CR));
		CRdata[i] = NULL;
	}

	char Course[5];
	char *Room = (char*) calloc(255, sizeof(char));
	while (!feof(file))
	{
		fscanf(file, "%s\t\t", Course);

		fscanf(file, "%s\t\t", Room);
		insert_CR(Course, Room, CRdata);
	}

	fclose(file);
	CR *ptr = CRdata;
	return ptr;
}