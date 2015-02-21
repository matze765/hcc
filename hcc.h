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

void init_queues();

void clean_up();
void add_predicate(char *pName);
void add_literal(char *pName, queue *expressionList);

void beginOfLiteral();
void add_variable(char *varName);
void endOfLiteral();

void end_of_clause();




void calcNewVariables();



void print_debug();

#endif /*HCC_H_*/
