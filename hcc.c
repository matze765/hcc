#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

extern int lineNumber;

//pointer as a entry to the Symboltable, which is a linked list
symtabEntry * theSymboltable = 0;
int counter=0;
symtabEntry* context=0;

char* getNextHV() {
	char *a = malloc(sizeof(char)*10); 
	sprintf(a, "h%d",counter);
	counter++;
	return a;
}
int getLineNumber() {
	return lineNumber;
}
void setContext(symtabEntry* sE){
	context = sE;
}
symtabEntry* getContext() {
	return context;
}

symtabEntry* symTabLookup(char* name) {
	//printf(">>IN symTabLookup name=%s\n", name);
	// check in current context
	symtabEntry* cur = theSymboltable;
	symtabEntry* globalMatch = 0;
	//printf("#preloop cur=%d\n", cur);
	while(cur != 0) {
		//printf("#inloop cur=%d cur.name=%s cur.next=%d\n", cur, cur->name, cur->next); 
		if((strcmp(cur->name, name)==0)){
			if(cur->vater == getContext()) {
				return cur;
			} else if(cur->vater==0) {
				globalMatch = cur;
			}
		}
		cur = cur->next;			
	}
	//printf("<<OUT symTabLookup\n");
	return globalMatch;
	
}
symtabEntry* getParameter(int i, symtabEntry* vater) {
	if(i>getParameterCount(vater)) return 0;
	symtabEntry* cur = theSymboltable;
	while(cur != 0) {
		if(cur->vater == vater && cur->parameter == i)
			return cur;
		cur = cur->next;
	}
	return 0;
}
int getParameterCount(symtabEntry* vater) {
	symtabEntry* cur = theSymboltable; 
	int ctr = 0;
	while(cur != 0) {
		if(cur->vater == vater && cur->parameter !=0) {
			ctr++;
		} 
		cur = cur->next;
	}
	return ctr;
}
	
	
int main (void)
{
  yyparse();
  
  
  //sample for a valid symboltable
  /*
  addSymboltableEntry("If_Demo"  , PROG,     NOP, 18, 0, 0, 0);
  addSymboltableEntry("wert"     , INTEGER,  NOP, NOP,  0, 0, 0 );
  addSymboltableEntry("d"        , INTEGER,  NOP, NOP,  4, 0, 0 );
  addSymboltableEntry("H0"       , BOOL,     NOP, NOP,  8, 0, 0 );
  addSymboltableEntry("H1"       , BOOL,     NOP, NOP,  9, 0, 0 );
  addSymboltableEntry("H2"       , BOOL,     NOP, NOP, 10, 0, 0 );
  addSymboltableEntry("H3"       , BOOL,     NOP, NOP, 11, 0, 0 );
  addSymboltableEntry("H4"       , BOOL,     NOP, NOP, 12, 0, 0 );
  addSymboltableEntry("H5"       , BOOL,     NOP, NOP, 13, 0, 0 );
  addSymboltableEntry("H6"       , BOOL,     NOP, NOP, 14, 0, 0 );
  addSymboltableEntry("H7"       , BOOL,     NOP, NOP, 15, 0, 0 );
  addSymboltableEntry("H8"       , BOOL,     NOP, NOP, 16, 0, 0 );
  addSymboltableEntry("H9"       , BOOL,     NOP, NOP, 17, 0, 0 );
  */
  // Write the symbol table to file
  FILE * outputFile;
  if ((outputFile = fopen("./Symboltable.out", "w")) != 0)
  {
    if (theSymboltable != 0)
    {
      writeSymboltable(outputFile);
    }
  }
  fclose(outputFile);
  
  return 0;
}

// This function prints any error during the parsing and the affected line in the input file.
void yyerror(char * str)
{
  fprintf(stderr, "Error in line %d: %s\n", lineNumber, str);
}

// This function adds a new entry to the symbol table
symtabEntry * addSymboltableEntry (char * name,
              symtabEntryType type,
              symtabEntryType internalType,
              int offset,
              int line,
              symtabEntry * vater,
              int parameter)
{
  //printf(">>IN FUNCTION 'addSymboltableEntry' name=%s type=%d inttype=%d offset=%d line=%d vater=%d par=%d \n", name, type, internalType, offset, line, vater, parameter);
  // If the global variable theSymboltable is not 
  // initialized, this will be done with the first call of this function


  symtabEntry* newSymtabEntry = (symtabEntry*) malloc(sizeof(symtabEntry));
  //printf("##malloc\n");
  // allocates the memory for the new symtabEntry
  newSymtabEntry->name = (char *) malloc(strlen(name) + 1);
  
  //printf("##init\n");
  strcpy(newSymtabEntry->name, name);
  newSymtabEntry->type = type;
  newSymtabEntry->internalType = internalType;
  newSymtabEntry->offset = offset;
  newSymtabEntry->line = line;
  newSymtabEntry->vater = vater;
  newSymtabEntry->parameter = parameter;
  newSymtabEntry->next = 0;
  
  //printf("##add to table\n");
  if (!theSymboltable)
  {
    //printf("##newtable");
	// there is no entry in the Symboltable
    theSymboltable = newSymtabEntry;
  }
  else
  {
    // there is at least one entry in the Symboltable
    symtabEntry* symtabHelp = theSymboltable;
    
    while (symtabHelp->next)
    {
      // walks to the last entry of Symboltable
      symtabHelp = symtabHelp->next;
    }
    symtabHelp->next = newSymtabEntry;
  }
  //printf("<<OUT FUNCTION 'addSymboltableEntry'\n");
  return newSymtabEntry;
}

//puts the printout for a given SymbolEntrytype to the given string
void getSymbolTypePrintout(symtabEntryType  type, char * writeIn)
{
  switch(type)
  {
    case PROG:     strcpy(writeIn,"Prg     ")  ;break;
    case NOP :     strcpy(writeIn,"None    ")  ;break;
    case REAL:     strcpy(writeIn,"Real    ")  ;break;
    case BOOL:     strcpy(writeIn,"Bool    ")  ;break;
    case INTEGER : strcpy(writeIn,"Int     ")  ;break;
    case ARR :     strcpy(writeIn,"Array   ")  ;break;
    case FUNC:     strcpy(writeIn,"Func    ")  ;break;
    case PROC:     strcpy(writeIn,"Proc    ")  ;break;
    case PRG_PARA: strcpy(writeIn,"P.Prmtr ")  ;break;
    default:       strcpy(writeIn,"        ")  ;break;
  }
}

// writes the Symboltable in the outFile formated in a table view 
void writeSymboltable (FILE * outputFile)
{
  fprintf (outputFile, "SYMBOLS\n");
  fprintf (outputFile, "-------\n");
  fprintf (outputFile, "\n");
  fprintf (outputFile, "Name                Type    Int_Typ Offset/Size    Line    Index1    Index2    Parent          Parameter\n");
  fprintf (outputFile, "-------------------------------------------------------------------------------------------------------\n");
  
  //variables
  symtabEntry * currentEntry;   //pointer for the current Symboltable entry for walking through the list
  int j;              //help variable, to build a string with the same length
  char helpString[21];      //string for formatted output
  
  currentEntry = theSymboltable;
  do
  {
    //walks through the Symboltable
    strncpy(helpString,currentEntry->name, 20);
    for(j=19;j>=strlen(currentEntry->name);j--)
    {
      //loop for formating the output to file 
      helpString[j]=' ';
    }
    helpString[20]=0;
    fprintf(outputFile, "%s",helpString);
    
    getSymbolTypePrintout(currentEntry->type,helpString);
    fprintf(outputFile, "%s",helpString);
    
    getSymbolTypePrintout(currentEntry->internalType,helpString);
    fprintf(outputFile, "%s",helpString);
    
    fprintf(outputFile, "%11d ",currentEntry->offset);
    fprintf(outputFile, "%7d ",currentEntry->line);
    fprintf(outputFile, "%9d ", 0);
    fprintf(outputFile, "%9d \t", 0);
    if(currentEntry->vater)
    {
      fprintf(outputFile, "%s \t",currentEntry->vater->name);
    }
    else
    {
      fprintf(outputFile, "None \t");
    }
    fprintf(outputFile, "%d \n",currentEntry->parameter);
    
    fflush(outputFile);
    
    currentEntry=currentEntry->next;
  }
  while(currentEntry);
  
  fprintf(outputFile, "\n");
  fflush(outputFile);
  
  // writeCode(outputFile);
}
