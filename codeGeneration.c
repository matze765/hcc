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
//TODO: get rid of goto
dependency *calcDependency(queue *variableQueue, queue *newVarQueue, int i, int j) {
	dependency *d = (dependency *) malloc(sizeof(dependency));
	d->gDependency = NULL;
	d->iDependency = NULL;
	if(d==NULL) return NULL;
	
	queue *left 	= queue_getItem(variableQueue, i);
	queue *leftNew 	= queue_getItem(newVarQueue,   i);
	queue *right  	= queue_getItem(variableQueue, j);
	queue *rightNew	= queue_getItem(newVarQueue,   j);
	
	// declare all variables needed later; 
	queue *intersection_LN_R 	= NULL;
	queue *intersection_L_R  	= NULL;
	queue *union_LN_R			= NULL;
	queue *union_L_RN			= NULL;
	queue *A 					= NULL;
	queue *B					= NULL;
	
	//check for DEPENDENCY_DEPENDENT
	intersection_LN_R = QL_intersection(leftNew,right);
	//printQueue(intersection_LN_R);
	if(!QL_isEmpty(intersection_LN_R)) {
		d->type = DEPENDENCY_DEPENDENT;
		goto end;
	} 
	// check for DEPENDENCY_G_INDEPENDENT
	intersection_L_R 	= QL_intersection(left,right);
	union_LN_R 			= QL_union(leftNew, right);
	union_L_RN 			= QL_union(left, rightNew);	 
	A 		  			= QL_without(left, union_LN_R); // L\(LN u R)
	B 		  			= QL_without(right, union_L_RN); // R\ (L u RN) 
	if(!QL_isEmpty(intersection_L_R) 
		&& QL_isEmpty(intersection_LN_R)
		&& (QL_isEmpty(A)||QL_isEmpty(B))){
		d->type = DEPENDENCY_G_INDEPENDENT;
		d->gDependency = QL_copy(intersection_L_R);
		goto end; 
	}
	
	// check for DEPENDENCY_G_I_INDEPENDENT
	if(!QL_isEmpty(intersection_L_R)
		&& QL_isEmpty(intersection_LN_R)
		&& !QL_isEmpty(A)
		&& !QL_isEmpty(B)){
		d->type = DEPENDENCY_G_I_INDEPENDENT;
		d->gDependency = QL_copy(intersection_L_R);
		d->iDependency = queue_new();
		int i,j;
		for(i=0;i<queue_getCount(A);i++){
			for(j=0;j<queue_getCount(B);j++){
				queue *tmp= queue_new();
				char *a = strdup(queue_getItem(A,i));
				char *b = strdup(queue_getItem(B,j));
				queue_enqueue(tmp,a);
				queue_enqueue(tmp,b);
				queue_enqueue(d->iDependency,tmp);
			}
		}
		goto end;
	}
	// check for DEPENDENCY_I_INDEPENDENT
	if(QL_isEmpty(intersection_L_R)
		&& !QL_isEmpty(A)
		&& !QL_isEmpty(B)){
		d->type = DEPENDENCY_I_INDEPENDENT;
		d->iDependency = queue_new();
		int i,j;
		for(i=0;i<queue_getCount(A);i++){
			for(j=0;j<queue_getCount(B);j++){
				queue *tmp= queue_new();
				char *a = strdup(queue_getItem(A,i));
				char *b = strdup(queue_getItem(B,j));
				queue_enqueue(tmp,a);
				queue_enqueue(tmp,b);
				queue_enqueue(d->iDependency,tmp);
			}
		}
		goto end;
	}
	
	// check for DEPENDENCY_INDEPENDENT
	if(QL_isEmpty(intersection_L_R)
		&&  (QL_isEmpty(A)||QL_isEmpty(B))){			
		d->type = DEPENDENCY_INDEPENDENT;
		goto end;
	}
	
	end:
	// clean up
	if(intersection_LN_R != NULL){
		queue_clear(intersection_LN_R);
		free(intersection_LN_R);
	}
	if(intersection_L_R != NULL ){
		queue_clear(intersection_L_R);
		free(intersection_L_R);
	}
	if(union_LN_R != NULL){
		queue_clear(union_LN_R);
		free(union_LN_R);
	}
	if(union_L_RN != NULL){
		queue_clear(union_L_RN);
		free(union_L_RN);
	}
	if(A != NULL){
		queue_clear(A);
		free(A);
	}
	if(B != NULL){
		queue_clear(B);
		free(B);
	}

	return d;
}
