#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "CDH.h"

int hash_course_day(char *course, char *day)
{
	int hashVal = 0;
	for (int i = 0; i < 5; i++)
	{
		hashVal += course[i];
	}

	return hashVal;
}

int hash_course_day(char *course, char *day);

CDH new_CDH(char *c, char *d, char *h)
{
	CDH this = (CDH) malloc(sizeof(struct CDH));
	this->Course = c;
	this->day = d;
	this->hour = h;
	this->next = NULL;
	return this;
}

void print_CDH(CDH this)
{
	printf("%s \t%s \t%s\n", this->Course, this->day, this->hour);
}

void printTable_CDH(CDH *database)
{
	printf("CDH Table:\n");
	for (int i = 0; i < 1009; i++)
	{
		CDH temp = database[i];
		while (temp != NULL)
		{
			print_CDH(temp);
			temp = temp->next;
		}
	}
}

int equals_CDH(CDH this, char *course, char *day, char *hour)
{
	if (strcmp(this->Course, course) == 0 || strcmp(course, "*") == 0)
	{
		if (strcmp(this->day, day) == 0 || strcmp(day, "*") == 0)
		{
			if (strcmp(this->hour, hour) == 0 || strcmp(hour, "*") == 0)
			{
				return 1;
			}
		}
	}

	return 0;
}

LinkedList lookup_CDH(char *course, char *day, char *hour, CDH *database)
{
	//printf("Items found: ");
	LinkedList list = new_LinkedList();
	if (strcmp(course, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CDH temp = database[i];
			if (temp != NULL)
			{
				CDH curr = database[i];
				while (curr != NULL)
				{
					if (equals_CDH(curr, course, day, hour))
						LinkedList_add_at_end(list, curr);
					curr = curr->next;
				}
			}
		}
	}
	else
	{
		int hash = hash_course_day(course, day);
		CDH curr = database[hash];
		while (curr != NULL)
		{
			if (equals_CDH(curr, course, day, hour))
				LinkedList_add_at_end(list, curr);
			curr = curr->next;
		}
	}

	return list;
}

void insert_CDH(char *course, char *day, char *hour, CDH *database)
{
	if (LinkedList_elementAt(lookup_CDH(course, day, hour, database), 0) != NULL)
	{
		printf("Failed to insert because item is already in list.\n");
		return;
	}

	CDH this = new_CDH(course, day, hour);
	int hash = hash_course_day(course, day);
	CDH curr = database[hash];
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
	print_CDH(this);
	printf("\n");
}

void delete_CDH(char *course, char *day, char *hour, CDH *database)
{
	int delCount = 0;
	if (strcmp(course, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CDH temp = database[i];
			if (temp != NULL)
			{
				while (temp != NULL && equals_CDH(temp, course, day, hour))
				{
					database[i] = temp->next;
					temp = database[i];
					delCount++;
				}

				if (database[i] != NULL && database[i]->next != NULL)
				{
					CDH prev = database[i];
					CDH curr = database[i]->next;
					while (curr != NULL)
					{
						if (equals_CDH(curr, course, day, hour))
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
		int hash = hash_course_day(course, day);
		CDH temp = database[hash];
		if (temp != NULL)
		{
			while (temp != NULL && equals_CDH(temp, course, day, hour))
			{
				database[hash] = temp->next;
				temp = database[hash];
				delCount++;
			}

			if (database[hash] != NULL && database[hash]->next != NULL)
			{
				CDH prev = database[hash];
				CDH curr = database[hash]->next;
				while (curr != NULL)
				{
					if (equals_CDH(curr, course, day, hour))
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

