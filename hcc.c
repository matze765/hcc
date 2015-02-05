#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hcc.h"

extern int lineNumber;
	
	
int main (void)
{
  yyparse(); 
  return 0;
}

// This function prints any error during the parsing and the affected line in the input file.
void yyerror(char * str)
{
  fprintf(stderr, "Error in line %d: %s\n", lineNumber, str);
}




