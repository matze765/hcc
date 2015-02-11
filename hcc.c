#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hcc.h"


//TODO: Malloc checks


extern int lineNumber;

int ruleNumber=-1;
queue *literalQueue;
queue *predicateQueue;
	
int main (void){
  yyparse(); 
  return 0;
}

void init_new_clause(){
	/*
	if(literalQueue == NULL){
		literalQueue = queue_new();
	} else {
		queue_clear(literalQueue);
	}*/
	
	if(predicateQueue == NULL){
		predicateQueue = queue_new();
	} else {
		queue_clear(predicateQueue);
	}
	ruleNumber++;
	
}

void add_predicate(char *pName) {
	//copy literal 
	char *dup =(char *)strdup(pName);
	queue_enqueue(predicateQueue, dup);
}

void print_predicates(){
	printf("RULE(%d)=[",ruleNumber);
	int i;
	int count = queue_getCount(predicateQueue);
	for(i=0;i<count;i++){
		if(i>0) printf(",");
		printf("%s",(char*)queue_getItem(predicateQueue, i));
	}
	printf("]\n");
}


// This function prints any error during the parsing and the affected line in the input file.
void yyerror(char * str){
  fprintf(stderr, "Error in line %d: %s\n", lineNumber, str);
}




