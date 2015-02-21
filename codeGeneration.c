#include "codeGeneration.h"


int cg_init_files(){
	codeOutputFile = fopen(CG_CODE_OUTPUT_FILE, "w+");
	symTabFile	 = fopen(CG_SYMTAB_FILE, "w+");
	if(codeOutputFile == NULL || symTabFile == NULL) {
		cg_close_files();
		return -1;
	} else {
		return 0;	
	}
}

void cg_close_files(){
	if(codeOutputFile != NULL){
		fclose(codeOutputFile);
	}
	if(symTabFile != NULL){
		fclose(symTabFile);
	}
	codeOutputFile = NULL;
	symTabFile = NULL;
}