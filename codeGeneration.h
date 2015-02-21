#ifndef CODEGENERATION_H_
#define CODEGENERATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "variableQueueLogic.h"

#define CG_CODE_OUTPUT_FILE "out"
#define CG_SYMTAB_FILE "out.tab"


#define DEPENDENCY_DEPENDENT 		1
#define DEPENDENCY_G_INDEPENDENT 	2
#define DEPENDENCY_G_I_INDEPENDENT 	3
#define DEPENDENCY_I_INDEPENDENT 	4
#define DEPENDENCY_INDEPENDENT 		5

typedef struct dependency {
	int type;
	queue *gDependency;
	queue *iDependency; 
} dependency;


FILE *codeOutputFile;
FILE *symTabFile;

int  cg_init_files();
void cg_close_files();

dependency *calcDependency(queue *variableQueue, queue *newVarQueue, int i, int j);



#endif /* CODEGENERATION_H_ */