#include "hcc.h"



//TODO: Malloc checks


extern int lineNumber;

int ruleNumber=-1;
queue *literalQueue;
queue *predicateQueue;
queue *variableQueue;		// used to store queues of variables 
queue *tmpVarQueue; 		// used to store variables of one literal
queue *newVarQueue;			// used to store which vars are new in each literal 



	
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
			backpatch(entryCopyNode, LEFT, entryUNode, LEFT);
			node *lastUNode=entryUNode;
			node *lastCheckNode = NULL;
			
			
			// create check nodes 
			int j;
			for(j=1;j<i;j++){
				dependency *d = calcDependency(variableQueue, newVarQueue, j,i);
				node *dependentCopyNode =queue_getItem(copyNodes,j);
				node *middleUNode;
				node *GNode;
				node *INode;
				char *str;
				int x;
				switch(d->type){
					case DEPENDENCY_DEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_DEPENDENT\n", j,i);
						middleUNode = (node *)createNode(nodeNr,'U', 0,0,0,0,NULL);
						nodeNr++;
						backpatch(dependentCopyNode, LEFT, middleUNode, LEFT);
						backpatch(lastUNode, LEFT, middleUNode, RIGHT);
						lastUNode = middleUNode;
						if(lastCheckNode != NULL){
							backpatch(lastCheckNode, RIGHT, middleUNode, RIGHT);
							lastCheckNode= NULL;
						}
						
						
						queue_enqueue(nodes, middleUNode);
						
					break;
					case DEPENDENCY_G_INDEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_G_INDEPENDENT\n",j,i);
						middleUNode = (node *) createNode(nodeNr, 'U', 0,0,0,0,NULL);
						nodeNr++;
						queue_enqueue(nodes, middleUNode);
						backpatch(dependentCopyNode, LEFT, middleUNode,  LEFT);
						str = malloc(sizeof(char)* 1000);
						str[0] = 0;
						for(x=0;x<queue_getCount(d->gDependency);x++){
							if(x>0)sprintf(str, "%s,",str);
							sprintf(str, "%s%s", str, (char *) queue_getItem(d->gDependency, x));
						}						
						GNode = (node *) createNode(nodeNr, 'G', 0,0,0,0, str);
						nodeNr++;
						queue_enqueue(nodes, GNode);
						backpatch(lastUNode, LEFT, GNode, LEFT);
						backpatch(GNode, LEFT, middleUNode, RIGHT);
						if(lastCheckNode != NULL){
							backpatch(lastCheckNode, RIGHT, GNode, LEFT);
						}	
						lastCheckNode = GNode;
						lastUNode     = middleUNode; 
						
					break;
					case DEPENDENCY_G_I_INDEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_G_I_INDEPENDENT\n",j,i);
						middleUNode = (node *) createNode(nodeNr, 'U', 0,0,0,0, NULL);
						nodeNr++;
						queue_enqueue(nodes, middleUNode);
						backpatch(dependentCopyNode, LEFT, middleUNode, LEFT);
						str = malloc(sizeof(char)* 1000);
						str[0] = 0;
						for(x=0;x<queue_getCount(d->gDependency);x++){
							if(x>0)sprintf(str, "%s,",str);
							sprintf(str, "%s%s", str, (char *) queue_getItem(d->gDependency, x));
						}						
						GNode = (node *) createNode(nodeNr, 'G', 0,0,0,0, str);
						nodeNr++;
						queue_enqueue(nodes, GNode);
						backpatch(lastUNode, LEFT, GNode,LEFT);
						backpatch(GNode, LEFT, middleUNode, RIGHT);
						if(lastCheckNode != NULL) {
							backpatch(lastCheckNode, RIGHT, GNode, LEFT);
						}
						str = malloc(sizeof(char)* 1000);
						sprintf(str, "[");
						for(x=0;x<queue_getCount(d->iDependency);x++){
							if(x>0)sprintf(str, "%s,",str);
							queue *tmpQueue = (queue*) queue_getItem(d->iDependency, x);

							sprintf(str, "%s[%s,%s]", str, (char *) queue_getItem(tmpQueue, 0), 
															(char *) queue_getItem(tmpQueue, 1));
						}
						sprintf(str,"%s]",str);
						INode  = (node *) createNode(nodeNr,'I', 0,0,0,0,str);
						nodeNr++;
						queue_enqueue(nodes, INode);
						backpatch(GNode, RIGHT, INode, LEFT);
						backpatch(INode, LEFT, middleUNode,RIGHT);
						lastCheckNode=INode;
						lastUNode= middleUNode;
						
						
					break;
					case DEPENDENCY_I_INDEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_I_INDEPENDENT\n",j,i);
						middleUNode = (node *) createNode(nodeNr, 'U', 0,0,0,0,NULL);
						nodeNr++;
						queue_enqueue(nodes, middleUNode);
						backpatch(dependentCopyNode, LEFT, middleUNode,  LEFT);
						str = malloc(sizeof(char)* 1000);
						sprintf(str, "[");
						for(x=0;x<queue_getCount(d->iDependency);x++){
							if(x>0)sprintf(str, "%s,",str);
							queue *tmpQueue = (queue*) queue_getItem(d->iDependency, x);

							sprintf(str, "%s[%s,%s]", str, (char *) queue_getItem(tmpQueue, 0), 
															(char *) queue_getItem(tmpQueue, 1));
						}
						sprintf(str,"%s]",str);
						INode  = (node *) createNode(nodeNr,'I', 0,0,0,0,str);
						nodeNr++;
						queue_enqueue(nodes, INode);
						backpatch(lastUNode, LEFT, INode, LEFT);
						backpatch(INode, LEFT, middleUNode, RIGHT);
						if(lastCheckNode != NULL){
							backpatch(lastCheckNode, RIGHT, GNode, LEFT);
						}
						lastCheckNode = INode;
						lastUNode     = middleUNode;
					break;
					case DEPENDENCY_INDEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_INDEPENDENT\n",j,i);
						// nothing to do here 
					break;
				}
			}
			
			// createApplynode
			node *aNode = (node *) createNode(nodeNr, 'A',0,0,0,0, NULL);
			nodeNr++;
			backpatch(lastUNode, LEFT, aNode, LEFT);
			if(lastCheckNode != NULL){
				backpatch(lastCheckNode, RIGHT, aNode, LEFT);
			}
			queue_enqueue(nodes, aNode);
			
			// create copyNode
			node  *copyNode = (node *) createNode(nodeNr, 'C',0,0,0,0, NULL);
			nodeNr++;
			backpatch(aNode, LEFT, copyNode, LEFT);
			queue_enqueue(nodes, copyNode);
			queue_enqueue(copyNodes, copyNode);
			
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




