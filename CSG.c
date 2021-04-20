#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "CSG.h"

int hash_Course_Id(char *course, int id)
{
	int hashVal = 0;
	for (int i = 0; i < 5; i++)
	{
		hashVal += course[i];
	}

	return ((hashVal + id) % 1009);
}

typedef struct CSG * CSG;

// struct CSG {
// 	char* Course;
// 	int StudentId;
// 	char* grade;
// 	CSG next;
// };

//Creates new CSG tuple
CSG new_CSG(char *c, int s, char *g)
{
	CSG this = (CSG) malloc(sizeof(struct CSG));
	this->Course = c;
	this->StudentId = s;
	this->grade = g;
	this->next = NULL;
	return this;
}

//Prints CSG tuple
void print_CSG(CSG this)
{
	printf("%s\t %d\t %s\n", this->Course, this->StudentId, this->grade);
}

//Prints CSG table
void printTable_CSG(CSG *database)
{
	printf("CSG Table:\n");
	for (int i = 0; i < 1009; i++)
	{
		CSG temp = database[i];
		while (temp != NULL)
		{
			print_CSG(temp);
			temp = temp->next;
		}
	}
}

//Checks equality between tuples
int equals_CSG(CSG this, char *course, char *stId, char *grade)
{
	if (strcmp(this->Course, course) == 0 || strcmp(course, "*") == 0)
	{
		if (this->StudentId == atoi(stId) || strcmp(stId, "*") == 0)
		{
			if (strcmp(this->grade, grade) == 0 || strcmp(grade, "*") == 0)
			{
				return 1;
			}
		}
	}

	return 0;
}

//Looks for tuple matching specified information
LinkedList lookup_CSG(char *course, char *stId, char *grade, CSG *database)
{
	LinkedList list = new_LinkedList();
	if (strcmp(course, "*") == 0 || strcmp(stId, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CSG temp = database[i];
			if (temp != NULL)
			{
				CSG curr = database[i];
				while (curr != NULL)
				{
					if (equals_CSG(curr, course, stId, grade))
						LinkedList_add_at_end(list, curr);
					curr = curr->next;
				}
			}
		}
	}
	else
	{
		int hash = hash_Course_Id(course, atoi(stId));
		CSG curr = database[hash];
		while (curr != NULL)
		{
			if (equals_CSG(curr, course, stId, grade))
				LinkedList_add_at_end(list, curr);
			curr = curr->next;
		}
	}

	return list;
}

//Inserts tuple into table
void insert_CSG(char *course, char *stId, char *grade, CSG *database)
{
	if (LinkedList_elementAt(lookup_CSG(course, stId, grade, database), 0) != NULL)
	{
		printf("Failed to insert because item is already in list.\n");
		return;
	}

	CSG this = new_CSG(course, atoi(stId), grade);
	int hash = hash_Course_Id(course, atoi(stId));
	CSG curr = database[hash];
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
	print_CSG(this);
	printf("\n");
}

//Deletes tuple matching specified information
void delete_CSG(char *course, char *stId, char *grade, CSG *database)
{
	int delCount = 0;
	if (strcmp(course, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CSG temp = database[i];
			if (temp != NULL)
			{
				while (equals_CSG(temp, course, stId, grade) && temp != NULL)
				{
					database[i] = temp->next;
					temp = database[i];
					delCount++;
				}

				if (database[i] != NULL && database[i]->next != NULL)
				{
					CSG prev = database[i];
					CSG curr = database[i]->next;
					while (curr != NULL)
					{
						if (equals_CSG(curr, course, stId, grade))
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
		int hash = hash_Course_Id(course, atoi(stId));
		CSG temp = database[hash];
		if (temp != NULL)
		{
			while (temp != NULL && equals_CSG(temp, course, stId, grade))
			{
				database[hash] = temp->next;
				temp = database[hash];
				delCount++;
			}

			if (database[hash] != NULL && database[hash]->next != NULL)
			{
				CSG prev = database[hash];
				CSG curr = database[hash]->next;
				while (curr != NULL)
				{
					if (equals_CSG(curr, course, stId, grade))
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

// //Prints table to file
// void printFile_CSG(char *fileName, CSG *database)
// {
// 	printf("Printing to %s.\n", fileName);
// 	FILE *file = fopen(fileName, "w+");
// 	if (file == NULL)
// 	{
// 		printf("Can't open %s\n", fileName);
// 		exit(EXIT_FAILURE);
// 	}

// 	fprintf(file, "CSG Table:\n");
// 	fprintf(file, "Course\tStudent ID\tGrade\n");
// 	for (int i = 0; i < 1009; i++)
// 	{
// 		CSG temp = database[i];
// 		while (temp != NULL)
// 		{
// 			fprintf(file, "%s\t%d\t\t%s\n", temp->Course, temp->StudentId, temp->grade);
// 			temp = temp->next;
// 		}
// 	}

// 	fclose(file);
// }

// //Reads table from file
// CSG* readFile_CSG(char *fileName)
// {
// 	FILE *file = fopen(fileName, "r");
// 	if (file == NULL)
// 	{
// 		printf("Can't open %s\n", fileName);
// 		exit(EXIT_FAILURE);
// 	}

// 	if (!feof(file))
// 	{
// 		fscanf(file, "%*[^\n]\n");	//Skip first 2 lines
// 		fscanf(file, "%*[^\n]\n");
// 	}

// 	CSG CSGdata[1009];
// 	for (int i = 0; i < 1009; i++)
// 	{
// 		CSGdata[i] = (CSG) malloc(sizeof(struct CSG));
// 		CSGdata[i] = NULL;
// 	}

// 	char course[5];
// 	char *grade = (char*) calloc(2, sizeof(char));
// 	char studentId[5];
// 	while (!feof(file))
// 	{
// 		fscanf(file, "%s\t", course);
// 		printf("%s\n", course);
// 		fscanf(file, "%s\t\t", studentId);
// 		printf("%s\n", studentId);
// 		fscanf(file, "%s\n", grade);
// 		printf("%s\n", grade);
// 		insert_CSG(course, studentId, grade, CSGdata);
// 	}

// 	fclose(file);
// 	CSG *ptr = CSGdata;
// 	return ptr;
// }
