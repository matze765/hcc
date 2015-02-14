#ifndef COMPILER_H_
#define COMPILER_H_

#include <stdio.h>
#include "queue.h"
#include "node.h"
#include "variableQueueLogic.h"

#define DEPENDENCY_DEPENDENT 		1
#define DEPENDENCY_G_INDEPENDENT 	2
#define DEPENDENCY_G_I_INDEPENDENT 	3
#define DEPENDENCY_I_INDEPENDENT 	4
#define DEPENDENCY_INDEPENDENT 		5

//declare function to get rid of warning
int yylex(); 
int yyparse(); 
void yyerror(char * str);

typedef struct dependency {
	int type;
	queue *gDependency;
	queue *iDependency; 
} dependency;




void init_queues();

void clean_up();
void add_predicate(char *pName);
void add_literal(char *pName, queue *expressionList);

void beginOfLiteral();
void add_variable(char *varName);
void endOfLiteral();

void backpatch(node *source, int side, node *target, int port);
void calcNewVariables();
dependency *calcDependency(queue *variableQueue,int i,int j);
void generateCode();


void print_debug();

#endif /*COMPILER_H_*/
