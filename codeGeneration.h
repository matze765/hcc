#ifndef CODEGENERATION_H_
#define CODEGENERATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CG_CODE_OUTPUT_FILE "out"
#define CG_SYMTAB_FILE "out.tab"


FILE *codeOutputFile;
FILE *symTabFile;

int  cg_init_files();
void cg_close_files();



#endif /* CODEGENERATION_H_ */