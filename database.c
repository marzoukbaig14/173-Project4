#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "CSG.h"
#include "SNAP.h"
#include "CP.h"
#include "CR.h"
#include "CDH.h"
#include "CRDH.h"

typedef struct CRDH * CRDH;

//	//Struct for joined CR and CDH table
// struct CRDH {
// 	char* Course;
// 	char* room;
// 	char* day;
// 	char* hour;
// };

//Creates new instance of CRDH
CRDH new_CRDH(char *c, char *r, char *d, char *h)
{
	CRDH this = (CRDH) malloc(sizeof(struct CRDH));
	this->Course = c;
	this->room = r;
	this->day = d;
	this->hour = h;
	return this;
}

//Print CRDH
void print_CRDH(CRDH this)
{
	printf("%s\t %s \t%s \t%s\n", this->Course, this->room, this->day, this->hour);
}

//Executes a query checking what grade a student got in a course
void studentCourseQuery(CSG *CSGdata, SNAP *SNAPdata)
{
	int done = 0;
	char *name = "C. Brown";
	char *course = "CS101";
	while (!done)
	{
		printf("\nWhat grade did StudentName get in CourseName?\n");
		LinkedList SNAPinfo = lookup_SNAP("*", name, "*", "*", SNAPdata);
		if (LinkedList_elementAt(SNAPinfo, 0) == NULL)
		{
			printf("%s does not exist.\n", name);
			done = 1;
		}
		else
		{
			SNAP info = (SNAP) LinkedList_elementAt(SNAPinfo, 0);
			int stuId = info->StudentId;
			char strId[5];
			sprintf(strId, "%d", stuId);
			LinkedList CSGinfo = lookup_CSG(course, strId, "*", CSGdata);
			if (LinkedList_elementAt(CSGinfo, 0) == NULL)
			{
				printf("%s has not taken %s.\n", name, course);
				done = 1;
			}
			else
			{
				CSG info2 = (CSG) LinkedList_elementAt(CSGinfo, 0);
				char *grade = info2->grade;
				printf("%s got a(n) %s in %s.", name, grade, course);
				done = 1;
			}
		}
	}
}

//Executes a query checking what class a student is on a given day
void studentScheduleQuery(CSG *CSGdata, SNAP *SNAPdata, CDH *CDHdata)
{
	int done = 0;
	char *name = "C. Brown";
	char *hour = "9AM";
	char *day = "M";
	while (!done)
	{
		printf("\nWhere is StudentName at Time on Day?\n");
		//printf("Enter StudentName: ");
		//scanf("%s", name);
		//printf("Enter CourseName: ");
		//scanf("%s", course);
		LinkedList SNAPinfo = lookup_SNAP("*", name, "*", "*", SNAPdata);
		if (LinkedList_elementAt(SNAPinfo, 0) == NULL)
		{
			printf("%s does not exist.\n", name);
			done = 1;
		}
		else
		{
			SNAP info = (SNAP) LinkedList_elementAt(SNAPinfo, 0);
			int stuId = info->StudentId;
			char strId[5];
			sprintf(strId, "%d", stuId);
			LinkedList CSGinfo = lookup_CSG("*", strId, "*", CSGdata);
			if (LinkedList_elementAt(CSGinfo, 0) == NULL)
			{
				printf("%s is not currently enrolled in any courses.\n", name);
				done = 1;
			}
			else
			{
				LinkedListIterator iter = LinkedList_iterator(CSGinfo);
				while (LinkedListIterator_hasNext(iter))
				{
					CSG temp = (CSG) LinkedListIterator_next(iter);
					char *course = temp->Course;
					LinkedList courseTimes = lookup_CDH(course, "*", "*", CDHdata);
					LinkedListIterator CDHiter = LinkedList_iterator(courseTimes);
					while (LinkedListIterator_hasNext(CDHiter))
					{
						CDH courseTime = (CDH) LinkedListIterator_next(CDHiter);
						if (strcmp(courseTime->day, day) == 0 && strcmp(courseTime->hour, hour) == 0)
						{
							printf("%s is in %s at %s on %s.\n", name, courseTime->Course, day, hour);
							done = 1;
						}
					}
				}

				done = 1;
			}
		}
	}
}

//Executes Example 8.12
void select_CSG(CSG *database)
{
	LinkedList CS101 = lookup_CSG("CS101", "*", "*", database);
	LinkedListIterator iter = LinkedList_iterator(CS101);
	while (LinkedListIterator_hasNext(iter))
	{
		CSG sel = (CSG) LinkedListIterator_next(iter);
		print_CSG(sel);
	}
}

//Executes Example 8.12
void project_CSG(CSG *database)
{
	LinkedList CS101 = lookup_CSG("CS101", "*", "*", database);
	LinkedListIterator iter = LinkedList_iterator(CS101);
	while (LinkedListIterator_hasNext(iter))
	{
		CSG sel = (CSG) LinkedListIterator_next(iter);
		int id = sel->StudentId;
		printf("%d\n", id);
	}
}

LinkedList join_CR_CDH(CR *CRdata, CDH *CDHdata)
{
	LinkedList CRDHdata = new_LinkedList();
	for (int i = 0; i < 1009; i++)
	{
		CR CRtuple = CRdata[i];
		while (CRtuple != NULL)
		{
			for (int j = 0; j < 1009; j++)
			{
				CDH CDHtuple = CDHdata[j];
				while (CDHtuple != NULL)
				{
					if (strcmp(CRtuple->Course, CDHtuple->Course) == 0)
					{
						CRDH temp = new_CRDH(CRtuple->Course, CRtuple->room, CDHtuple->day, CDHtuple->hour);
						LinkedList_add_at_end(CRDHdata, temp);
					}

					CDHtuple = CDHtuple->next;
				}
			}

			CRtuple = CRtuple->next;
		}
	}

	return CRDHdata;
}

void project_CR_CDH(CR *CRdata, CDH *CDHdata)
{
	LinkedList selected = join_CR_CDH(CRdata, CDHdata);
	LinkedListIterator iter = LinkedList_iterator(selected);
	while (LinkedListIterator_hasNext(iter))
	{
		CRDH sel = (CRDH) LinkedListIterator_next(iter);
		if (sel != NULL && strcmp(sel->room, "Turing Aud.") == 0)
			printf("%s\t %s\n", sel->day, sel->hour);
	}
}

int main()
{
	// Manually populating tables
	printf("\nPopulating CSG table...\n");
	CSG CSGdata[1009];
	for (int i = 0; i < 1009; i++)
	{
		CSGdata[i] = (CSG) malloc(sizeof(struct CSG));
		CSGdata[i] = NULL;
	}

	insert_CSG("CS101", "12345", "A", CSGdata);
	insert_CSG("CS101", "67890", "B", CSGdata);
	insert_CSG("EE200", "12345", "C", CSGdata);
	insert_CSG("EE200", "22222", "B+", CSGdata);
	insert_CSG("CS101", "33333", "A-", CSGdata);
	insert_CSG("PH100", "67890", "C+", CSGdata);
	printTable_CSG(CSGdata);

	printf("\nPopulating SNAP table...\n");
	SNAP SNAPdata[1009];
	for (int i = 0; i < 1009; i++)
	{
		SNAPdata[i] = (SNAP) malloc(sizeof(struct SNAP));
		SNAPdata[i] = NULL;
	}

	insert_SNAP("12345", "C. Brown", "12 Apple St.", "555-1234", SNAPdata);
	insert_SNAP("67890", "L. Van Pelt", "34 Pear Ave.", "555-5678", SNAPdata);
	insert_SNAP("22222", "P. Patty", "56 Grape Blvd.", "555-9999", SNAPdata);
	printTable_SNAP(SNAPdata);

	printf("\nPopulating CP table...\n");
	CP CPdata[1009];
	for (int i = 0; i < 1009; i++)
	{
		CPdata[i] = (CP) malloc(sizeof(struct CP));
		CPdata[i] = NULL;
	}

	insert_CP("CS101", "CS100", CPdata);
	insert_CP("EE200", "EE005", CPdata);
	insert_CP("EE200", "CS100", CPdata);
	insert_CP("CS120", "CS101", CPdata);
	insert_CP("CS121", "CS120", CPdata);
	insert_CP("CS205", "CS101", CPdata);
	insert_CP("CS206", "CS121", CPdata);
	insert_CP("CS206", "CS205", CPdata);
	printTable_CP(CPdata);

	CDH CDHdata[1009];
	for (int i = 0; i < 1009; i++)
	{
		CDHdata[i] = (CDH) malloc(sizeof(struct CDH));
		CDHdata[i] = NULL;
	}

	printf("\nPopulating CDH table...\n");
	insert_CDH("CS101", "M", "9AM", CDHdata);
	insert_CDH("CS101", "W", "9AM", CDHdata);
	insert_CDH("CS101", "F", "9AM", CDHdata);
	insert_CDH("EE200", "Tu", "10AM", CDHdata);
	insert_CDH("EE200", "W", "1PM", CDHdata);
	insert_CDH("EE200", "Th", "10AM", CDHdata);
	printTable_CDH(CDHdata);

	printf("\nPopulating CR table...\n");
	CR CRdata[1009];
	for (int i = 0; i < 1009; i++)
	{
		CRdata[i] = (CR) malloc(sizeof(struct CR));
		CRdata[i] = NULL;
	}

	insert_CR("CS101", "Turing Aud.", CRdata);
	insert_CR("EE200", "25 Ohm Hall", CRdata);
	insert_CR("PH100", "Newton Lab.", CRdata);
	printTable_CR(CRdata);

	//Part 1: Testing Insert, Delete, and Lookup functions

	printf("\nDemonstrating examples in Part 1...\n");
	LinkedList lookCSG = lookup_CSG("CS101", "12345", "*", CSGdata);
	LinkedListIterator iter3 = LinkedList_iterator(lookCSG);
	printf("\nItems found:\n");
	while (LinkedListIterator_hasNext(iter3))
	{
		CSG temp = (CSG) LinkedListIterator_next(iter3);
		print_CSG(temp);
	}

	lookup_CP("CS205", "CS120", CPdata);
	LinkedList lookCP = lookup_CP("CS205", "CS120", CPdata);
	LinkedListIterator iter2 = LinkedList_iterator(lookCP);
	printf("Items found:\n");
	while (LinkedListIterator_hasNext(iter2))
	{
		CP temp = (CP) LinkedListIterator_next(iter2);
		print_CP(temp);
	}

	delete_CR("CS101", "*", CRdata);
	insert_CP("CS205", "CS120", CPdata);
	insert_CP("CS205", "CS101", CPdata);

	char *CSGfile = "CSGtable.txt";
	printFile_CSG(CSGfile, CSGdata);
	char *SNAPfile = "SNAPtable.txt";
	printFile_SNAP(SNAPfile, SNAPdata);
	char *CPfile = "CPtable.txt";
	printFile_CP(CPfile, CPdata);
	char *CRfile = "CRtable.txt";
	printFile_CR(CRfile, CRdata);
	char *CDHfile = "CDHtable.txt";
	printFile_CDH(CDHfile, CDHdata);

	printf("Reading CSGtable.txt\n");
	readFile_CSG(CSGfile);

	// Part 2: Testing Queries

	printf("\nDemonstrating queries for Part 2...\n");
	studentCourseQuery(CSGdata, SNAPdata);
	studentScheduleQuery(CSGdata, SNAPdata, CDHdata);

	// Part 3: Testing Relational Algebra

	printf("\nDemonstrating relational algebra examples for Part 3...\n");
	printf("\nSelecting CS101 in CSG:\n");
	select_CSG(CSGdata);
	printf("\nProjecting StudentId in selection of CS101 in CSG:\n");
	project_CSG(CSGdata);

	//Reinserting into CR table for next two relational algebra examples
	insert_CR("CS101", "Turing Aud.", CRdata);

	LinkedList CRDHdata = join_CR_CDH(CRdata, CDHdata);

	printf("CRDH Table:\n");
	LinkedListIterator iter = LinkedList_iterator(CRDHdata);
	while (LinkedListIterator_hasNext(iter))
	{
		CRDH temp = (CRDH) LinkedListIterator_next(iter);
		printf("%s\t %s\t %s\t %s\n", temp->Course, temp->room, temp->day, temp->hour);
	}

	printf("\nProjecting Day and Hour in selection of Turing Aud. in joined CR and CDH table:\n");
	project_CR_CDH(CRdata, CDHdata);

}