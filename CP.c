#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "CP.h"

int hash_Course_Prereq(char *course, char *prereq)
{
	int hashVal = 0;
	for (int i = 0; i < 5; i++)
	{
		hashVal += course[i] + prereq[i];
	}

	return (hashVal % 1009);
}

typedef struct CP * CP;

CP new_CP(char *c, char *p)
{
	CP this = (CP) malloc(sizeof(struct CP));
	this->Course = c;
	this->prereq = p;
	this->next = NULL;
	return this;
}

void print_CP(CP this)
{
	printf("%s \t%s\n", this->Course, this->prereq);
}

void printTable_CP(CP *database)
{
	printf("CP Table:\n");
	for (int i = 0; i < 1009; i++)
	{
		CP temp = database[i];
		while (temp != NULL)
		{
			print_CP(temp);
			temp = temp->next;
		}
	}
}

int equals_CP(CP this, char *course, char *prereq)
{
	if (strcmp(this->Course, course) == 0 || strcmp(course, "*") == 0)
	{
		if (strcmp(this->prereq, prereq) == 0 || strcmp(prereq, "*") == 0)
		{
			return 1;
		}
	}

	return 0;
}

LinkedList lookup_CP(char *course, char *prereq, CP *database)
{
	//printf("Items found:\n");
	LinkedList list = new_LinkedList();
	if (strcmp(course, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CP temp = database[i];
			if (temp != NULL)
			{
				CP curr = database[i];
				while (curr != NULL)
				{
					if (equals_CP(curr, course, prereq))
						LinkedList_add_at_end(list, curr);
					curr = curr->next;
				}
			}
		}
	}
	else
	{
		int hash = hash_Course_Prereq(course, prereq);
		CP curr = database[hash];
		while (curr != NULL)
		{
			if (equals_CP(curr, course, prereq))
				LinkedList_add_at_end(list, curr);
			curr = curr->next;
		}
	}

	return list;
}

void insert_CP(char *course, char *prereq, CP *database)
{
	if (LinkedList_elementAt(lookup_CP(course, prereq, database), 0) != NULL)
	{
		printf("Failed to insert because item is already in list.\n");
		return;
	}

	CP this = new_CP(course, prereq);
	int hash = hash_Course_Prereq(course, prereq);
	CP curr = database[hash];
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
	print_CP(this);
	printf("\n");
}

void delete_CP(char *course, char *prereq, CP *database)
{
	int delCount = 0;
	if (strcmp(course, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CP temp = database[i];
			if (temp != NULL)
			{
				while (temp != NULL && equals_CP(temp, course, prereq))
				{
					database[i] = temp->next;
					temp = database[i];
					delCount++;
				}

				if (database[i] != NULL && database[i]->next != NULL)
				{
					CP prev = database[i];
					CP curr = database[i]->next;
					while (curr != NULL)
					{
						if (equals_CP(curr, course, prereq))
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
		int hash = hash_Course_Prereq(course, prereq);
		CP temp = database[hash];
		if (temp != NULL)
		{
			while (temp != NULL && equals_CP(temp, course, prereq))
			{
				database[hash] = temp->next;
				temp = database[hash];
				delCount++;
			}

			if (database[hash] != NULL && database[hash]->next != NULL)
			{
				CP prev = database[hash];
				CP curr = database[hash]->next;
				while (curr != NULL)
				{
					if (equals_CP(curr, course, prereq))
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

void printFile_CP(char *fileName, CP *database)
{
	printf("Printing to %s.\n", fileName);
	FILE *file = fopen(fileName, "w+");
	if (file == NULL)
	{
		printf("Can't open %s\n", fileName);
		exit(EXIT_FAILURE);
	}

	fprintf(file, "CP Table:\n");
	fprintf(file, "Course\tPrerequisite\n");
	for (int i = 0; i < 1009; i++)
	{
		CP temp = database[i];
		while (temp != NULL)
		{
			fprintf(file, "%s\t%s\n", temp->Course, temp->prereq);
			temp = temp->next;
		}
	}

	fclose(file);
}

CP* readFile_CP(char *fileName)
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

	CP CPdata[1009];
	for (int i = 0; i < 1009; i++)
	{
		CPdata[i] = (CP) malloc(sizeof(struct CP));
		CPdata[i] = NULL;
	}

	char Course[5];
	char Prereq[5];
	while (!feof(file))
	{
		fscanf(file, "%s\t", Course);
		fscanf(file, "%s\n", Prereq);
		insert_CP(Course, Prereq, CPdata);
	}

	fclose(file);
	CP *ptr = CPdata;
	return ptr;
}

