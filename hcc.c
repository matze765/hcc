#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hcc.h"

#define LEFT 1
#define RIGHT 2

//TODO: Malloc checks


extern int lineNumber;

int ruleNumber=-1;
queue *literalQueue;
queue *predicateQueue;
queue *variableQueue;		// used to store queues of variables 
queue *tmpVarQueue; 		// used to store variables of one literal


FILE *codeOutputFile;
FILE *symTabFile;
	
//TODO dynamic file names
int main (void){
  codeOutputFile = fopen("out", "w+");
  symTabFile	 = fopen("out.tab", "w+");
  if(codeOutputFile == NULL || symTabFile == NULL ) {
	return -1;  
  } else {
	yyparse();
	fclose(codeOutputFile);
	fclose(symTabFile);
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
void backpatch(node *source, int side, node *target, int port) {
	if(source->type == 'C'){
		copyNode_add((copyNode *)source, target->nr, port);
	} else {
		if(side == LEFT) {
			source->leftNode = target->nr;
			source->leftNodePort = port;
		} else if( side == RIGHT){
			source->rightNode = target->nr; 
			source->rightNodePort = port; 
		}
	}
	
}

void generateCode() {
	queue *nodes = queue_new();
	int nodeNr =1 ;
	if(queue_getCount(predicateQueue) == 1 ) { // fact
		node *n = (node *) createNode(nodeNr, 'E', 2,1,0,0, 
									  (char *) queue_getItem(literalQueue, 0));
									  

		queue_enqueue(nodes, n);
		nodeNr++;
		n = (node *) createNode(nodeNr, 'R', 0,0,0,0, NULL);
	
		queue_enqueue(nodes, n);
	} else {
		// create entry unification node
		node *entryNode = (node *) createNode(nodeNr, 'E',0,0,0,0, (char *) queue_getItem(literalQueue,0));
		node *lastUnification = entryNode;
		nodeNr++;
		queue_enqueue(nodes, entryNode);
		// create copy node to distribute entry token 
		node *entryCopyNode = (node *) createNode(nodeNr, 'C', 0,0,0,0, NULL);
		nodeNr++;
		queue_enqueue(nodes, entryCopyNode);
		backpatch(entryNode, RIGHT, entryCopyNode, LEFT);
		
		// queue to store the sub goal copy nodes
		queue *copyNodes = queue_new();
		queue_enqueue(copyNodes, entryCopyNode);
		int i;
		for(i=1; i<queue_getCount(literalQueue);i++){
			// create entry U node
			node *entryUNode = (node *) createNode(nodeNr, 'U',0,0,0,0, (char *) queue_getItem(literalQueue,i));
			nodeNr++;
			queue_enqueue(nodes, entryUNode);
			
			node *lastUNode=entryUNode;
			node *lastCheckNode = NULL;
			
			// createApplynode
			node *aNode = (node *) createNode(nodeNr, 'A',0,0,0,0, NULL);
			nodeNr++;
			backpatch(lastUNode, LEFT, aNode, LEFT);
			queue_enqueue(nodes, aNode);
			
			node  *copyNode = (node *) createNode(nodeNr, 'C',0,0,0,0, NULL);
			nodeNr++;
			backpatch(aNode, LEFT, copyNode, LEFT);
			queue_enqueue(nodes, copyNode);
			
			
			// create exit U node
			node *exitUNode = (node *) createNode(nodeNr, 'U',0,0,0,0, NULL);
			nodeNr++;
			backpatch(copyNode,LEFT, exitUNode, LEFT);
			backpatch(lastUnification, LEFT, exitUNode, RIGHT);
			lastUnification = exitUNode;
			queue_enqueue(nodes, exitUNode);
			
			
			
			
		}
			
		
	
		
		
		// create return node
		node *returnNode = (node *) createNode(nodeNr, 'R', 0,0,0,0, NULL);
		backpatch(lastUnification, LEFT, returnNode, 1);
		queue_enqueue(nodes, returnNode);
	}
	
	
	
	node *n = (node *) queue_dequeue(nodes);
	
	while(n != NULL) {
		if(n->type == 'C'){
			copyNode *cp = (copyNode *)n;
			fprintf(codeOutputFile, "%d\t%c", cp->nr, cp->type);
			int *target = (int *) queue_dequeue(cp->nextNodes);
			while(target != NULL ){
				fprintf(codeOutputFile, "\t(%d,%d)", target[0], target[1]);
				target = (int *) queue_dequeue(cp->nextNodes);
			}
		} else {
			fprintf(codeOutputFile, "%d\t%c\t", n->nr, n->type);
			
			if(n->leftNode != 0){
				fprintf(codeOutputFile, "(%d,%d)\t",n->leftNode, n->leftNodePort);
			} else {
				fprintf(codeOutputFile, "-\t");
			}
			if(n->rightNode != 0){
				fprintf(codeOutputFile, "(%d,%d)\t",n->rightNode, n->rightNodePort);
			} else {
				fprintf(codeOutputFile, "-\t");
			}
			
			if(n->str != NULL){
				 fprintf(codeOutputFile, "%s", n->str);
			}else {
				fprintf(codeOutputFile, "-");
			}
		}
		fprintf(codeOutputFile, "\n");
		n = (node *) queue_dequeue(nodes);
	}
	fprintf(codeOutputFile, "\n");
	
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




