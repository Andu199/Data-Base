/* BOLDIȘOR Dragoș-Alexandru - 312CB */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __STRUCTURES__
#define __STRUCTURES__

#define MAX_COLUMN_NAME_LEN 30
#define MAX_TABLE_NAME_LEN  30
#define MAX_DB_NAME_LEN     30
#define MAX_CMD_LEN         300

// int
typedef struct intCell {
	int    value;
	struct intCell* next;
} t_intCell;

typedef struct intLine {
	t_intCell*      cells;
	struct intLine* next;
} t_intLine;

// float
typedef struct floatCell {
	float  value;
	struct floatCell* next;
} t_floatCell;

typedef struct floatLine {
	t_floatCell*      cells;
	struct floatLine* next;
} t_floatLine;

// string
typedef struct stringCell {
	char*  value;
	struct stringCell* next;
} t_stringCell;

typedef struct stringLine {
	t_stringCell*       cells;
	struct stringLine* next;
} t_stringLine;

// cell type
typedef enum {
	INT,
	FLOAT,
	STRING
} t_cellType;

// column
typedef struct column {
	char name[MAX_COLUMN_NAME_LEN];
	struct column* next;
} t_column;

// table
typedef struct table {
	char name[MAX_TABLE_NAME_LEN];
	t_cellType    type;
	t_column*     columns;
	void*         lines;  // t_intLine* | t_floatLine* | t_stringLine*
	struct table* next;
} t_table;

// database
typedef struct db {
	char name[MAX_DB_NAME_LEN];
	t_table* tables;
} t_db;

t_db *initDB(char *name);
void deleteDB(t_db **d);

void clearIntLines(t_table *t);
void clearFloatLines(t_table *t);
void clearStringLines(t_table *t);

void createNewTable(t_db *d, char *buffer);
void deleteTable(t_db *d, char *name);

int findRelation(char *string);
int findColumn(t_table *t, char *name);
t_table *findTable(t_db *d, char *buffer);
int Digits(int nr);

void printIntLine(t_intCell *cell);
void printFloatLine(t_floatCell *cell);
void printStringLine(t_stringCell *cell);
void printIntTable(t_table *t);
void printFloatTable(t_table *t);
void printStringTable(t_table *t);
void printTable(t_db *d, char *name, int with);
void printDB(t_db *d);

t_intCell *alocIntCell(char *value);
t_floatCell *alocFloatCell(char *value);
t_stringCell *alocStringCell(char *value);
void addIntLine(t_table *t, char *buffer);
void addFloatLine(t_table *t, char *buffer);
void addStringLine(t_table *t, char *buffer);
void addLine(t_db *d, char *buffer);
void clearLines(t_db *d, char *name);

void deleteIntLine(t_intLine *ant, t_table *t);
void deleteFloatLine(t_floatLine *ant, t_table *t);
void deleteStringLine(t_stringLine *ant, t_table *t);
void PoR_intLine(int task, t_table *t, int poz, int rel, char *val);
void PoR_floatLine(int task, t_table *t, int poz, int rel, char *val);
void PoR_stringLine(int task, t_table *t, int poz, int rel, char *val);
void Print_or_Delete_Lines(int task, t_db *d, char *buffer);

#endif