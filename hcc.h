#ifndef HCC_H_
#define HCC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "node.h"
#include "variableQueueLogic.h"
#include "codeGeneration.h"


queue *literalQueue;
queue *predicateQueue;
queue *variableQueue;		// used to store queues of variables 
queue *tmpVarQueue; 		// used to store variables of one literal
queue *newVarQueue;			// used to store which vars are new in each literal 


//declare function to get rid of warning
int yylex(); 
int yyparse(); 
void yyerror(char * str);


void begin_of_clause();
void end_of_clause();

void begin_of_literal();
void end_of_literal();



void add_predicate(char *pName);
void add_literal(char *pName, queue *expressionList);
void add_variable(char *varName);

void calc_new_variables();

void clean_up();

void print_debug();

#endif /*HCC_H_*/
