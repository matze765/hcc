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
}

%type<sval> ID
%type<sval> id



%%    // grammar rules

programm
    : programm clause
	|
    ;
	
clause
	: h_beginOfClause head IFTHEN body 	{
											print_predicates();
										}
	| h_beginOfClause head DOT			{
											print_predicates();
										}
	;
h_beginOfClause 
	: 	{
			init_new_clause();
		}
	;
	
head 
	: literal
	;
body 
	: literal DOT
	| literal COMMA body
	;

literal
	: id OPA expressionList CPA 
								{ 
									add_predicate($1);
								}
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