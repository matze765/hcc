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
%token PLUS MINUS TIMES DIV
%token IS 
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
%type<sval> function

%type<sval> rop


%left PLUS MINUS
%left TIMES DIV
%left IS




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
	| LEQ 	{  $$ = strdup("=<");	}
	| U 	{  $$ = strdup("=");	}
	| NU 	{  $$ = strdup("\\=");	}
	| EQ 	{  $$ = strdup("==");	}
	| NEQ 	{  $$ = strdup("\\=");	}
	| IS    {  $$ = strdup("is");   }
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
	| op PLUS op 	{
						$$ = (char *) malloc(1000);
						sprintf($$, "+(%s,%s)", $1,$3);
					}
	| op MINUS op 	{
						$$ = (char *) malloc(1000);
						sprintf($$, "-(%s,%s)", $1,$3);
					}
	| op TIMES op 	{
						$$ = (char *) malloc(1000);
						sprintf($$, "*(%s,%s)", $1,$3);
					}
					
	| op DIV op 	{
						$$ = (char *) malloc(1000);
						sprintf($$, "/(%s,%s)", $1,$3);
					}
	| OPA op CPA 	{
						$$ = $2;
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
	| function								{
												$$=$1;
											}
	;
	
function 
	: id OPA expressionList CPA  			{
												
												char *tmp = (char *) malloc(1000);
												strcpy(tmp, "");
												int i;
												for(i=0;i<queue_getCount($3);i++){
													char *itm = (char *)queue_getItem($3, i);
													if(i!=0){
														sprintf(tmp, "%s,", tmp);
													}
													sprintf(tmp, "%s%s", tmp,itm);
												}
												queue_clear($3);
											
												$$ = (char *) malloc(1000);
												sprintf($$, "%s(%s)", $1, tmp);
												free(tmp);
											}

	
%%