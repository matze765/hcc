#ifndef COMPILER_H_
#define COMPILER_H_

#include <stdio.h>

// List of available data types
typedef enum symtab_EntryType { INTEGER, REAL, BOOL, PROC, NOP, ARR, FUNC, PROG, PRG_PARA }
  symtabEntryType;
 

// Symbol table entry
typedef struct a_symtabEntry {
  char * name;
  symtabEntryType type;
  symtabEntryType internalType;
  int offset;
  int line;
  struct a_symtabEntry * vater;
  int parameter;
  struct a_symtabEntry * next;
} symtabEntry;

// Functions to manage symbol table
symtabEntry * addSymboltableEntry(char * name, symtabEntryType type, symtabEntryType internalType, int offset, int line, symtabEntry * vater, int parameter);
void          getSymbolTypePrintout(symtabEntryType type, char * writeIn);
void          writeSymboltable(FILE * outputFile);
char* 		  getNextHV();
int 	      getLineNumber();
void setContext(symtabEntry* sE);
symtabEntry* getContext();
symtabEntry* symTabLookup(char* name);
symtabEntry* getParameter(int i, symtabEntry* vater);
int getParameterCount(symtabEntry* vater);


#endif /*COMPILER_H_*/
