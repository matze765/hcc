#include "hcc.h"



//TODO: Malloc checks


extern int lineNumber;


int ruleNumber=-1;




	
//TODO dynamic file names
int main (void){
  if(cg_init_files() == -1) {
	return -1;  
  } else {
	yyparse();
	cg_close_files();
  } 
  return 0;
}

void init_new_clause(){
	
	if(literalQueue == NULL){
		literalQueue = queue_new();
	} else {
		queue_clear(literalQueue);
	}
	
	if(predicateQueue == NULL){
		predicateQueue = queue_new();
	} else {
		queue_clear(predicateQueue);
	}
	if(variableQueue == NULL) {
		variableQueue = queue_new();
	} else {
		queue_clear(variableQueue);
	}
	ruleNumber++;
}
void end_of_clause(){
	calcNewVariables();
	print_debug();
	cg_generate_code(	variableQueue, newVarQueue,
						predicateQueue, literalQueue);
	
}

//TODO: clean up
void clean_up(){

}

void add_predicate(char *pName) {
	//copy name 
	char *dup =(char *)strdup(pName);
	queue_enqueue(predicateQueue, dup);
}
void add_literal(char *pName, queue *expressionList){
	// TODO: dynamic strlen
	char *str = (char *) malloc(1000);
	sprintf(str, "%s(",pName);
	int i;
	int count = queue_getCount(expressionList);
	for(i=0;i<count;i++){
		if(i>0) sprintf(str,"%s,",str);
		sprintf(str, "%s%s",str,(char*)queue_getItem(expressionList, i));
	}
	sprintf(str, "%s)",str);
	queue_enqueue(literalQueue, str);
	queue_clear(expressionList);
	free(expressionList);
}
void beginOfLiteral(){
	tmpVarQueue = queue_new();
}
void add_variable(char *varName){
	if(!QL_isPartOf(varName, tmpVarQueue)){
		char *dup= (char *) strdup(varName);
		queue_enqueue(tmpVarQueue,dup);
	}
}

void endOfLiteral(){
	queue_enqueue(variableQueue,tmpVarQueue);
	tmpVarQueue = NULL;
}

void calcNewVariables(){
	newVarQueue = queue_new();
	
	int i,j;
	for(i=0; i<queue_getCount(variableQueue);i++){
		queue *innerQueue = (queue *) queue_getItem(variableQueue, i);
		queue *tmpQueue = queue_new();
		queue_enqueue(newVarQueue, tmpQueue);
		for(j=0;j<queue_getCount(innerQueue);j++){
			char *variable = (char *) queue_getItem(innerQueue,j);
			int isNewVariable = 1;  // ==TRUE
			int x,y;
			for(x=0;x<i;x++){
				
				queue *nInnerQueue =(queue *) queue_getItem(variableQueue, x);
				for(y=0;y<queue_getCount(nInnerQueue);y++){
					char *nVariable = (char *) queue_getItem(nInnerQueue, y);
					isNewVariable = isNewVariable &&  strcmp(variable,nVariable);
					if(!isNewVariable)break;
				}
				if(!isNewVariable)break;
			}
			if(isNewVariable)	 queue_enqueue(tmpQueue, variable);
		}
	}
}

void printQueue(queue *q){
	printf("[");
	int i;
	int count = queue_getCount(q);
	for(i=0;i<count;i++){
		if(i>0) printf(",");
		printf("%s",(char*)queue_getItem(q, i));
	}
	printf("]\n");
}

void print_debug(){
	//printing Predicate List
	printf("PREDICATES(%d)=[",ruleNumber);
	int i;
	int count = queue_getCount(predicateQueue);
	for(i=0;i<count;i++){
		if(i>0) printf(",");
		printf("%s",(char*)queue_getItem(predicateQueue, i));
	}
	printf("]\n");
	
	//printing Literal List
	printf("LITERALS(%d)=[",ruleNumber);
	count = queue_getCount(literalQueue);
	for(i=0;i<count;i++){
		if(i>0) printf(",");
		printf("%s",(char*)queue_getItem(literalQueue, i));
	}
	printf("]\n");
	
	//Printing List of Variable Lists
	printf("VARS(%d)=[",ruleNumber);
	count = queue_getCount(variableQueue);
	int j;
	for(i=0;i<count;i++){
		if(i>0) printf(",");
		printf("[");
		queue* q = (queue*) queue_getItem(variableQueue,i);
		int icount = queue_getCount(q);
		for(j=0;j<icount;j++){
			if(j>0) printf(",");
			printf("%s", (char*)queue_getItem(q,j));
		}
		printf("]");
	}
	printf("]\n");
	
	//Printing List of New Variables
	printf("NEWVARS(%d)=[", ruleNumber);
	count = queue_getCount(newVarQueue);
	for(i=0;i<count;i++){
		if(i>0) printf(",");
		printf("[");
		queue* q = (queue*) queue_getItem(newVarQueue,i);
		int icount = queue_getCount(q);
		for(j=0;j<icount;j++){
			if(j>0) printf(",");
			printf("%s", (char*)queue_getItem(q,j));
		}
		printf("]");
	}
	printf("]\n");
}


// This function prints any error during the parsing and the affected line in the input file.
void yyerror(char * str){
  fprintf(stderr, "Error in line %d: %s\n", lineNumber, str);
}




