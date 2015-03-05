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
%token ID id num DOT IFTHEN
%token OPA CPA COMMA
%token OBR CBR PIPE
%token GT GEQ LT LEQ U NU EQ NEQ

%union{
	int ival; 
	float fval;
	char* sval;
	queue *qval;
}

%type<sval> ID
%type<sval> id
%type<sval> num
%type<sval> expression
%type<qval> expressionList
%type<sval> op

%type<sval> rop







%%    // grammar rules

programm
    : programm clause			
	|						{
									clean_up();
							}
    ;
	
clause
	: h_beginOfClause head IFTHEN body 	{
											end_of_clause();
										}
	| h_beginOfClause head DOT			{
											end_of_clause();
										}
	;
h_beginOfClause 
	: 	{
			begin_of_clause();
		}
	;
	
head 
	: literal
	;
body 
	: literal DOT
	| literal COMMA body
	| r_literal DOT
	| r_literal COMMA body
	;

literal
	: h_beginOfLiteral id OPA expressionList CPA 
								{ 
									add_predicate($2);
									add_literal($2,$4);
									end_of_literal();
								}
	;	
h_beginOfLiteral 
	:				{
						begin_of_literal();
					}
	;
r_literal
	: h_beginOfLiteral op rop op     {
										add_predicate($3);
										queue *q = queue_new();
										queue_enqueue(q, $2);
										queue_enqueue(q, $4);
										add_literal($3, q);
										end_of_literal();
									}
									
rop 
	: GT 	{  $$ = strdup(">");	}
	| GEQ 	{  $$ = strdup(">=");	}
	| LT 	{  $$ = strdup("<");	}
	| LEQ 	{  $$ = strdup("<=");	}
	| U 	{  $$ = strdup("=");	}
	| NU 	{  $$ = strdup("\\=");	}
	| EQ 	{  $$ = strdup("==");	}
	| NEQ 	{  $$ = strdup("\\=");	};
									
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
	
op 	
	: id {
			$$=$1;
		 }
	| ID {
			$$=$1;
			add_variable($1);
		 }
	| num	{
		$$=$1;
			}
	;
	
expression 
	: op   
			{
				$$=$1;
			}
	
	| OBR expression PIPE expression CBR 	{
												$$ = (char *) malloc(1000);
												sprintf($$, "[%s|%s]", $2, $4);
											}
	| OBR expression CBR 					{
												$$ = (char *) malloc(1000);
												sprintf($$, "[%s]", $2);
											}
	;

	
%%