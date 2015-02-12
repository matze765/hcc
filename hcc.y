%{
//Prologue

#include "hcc.h"
#include "queue.h"
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
	queue *qval;
}

%type<sval> ID
%type<sval> id
%type<sval> expression
%type<qval> expressionList





%%    // grammar rules

programm
    : programm clause			{
									clean_up();
								}
	|
    ;
	
clause
	: h_beginOfClause head IFTHEN body 	{
											print_debug();
										}
	| h_beginOfClause head DOT			{
											print_debug();
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
	: h_beginOfLiteral id OPA expressionList CPA 
								{ 
									add_predicate($2);
									add_literal($2,$4);
									endOfLiteral();
								}
	;	
h_beginOfLiteral 
	:				{
						beginOfLiteral();
					}
	;
	
expressionList 
	: expression   							{
												$$ = queue_new();
												char *ex = (char *) strdup($1);
												queue_enqueue($$, ex);
											}
	| expressionList COMMA expression		{
												char *ex = (char *) strdup($3);
												$$=$1;
												queue_enqueue($$, ex);
											}
	;
	
expression 
	: id   
			{
				$$=$1;
			}
	| ID	{
				$$=$1;
				add_variable($1);
			}
	;

	
%%