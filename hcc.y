%{
//Prologue

#include "hcc.h"
#include <string.h>

%}
//Bison declarations

%error-verbose

// Define all tokens used in the source code
%start programm
%token ID id DOT IFTHEN
%token OPA CPA COMMA


%union{
	int ival; 
	float fval;
	char* sval;
	symtabEntryType type;
}

%type<sval> ID
%type<sval> id



%%    // grammar rules

programm
    : programm clause
	|
    ;
	
clause
	: head IFTHEN body
	| head DOT
	;
	
head 
	: literal
	;
body 
	: literal DOT
	| literal COMMA body
	;

literal
	: id OPA expressionList CPA { printf("Literal=%s\n", $1);}
	;	

expressionList 
	: expression 
	| expressionList COMMA expression
	;
	
expression 
	: id
	| ID
	;

	
%%