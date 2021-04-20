#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "CRDH.h"

int hash_Course_day(char *course, char *day)
{
	int hashVal = 0;
	for (int i = 0; i < 5; i++)
	{
		hashVal += course[i];
	}

	return hashVal;
}

typedef struct CRDH * CRDH;

CRDH new_CRDH(char *c, char *r, char *d, char *h)
{
	CRDH this = (CRDH) malloc(sizeof(struct CRDH));
	this->Course = c;
	this->room = r;
	this->day = d;
	this->hour = h;
	return this;
}

void print_CRDH(CRDH this)
{
	printf("%s\t %s \t%s \t%s\n", this->Course, this->room, this->day, this->hour);
}

void printTable_CRDH(CRDH *database)
{
	printf("CRDH Table:\n");
	for (int i = 0; i < 1009; i++)
	{
		CRDH temp = database[i];
		while (temp != NULL)
		{
			print_CRDH(temp);
			temp = temp->next;
		}
	}
}

int equals_CRDH(CRDH this, char *course, char *room, char *day, char *hour)
{
	if (strcmp(this->Course, course) == 0 || strcmp(course, "*") == 0)
	{
		if (strcmp(this->room, room) == 0 || strcmp(room, "*") == 0)
		{
			if (strcmp(this->day, day) == 0 || strcmp(day, "*") == 0)
			{
				if (strcmp(this->hour, hour) == 0 || strcmp("*", hour) == 0)
					return 1;
			}
		}
	}

	return 0;
}

LinkedList lookup_CRDH(char *course, char *room, char *day, char *hour, CRDH *database)
{
	//printf("Items found:\n");
	LinkedList list = new_LinkedList();
	if (strcmp(course, "*") == 0 || strcmp(day, "*") == 0)
	{
		for (int i = 0; i < 1009; i++)
		{
			CRDH temp = database[i];
			if (temp != NULL)
			{
				CRDH curr = database[i];
				while (curr != NULL)
				{
					if (equals_CRDH(curr, course, room, day, hour))
						LinkedList_add_at_end(list, curr);
					curr = curr->next;
				}
			}
		}
	}
	else
	{
		int hash = hash_Course_day(course, day);
		CRDH curr = database[hash];
		while (curr != NULL)
		{
			if (equals_CRDH(curr, course, room, day, hour))
				LinkedList_add_at_end(list, curr);
			curr = curr->next;
		}
	}

	return list;
}

void insert_CRDH(char *course, char *room, char *day, char *hour, CRDH *database)
{
	if (LinkedList_elementAt(lookup_CRDH(course, room, day, hour, database), 0) != NULL)
	{
		//printf("Failed to insert because item is already in list.\n");
		return;
	}

	CRDH this = new_CRDH(course, room, day, hour);
	int hash = hash_Course_day(course, day);
	//printf("Hash: %d", hash);
	CRDH curr = database[hash];
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

	//printf("Inserted.\n");
}