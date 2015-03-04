#ifndef SYMTAB_H_
#define SYMTAB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ST_SYMTAB_FILE "out.tab"

#include "queue.h"

typedef struct symbolTable {
	queue *symTabEntries; // queue<symTabEntry>
} symbolTable;

typedef struct symTabEntry {
	char *predicateName;
	int   adicity;	
	queue *entryNodes; // queue<int*>
} symTabEntry;


FILE *symTabFile;

int st_init_symTab();
void st_print_symTab();
void st_close_files();
void st_add_predicate(char *pName, int adicity, int entryLine);



#endif /*SYMTAB_H_*/
