#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NAME 99

typedef enum { false, true }BOOL;

typedef struct Student
{
	char* Name;
	long  ID;
	float MtmGrade;
	char  LGrades[5];
	char  FGrade;
}Student, * PStudent;

typedef struct University
{
	PStudent Students;
	int SCount;
}University;

void Menu_UI_Line(int count);
void Menu_Main(University* uni, FILE* in, FILE* out);
void Menu_Option_A(University* uni, FILE* in);
void Menu_Option_B(University* uni, FILE* out);
void Menu_Option_C(University* uni, FILE* out);
void Menu_Option_D(University* uni, FILE* out);
void Menu_Option_E(University* uni, FILE* out);
void Menu_Option_F(FILE* out);
int Student_HasPassedLabs(Student student);
double Student_CalculateFinalGrade(Student student);

void Free_Program_Memory(University* uni);
void Error_Msg(char* s);

void main()
{
	FILE* in, * out;
	University University = { 0 };

	if (!(in = fopen("input.txt", "r")))
		Error_Msg("Failed to read file. Exiting...");
	if (!(out = fopen("output.txt", "w")))
		Error_Msg("Failed to create file. Exiting...");

	/* Prompt Application Menu. */
	Menu_Main(&University, in, out);

	fclose(in);
	fclose(out);
	Free_Program_Memory(&University);
}

/* The function prints colored dashes in the console using the input 'count' as to how many to dashes. */
void Menu_UI_Line(int count)
{
	printf("\033[0;32m");
	putchar('+');

	while (count-- > 0)
		putchar('-');

	putchar('+');
	printf("\n\033[0m");
}

/* The function prompts the main menu of the Information System, it requires a University structure and input & output file streams. */
void Menu_Main(University* uni, FILE* in, FILE* out)
{
	BOOL _OPTION_A = false;
	char option;

	/* Print UI Header. */
	Menu_UI_Line(90);
	printf("\t\t\t\t\033[1;32mUniversity Information System\033[0m\n");
	Menu_UI_Line(90);
	printf("\ta. Input Students Data\n\tb. Output Students Data\n\tc. Output Students Final Grade\n"
		"\td. Output Statistical Data\n\te. Output Students Blacklist\n\tf. Exit The Program\n"
		"\n\033[1;33m/!\\ Type 'a' to get started /!\\ \033[0m\n");
	Menu_UI_Line(90);

	do
	{
		printf("Select an option <a-f>: ");
		scanf("%c%*c", &option);

		/* Makes sure the client inputs data into memory before doing any operations. */
		if (!_OPTION_A && option != 'a' && option != 'f')
		{
			printf("\033[0;31mType 'a' to get started \033[0m\n");
			continue;
		}

		switch (option)
		{
			case 'a':
				/* Prevent multiple uses of the option. */
				if (_OPTION_A)
				{
					printf("\033[0;31mOption 'a' cannot be used more than once!\033[0m\n");
					continue;
				}
				Menu_Option_A(uni, in);
				_OPTION_A = true;
				break;
			case 'b':
				Menu_Option_B(uni, out);
				break;
			case 'c':
				Menu_Option_C(uni, out);
				break;
			case 'd':
				Menu_Option_D(uni, out);
				break;
			case 'e':
				Menu_Option_E(uni, out);
				break;
			case 'f':
				Menu_Option_F(out);
				break;
			default:
				printf("\033[0;31mInvalid input!\033[0m\n");
				break;
		}
	} while (option != 'f');
}

/* The function inputs data from a given file stream and inserts it into a University structure. */
void Menu_Option_A(University* uni, FILE* in)
{
	PStudent TempList;
	char NameBuffer[MAX_NAME + 1];

	while (fscanf(in, "%s", NameBuffer) != EOF)
	{
		TempList = (PStudent)realloc(uni->Students, (uni->SCount + 1) * sizeof(Student));
		if (TempList == NULL)
		{
			Free_Program_Memory(uni);
			Error_Msg("Failed to allocate memory. Exiting..");
		}
		uni->Students = TempList;

		uni->Students[uni->SCount].Name = (char*)calloc((strlen(NameBuffer) + 1), sizeof(char));
		if ((*uni).Students[uni->SCount].Name == NULL)
		{
			Free_Program_Memory(uni);
			Error_Msg("Failed to allocate memory. Exiting..");
		}

		strcpy((*uni).Students[uni->SCount].Name, NameBuffer);
		fscanf(in, "%ld%f%s", &uni->Students[uni->SCount].ID, &uni->Students[uni->SCount].MtmGrade, uni->Students[uni->SCount].LGrades);
		uni->SCount++;
	}
	puts("\033[1;32mDone\033[0m");
}

/* The function outputs Students Data from a given University to a given output file. */
void Menu_Option_B(University* uni, FILE* out)
{
	int i;

	fprintf(out, "Option B:\n");
	for (i = 0; i < uni->SCount; i++)
		fprintf(out, "Student %d: %s %ld %.2f %d\n", i + 1, uni->Students[i].Name, uni->Students[i].ID, uni->Students[i].MtmGrade, Student_HasPassedLabs(uni->Students[i]) ? 1 : 0);
	puts("\033[1;32mDone\033[0m");
}

/* The function outputs Student Data with final grades from a given University to a given output file. */
void Menu_Option_C(University* uni, FILE* out)
{
	int i;

	fprintf(out, "Option C:\n");
	fprintf(out, "BEFORE:\n");
	for (i = 0; i < uni->SCount; i++)
		fprintf(
			out,
			"Student %d: %s %ld %.2f %d\n", i + 1,
			uni->Students[i].Name, uni->Students[i].ID, uni->Students[i].MtmGrade,
			Student_HasPassedLabs(uni->Students[i]) ? 1 : 0
		);
	fprintf(out, "AFTER:\n");
	for (i = 0; i < uni->SCount; i++)
		fprintf(
			out, 
			"Student %d: %s %ld %.2f %d final: %.2f\n", i + 1,
			uni->Students[i].Name, uni->Students[i].ID, uni->Students[i].MtmGrade,
			Student_HasPassedLabs(uni->Students[i]) ? 1 : 0, Student_CalculateFinalGrade(uni->Students[i])
		);
	puts("\033[1;32mDone\033[0m");
}

/* The function outputs Statistical Data of a given University to a given output file. */
void Menu_Option_D(University* uni, FILE* out)
{
	int i;
	double Average = 0, STDDeviation = 0, Max = uni->Students[0].MtmGrade, Min = uni->Students[0].MtmGrade;

	fprintf(out, "Option D:\n");

	for (i = 0; i < uni->SCount; i++)
		Average += uni->Students[i].MtmGrade;

	Average /= uni->SCount;
	fprintf(out, "1. Average: %.2f\n", Average);

	for (i = 0; i < uni->SCount; i++)
		STDDeviation += pow(uni->Students[i].MtmGrade - Average, 2);

	STDDeviation = sqrt(STDDeviation / uni->SCount);
	fprintf(out, "2. Standard Deviation: %.2f\n3. Students Count: %d\n", STDDeviation, uni->SCount);

	for (i = 0; i < uni->SCount; i++)
	{
		if (uni->Students[i].MtmGrade > Max)
			Max = uni->Students[i].MtmGrade;
		if (uni->Students[i].MtmGrade < Min)
			Min = uni->Students[i].MtmGrade;
	}

	fprintf(out, "4. Range: %.2f-%.2f\n", Max, Min);

	puts("\033[1;32mDone\033[0m");
}

/* The function outputs a Student Blacklist (Didn't pass the course) of a given University to a given output file. */
void Menu_Option_E(University* uni, FILE* out)
{
	int i;

	fprintf(out, "Option E:\n");

	for (i = 0; i < uni->SCount; i++)
		if (!Student_HasPassedLabs(uni->Students[i]))
			fprintf(out, "%s %ld\n", uni->Students[i].Name, uni->Students[i].ID);

	puts("\033[1;32mDone\033[0m");
}

/* The function terminates the Menu and outputs an ending message to a given output file.*/
void Menu_Option_F(FILE* out)
{
	printf("\033[1;32mThank you for using the University Information System. See you next time!\033[0m");
	fprintf(out, "Option F:\nEnd Of Program");
}

/* The function returns true when a Student passed at least 3 labs; false otherwise */
BOOL Student_HasPassedLabs(Student student)
{
	int passedLabs = 0, i;

	for (i = 0; i < 5; i++)
		if (student.LGrades[i] - '0')
			passedLabs++;
	return (passedLabs >= 3) ? true : false;
}

/* The function calculates a Student's Final Grade and/or if he meets minimum passing requirement(s):
	- At least 55 in the final exam.
	- Labs comprise 15% of the final grade.
	- Lab grade 1 means 100; 0 means 0. 
*/
double Student_CalculateFinalGrade(Student student)
{
	return student.MtmGrade < 55 ? student.MtmGrade : (Student_HasPassedLabs(student) ? 15 : 0) + (student.MtmGrade * 0.85);
}

/* The function frees all dynamically allocated memory of a given University structure. */
void Free_Program_Memory(University* uni)
{
	int i;

	for (i = 0; i < uni->SCount; i++)
		free(uni->Students[i].Name);
	free(uni->Students);
}

/* Prints a specified message and exits the program afterwards. */
void Error_Msg(char* s)
{
	puts(s);
	exit(1);
}
