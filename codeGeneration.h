#ifndef CODEGENERATION_H_
#define CODEGENERATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "variableQueueLogic.h"
#include "node.h"
#include "symTab.h"

#define CG_CODE_OUTPUT_FILE "out"



#define DEPENDENCY_DEPENDENT 		1
#define DEPENDENCY_G_INDEPENDENT 	2
#define DEPENDENCY_G_I_INDEPENDENT 	3
#define DEPENDENCY_I_INDEPENDENT 	4
#define DEPENDENCY_INDEPENDENT 		5


#define LEFT_PORT 1
#define RIGHT_PORT 2

typedef struct dependency {
	int type;
	queue *gDependency;
	queue *iDependency; 
} dependency;


FILE *codeOutputFile;


// public API
int  cg_init_files();
void cg_close_files();


void cg_generate_code(queue *variableQueue, queue *newVarQueue, 
			      queue *predicateQueue, queue *literalQueue);
				  




#endif /* CODEGENERATION_H_ */