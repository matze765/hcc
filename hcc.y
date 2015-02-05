%{
//Prologue

#include "compiler.h"
#include <string.h>
int functionDeclarationAlreadyExists;
int functionParameterCounter; 
symtabEntry* calledFunction;

%}
//Bison declarations

%error-verbose

// Define all tokens used in the source code
%start programm
%token identifier
%token IF ELSE RETURN WHILE DO CONST VOID 
%token PLUSPLUS MINUSMINUS 
%token NOTEQUAL  LESSEQUAL GREATEREQUAL GREATER LESS 
%token SHIFTL SHIFTR 
%token ORL ANDL EQUALL NOTL 
%token PLUS MINUS TIMES DIV MOD 
%token OPENBR CLOSEBR COPENBR CCLOSEBR
%token COMMA EQUAL SEMICOLON
%token INT FLOAT
%token NAME COMMENT MCOMMENT 
%token INTT FLOATT

%right COMMA
%right EQUAL
%left ORL 
%left ANDL 
%left EQUALL NOTEQUAL
%left LESSEQUAL LESS GREATEREQUAL GREATER
%left SHIFTL SHIFTR 
%left PLUS MINUS
%left TIMES DIV MOD
%right NOTL
%right PLUSPLUS MINUSMINUS

%union{
	int ival; 
	float fval;
	char* sval;
	symtabEntryType type;
}

%type<sval> identifier 
%type<sval> ID
%type<fval> FLOAT
%type<ival> INT
%type<sval> expression
%type<sval> assignment
%type<ival> parameter_list
%type<type> var_type declaration
%type<type> ret_type
%type<sval> functioncall
%type<sval> functionname

%%    // grammar rules

programm
    : function
    | programm function
    ;
	


	

	
function			: 	FD  CLOSEBR SEMICOLON	{
													setContext(0);
												}
					|  FD PL CLOSEBR SEMICOLON  {
													setContext(0);
					}
					| FD VOID CLOSEBR SEMICOLON {
													setContext(0);
					}
					| FD CLOSEBR function_body {
													setContext(0);
					}
					| FD PL CLOSEBR function_body {
													setContext(0);
					}
					| FD VOID CLOSEBR function_body {
													setContext(0);
					}
FD : 	ret_type ID OPENBR {		
	//printf(">>IN RULE 'FD' ret_type=%d, id=%s\n", $1,$2);
	functionParameterCounter = 1;	
	symtabEntry *lookup = symTabLookup($2);
	//printf("##lookup %d", lookup);
	if(lookup != 0 && lookup->vater == 0)	{
		functionDeclarationAlreadyExists = 1;
		setContext(lookup);
		if(lookup->internalType != $1)
			yyerror("Wrong ret type");
	} else {
		functionDeclarationAlreadyExists = 0;
		symtabEntry* neu= addSymboltableEntry($2, FUNC, $1,0,getLineNumber(), 0,0);
		setContext(neu);				
	}
		//printf(">>OUT RULE 'FD'");	
	}
PL : pdeclaration 
   | PL COMMA pdeclaration;

pdeclaration : var_type ID {
	//printf(">>>IN RULE 'pdeclaration' with var_type=%d and ID=%s\n", $1,$2);
	if(functionDeclarationAlreadyExists)  {
		//printf(">>>>>FUNCTON ALREADY EXISTS IN SYMBOLTABLE\n");
		symtabEntry*  par = getParameter(functionParameterCounter, getContext());
		if(par == 0 || par->type != $1 || strcmp(par->name, $2)!=0) 
			yyerror("Parameter error");
	} else {
		//printf(">>>>>FUNCTON DOES NOT EXIST IN SYMBOLTABLE\n");
		//printf(">>>>>ADDING NEW ENTRY \n");
		symtabEntry* neu = addSymboltableEntry($2, $1, NOP, 0, getLineNumber(), getContext(), functionParameterCounter);
		//printf("<<<<<<ADDING NEW ENTRY \n");
	}
	functionParameterCounter++;
	//printf("<<<OUT RULE 'pdeclaration'\n");
}

	
ret_type
	: VOID     {//printf("<<ret_type=NOP");
				$$=NOP;}
	| var_type {$$=$1}
	;
	
function_body
    : COPENBR statement_list  CCLOSEBR
    | COPENBR declaration_list statement_list CCLOSEBR
    ;

declaration_list
    : declaration SEMICOLON
    | declaration_list declaration SEMICOLON
    ;

declaration
	: var_type ID 		{	
							symtabEntry* lookup = symTabLookup($2);
							if(lookup != 0 && lookup->vater!= 0) 
								yyerror("Redeclaration");
							else {
								addSymboltableEntry($2, $1, NOP, 0, getLineNumber(), getContext(), 0);
							}							
							$$=$1;
							
						}
	| declaration COMMA ID  	{
							symtabEntry* lookup = symTabLookup($3);
							if(lookup != 0 &&lookup->vater!= 0) 
								yyerror("Redeclaration");
							else {
								addSymboltableEntry($3, $1, NOP, 0, getLineNumber(), getContext(), 0);
							}							
							$$=$1;
	}
	| CONST var_type ID	{
		
							symtabEntry* lookup = symTabLookup($3);
							if(lookup != 0&&lookup->vater!= 0) 
								yyerror("Redeclaration");
							else {
								addSymboltableEntry($3, $2, NOP, 0, getLineNumber(), getContext(), 0);
							}							
							$$=$2;
							
						}
		
	;

	


var_type
    : INTT {$$ = INTEGER;}
	| FLOATT {$$ = REAL;}
    ;

statement_list
    : statement
    | statement_list statement
    ;

statement
    : matched_statement
    | unmatched_statement
    ;

matched_statement
    : IF OPENBR assignment CLOSEBR matched_statement ELSE matched_statement	{
		symtabEntry* lookup = symTabLookup($3);
		// Zwischencode Erzeugung
	}
    | assignment SEMICOLON {}
    | RETURN SEMICOLON {}
    | RETURN assignment SEMICOLON {}
    | WHILE OPENBR assignment CLOSEBR matched_statement {
		symtabEntry* lookup = symTabLookup($3);
		// Zwischencode Erzeugung
	}	
    | DO statement WHILE OPENBR assignment CLOSEBR SEMICOLON  {
		//printf("reduced do-while");
		symtabEntry* lookup = symTabLookup($5);
		// Zwischencide Erzeugung
	}
    | COPENBR statement_list CCLOSEBR 
    | COPENBR CCLOSEBR
    ;

unmatched_statement
    : IF OPENBR assignment CLOSEBR statement {
		symtabEntry* lookup = symTabLookup($3);
		// Zwischencode Erzeugung
	}
    | WHILE OPENBR assignment CLOSEBR unmatched_statement {
		symtabEntry* lookup = symTabLookup($3);
		// Zwischencode Erzeugung
	}
    | IF OPENBR assignment CLOSEBR matched_statement ELSE unmatched_statement {
		symtabEntry* lookup = symTabLookup($3);
		// Zwischencode Erzeugung
	}
    ;


assignment
    : expression {
		$$ = $1;
	}
    | ID EQUAL expression {
		$$ = $1;
		symtabEntry* id = symTabLookup($1);
		symtabEntry* exp = symTabLookup($3);
		if(id == 0 || exp == 0) {
			yyerror("Error");
		} else  {
			if(id->type != exp->type) {
				yyerror("Wrong Assignment");
			}
		}
	}
    ;

expression
    : PLUSPLUS expression {	
				symtabEntry* lookup = symTabLookup($2);
				if(lookup == 0)
					yyerror("ERROR");
				else {
					char* h=getNextHV();
					addSymboltableEntry(h, lookup->type, NOP, 0,getLineNumber(),getContext(),0);
					$$ = h;
				}
				}			
	| MINUSMINUS expression {
					symtabEntry* lookup = symTabLookup($2);
					if(lookup == 0 )
						yyerror("ERROR");
					else {
						char* h=getNextHV();
						addSymboltableEntry(h, lookup->type, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					}
				}
	| OPENBR expression CLOSEBR {$$ = $2;} 
	| NOTL expression {
		char* h = getNextHV();
		addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
		$$ = h;
		}
	| expression PLUS expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0){
						char* h = getNextHV();
						if(lookup1->type == REAL || lookup2->type == REAL) {
							addSymboltableEntry(h, REAL, NOP, 0,getLineNumber(),getContext(),0);
						} else {
							addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						}
						$$ = h;
					}
		}
	| expression MINUS expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0){
						char* h = getNextHV();
						if(lookup1->type == REAL || lookup2->type == REAL) {
							addSymboltableEntry(h, REAL, NOP, 0,getLineNumber(),getContext(),0);
						} else {
							addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						}
						$$ = h;
					}
	}
	| expression TIMES expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0){
						char* h = getNextHV();
						if(lookup1->type == REAL || lookup2->type == REAL) {
							addSymboltableEntry(h, REAL, NOP, 0,getLineNumber(),getContext(),0);
						} else {
							addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						}
						$$ = h;
					}
	}
	| expression DIV expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0){
						char* h = getNextHV();
						if(lookup1->type == REAL || lookup2->type == REAL) {
							addSymboltableEntry(h, REAL, NOP, 0,getLineNumber(),getContext(),0);
						} else {
							addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						}
						$$ = h;
					}
		}
	| expression MOD expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0){
						if(lookup2->type != INTEGER) {
							yyerror("WRONG USE OF MOD");
						} else {
							char* h = getNextHV();
							if(lookup1->type == REAL) {
								addSymboltableEntry(h, REAL, NOP, 0,getLineNumber(),getContext(),0);
							} else {
								addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
							}
							$$ = h;
						}
					}
	}
	| expression SHIFTL expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0){
						if(lookup2->type != INTEGER) {
							yyerror("WRONG USE OF SHIFTL");
						} else {
							char* h = getNextHV();
							if(lookup1->type == REAL) {
								addSymboltableEntry(h, REAL, NOP, 0,getLineNumber(),getContext(),0);
							} else {
								addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
							}
							$$ = h;
						}
					}
	
	}
	| expression SHIFTR expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0){
						if(lookup2->type != INTEGER) {
							yyerror("WRONG USE OF SHIFTL");
						} else {
							char* h = getNextHV();
							if(lookup1->type == REAL) {
								addSymboltableEntry(h, REAL, NOP, 0,getLineNumber(),getContext(),0);
							} else {
								addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
							}
							$$ = h;
						}
					}
	}
	| expression NOTEQUAL expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0) {
						char* h = getNextHV();
						addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					} 
	}
	| expression LESSEQUAL expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0) {
						char* h = getNextHV();
						addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					} 
	}
	| expression GREATEREQUAL expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0) {
						char* h = getNextHV();
						addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					} 
	}
	| expression GREATER expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0) {
						char* h = getNextHV();
						addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					} 
	}
	| expression LESS expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0) {
						char* h = getNextHV();
						addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					} 
	}
	| expression EQUALL expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0) {
						char* h = getNextHV();
						addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					} 
	}
	| expression ORL expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0) {
						char* h = getNextHV();
						addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					} 
	}
	| expression ANDL expression {
					symtabEntry* lookup1 = symTabLookup($1);
					symtabEntry* lookup2 = symTabLookup($3);
					if(lookup1 != 0 && lookup2 != 0) {
						char* h = getNextHV();
						addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
						$$ = h;
					} 
	}
	| functioncall { strcpy($$,$1);}
	| ID {	strcpy($$,$1);} 
	| INT {	char* h =getNextHV(); 
			addSymboltableEntry(h, INTEGER, NOP, 0,getLineNumber(),getContext(),0);
			$$ = h;}
	| FLOAT { char* h=getNextHV();
			  addSymboltableEntry(h, REAL, NOP, 0,getLineNumber(),getContext(),0);
			  $$ = h;}
    ;

functioncall 
	: functionname OPENBR parameter_list CLOSEBR {
		int count = getParameterCount(calledFunction);
		if(count != $3) {
			yyerror("Wrong Parameter Count");
		} else {
			char* h =getNextHV();
			addSymboltableEntry(h, calledFunction->internalType, NOP, 0,getLineNumber(),getContext(),0);
			$$=h;
		}
		calledFunction = 0;
	}
	| functionname OPENBR CLOSEBR  {
		int count = getParameterCount(calledFunction);
		if(count != 0) {
			yyerror("Wrong Parameter Count");
		} else {
			char* h =getNextHV();
			addSymboltableEntry(h, calledFunction->internalType, NOP, 0,getLineNumber(),getContext(),0);
			$$=h;
		}
		calledFunction = 0;
	}
	;
	
functionname : ID {
		symtabEntry* lookup1 = symTabLookup($1);
		if(lookup1 == 0||lookup1->type != FUNC){
			yyerror("Function called but not defined");
		} else {
			calledFunction = lookup1;
		}
		$$=$1;
}
parameter_list
    : expression	{
		$$ = 1;
		symtabEntry* exp = symTabLookup($1);
		symtabEntry* par = getParameter($$, calledFunction);
		
		//printf("expName=%s parCtr=%d calledFunction=%d par=%d exp=%d\n",$1,$$,calledFunction,par,exp); 
		if(exp == 0 || par == 0 ) {
			yyerror("Internal Error");
		} else if(exp->type != par->type) {
			yyerror("Wrong type of Parameter in functioncall.");
		}
	}
    | parameter_list COMMA expression{
		$$ = $1+1;
		symtabEntry* exp = symTabLookup($3);
		symtabEntry* par = getParameter($$, calledFunction);
		if(exp == 0 || par == 0 ) {
			yyerror("Internal Error");
		} else if(exp->type != par->type) {
			yyerror("Wrong type of Parameter in functioncall");
		}
	}
    ;
	
ID : identifier {	//printf("###found id %s\n", $1);
					$$ = strdup($1);
					};



%%




//Epilogue