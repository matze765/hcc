#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hcc.h"


//TODO: Malloc checks


extern int lineNumber;

int ruleNumber=-1;
queue *literalQueue;
queue *predicateQueue;
queue *variableQueue;		// used to store queues of variables 
queue *tmpVarQueue; 		// used to store variables of one literal
	
int main (void){
  yyparse(); 
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
	char *dup= (char *) strdup(varName);
	queue_enqueue(tmpVarQueue,dup);
}

void endOfLiteral(){
	queue_enqueue(variableQueue,tmpVarQueue);
	tmpVarQueue = NULL;
}
void generateCode() {
	queue *nodes = queue_new();
	int nodeNr =1 ;
	if(queue_getCount(predicateQueue) == 1 ) { // fact
		node *n = (node *) createNode(nodeNr, 'E', 1,1,0,0, 
									  (char *) queue_getItem(literalQueue, 0));
		queue_enqueue(nodes, n);
		nodeNr++;
		n = (node *) createNode(nodeNr, 'R', 0,0,0,0, NULL);
		queue_enqueue(nodes, n);
	} else {
	
	}
	
	// write code to file
	node *n = (node *) queue_dequeue(nodes);
	while(n != NULL) {
		if(n->type == 'C'){
			
		} else {
		
		}
		n = (node *) queue_dequeue(nodes);
	}
	
	
	free(nodes);
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
}


// This function prints any error during the parsing and the affected line in the input file.
void yyerror(char * str){
  fprintf(stderr, "Error in line %d: %s\n", lineNumber, str);
}




