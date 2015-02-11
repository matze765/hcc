#ifndef COMPILER_H_
#define COMPILER_H_

#include <stdio.h>
#include "queue.h"

//declare function to get rid of warning
int yylex(); 
int yyparse(); 
void yyerror(char * str);

void init_queues();
void add_predicate(char *pName);
void add_literal(char *pName, queue *expressionList);

void print_debug();

#endif /*COMPILER_H_*/
