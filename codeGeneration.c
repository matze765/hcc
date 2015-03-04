#include "codeGeneration.h"

//private functions
dependency *cg_calc_dependency(queue *variableQueue, queue *newVarQueue, int i, int j);
void cg_write_nodes_to_file(queue *nodes);
void cg_backpatch(node *source, int side, node *target, int port);
int cg_get_adicity(char *literal);

int cg_init_files(){
	codeOutputFile = fopen(CG_CODE_OUTPUT_FILE, "w+");

	if(codeOutputFile == NULL) {
		return -1;
	} else {
		return 0;	
	}
}
	
void cg_close_files(){
	if(codeOutputFile != NULL){
		fclose(codeOutputFile);
	}
	codeOutputFile = NULL;
}
dependency *cg_calc_dependency(queue *variableQueue, queue *newVarQueue, int i, int j) {
	dependency *d = (dependency *) malloc(sizeof(dependency));
	if(d==NULL) return NULL;
	d->gDependency = NULL;
	d->iDependency = NULL;
	
	queue *left 	= queue_getItem(variableQueue, i);
	queue *leftNew 	= queue_getItem(newVarQueue,   i);
	queue *right  	= queue_getItem(variableQueue, j);
	queue *rightNew	= queue_getItem(newVarQueue,   j);
	
	// declare all variables needed later; 
	queue *intersection_LN_R 	= NULL;		// LN ^ R
	queue *intersection_L_R  	= NULL;		// L ^ R
	queue *union_LN_R			= NULL;		// LN u R
	queue *union_L_RN			= NULL;		// L u RN
	queue *A 					= NULL;		// L\(LN u R)
	queue *B					= NULL;		// R\ (L u RN) 
	
	//check for DEPENDENCY_DEPENDENT
	intersection_LN_R = QL_intersection(leftNew,right);
	
	if(!QL_isEmpty(intersection_LN_R)) {
		d->type = DEPENDENCY_DEPENDENT;
	} else {
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
		} else {
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
			} else {
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
				} else {	
					// check for DEPENDENCY_INDEPENDENT
					if(QL_isEmpty(intersection_L_R)
						&&  (QL_isEmpty(A)||QL_isEmpty(B))){			
						d->type = DEPENDENCY_INDEPENDENT;
					} else {
						// should never happen 
						d = NULL;
					}
				}
			}
		}
	}
	
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


//TODO cleanup
void cg_generate_code(queue *variableQueue, queue *newVarQueue, 
			      queue *predicateQueue, queue *literalQueue){

	
					  
	
	queue *nodes = queue_new(); 
	if(queue_getCount(predicateQueue) == 1 ) { // fact
		node *e =	(node *) createNode(nodes, 'E', (char *) queue_getItem(literalQueue, 0));
		node *r = 	(node *) createNode(nodes, 'R',NULL);
		cg_backpatch(e,LEFT_PORT,r,LEFT_PORT);
		
		
		//add predicate to symTab
		st_add_predicate( 	(char *) queue_getItem(predicateQueue, 0),
							cg_get_adicity((char *) queue_getItem(literalQueue, 0)),
							e->nr);
		
		
	} 	else {    // clause
		node *e 	= 	(node *) createNode(nodes, 'E', (char *) queue_getItem(literalQueue, 0));
		
		//add predicate to symTab
		st_add_predicate( 	(char *) queue_getItem(predicateQueue, 0),
							cg_get_adicity((char *) queue_getItem(literalQueue, 0)),
							e->nr);
		
		
		// last U Node in clause
		node *lastU =	e;
		
		// create copy node to distribute entry token 
		node *entryCopyNode = (node *) createNode(nodes, 'C',NULL);
		cg_backpatch(e,RIGHT_PORT, entryCopyNode, LEFT_PORT);
		
		// queue to store the sub goal copy nodes
		queue *copyNodes = queue_new();
		queue_enqueue(copyNodes, entryCopyNode);
		
		//for every subgoal 
		int i;
		for(i=1; i<queue_getCount(literalQueue);i++){
			// create entry U node
			node *entryUNode = (node *) createNode(nodes, 'U', (char *) queue_getItem(literalQueue,i));
			cg_backpatch(entryCopyNode, LEFT_PORT, entryUNode, LEFT_PORT);
			
			// last U Node in sub goal
			node *lastUSub 	= entryUNode;
			// last check (G,I) in sub goal
			node *lastCheckNode = NULL;
			
			// create check nodes for every sub goal left of i
			int j;
			for(j=1;j<i;j++){
				dependency *d = cg_calc_dependency(variableQueue, newVarQueue, j,i);
				node *dependentCopyNode =queue_getItem(copyNodes,j);
				node *middleUNode;
				node *GNode;
				node *INode;
				char *str;
				int x;
				switch(d->type){
					case DEPENDENCY_DEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_DEPENDENT\n", j,i);
						middleUNode = (node *)createNode(nodes,'U',NULL);
						
						cg_backpatch(dependentCopyNode, LEFT_PORT, middleUNode, LEFT_PORT);
						cg_backpatch(lastUSub, LEFT_PORT, middleUNode, RIGHT_PORT);
						lastUSub = middleUNode;
						if(lastCheckNode != NULL){
							cg_backpatch(lastCheckNode, RIGHT_PORT, middleUNode, RIGHT_PORT);
							lastCheckNode= NULL;
						}
					break;
					case DEPENDENCY_G_INDEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_G_INDEPENDENT\n",j,i);
						middleUNode = (node *) createNode(nodes, 'U',NULL);
						cg_backpatch(dependentCopyNode, LEFT_PORT, middleUNode,  LEFT_PORT);
						str = malloc(sizeof(char)* 1000);
						str[0] = 0;
						for(x=0;x<queue_getCount(d->gDependency);x++){
							if(x>0)sprintf(str, "%s,",str);
							sprintf(str, "%s%s", str, (char *) queue_getItem(d->gDependency, x));
						}						
						GNode = (node *) createNode(nodes, 'G', str);
						free(str);
						queue_clear(d->gDependency);
						free(d->gDependency);
					

						cg_backpatch(lastUSub, LEFT_PORT, GNode, LEFT_PORT);
						cg_backpatch(GNode, LEFT_PORT, middleUNode, RIGHT_PORT);
						if(lastCheckNode != NULL){
							cg_backpatch(lastCheckNode, RIGHT_PORT, GNode, LEFT_PORT);
						}	
						lastCheckNode = GNode;
						lastUSub     = middleUNode; 
					break;
					case DEPENDENCY_G_I_INDEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_G_I_INDEPENDENT\n",j,i);
						middleUNode = (node *) createNode(nodes, 'U', NULL);
						cg_backpatch(dependentCopyNode, LEFT_PORT, middleUNode, LEFT_PORT);
						str = malloc(sizeof(char)* 1000);
						str[0] = 0;
						for(x=0;x<queue_getCount(d->gDependency);x++){
							if(x>0)sprintf(str, "%s,",str);
							sprintf(str, "%s%s", str, (char *) queue_getItem(d->gDependency, x));
						}						
						GNode = (node *) createNode(nodes, 'G', str);
						free(str);
						queue_clear(d->gDependency);
						free(d->gDependency);
						
						cg_backpatch(lastUSub, LEFT_PORT, GNode,LEFT_PORT);
						cg_backpatch(GNode, LEFT_PORT, middleUNode, RIGHT_PORT);
						if(lastCheckNode != NULL) {
							cg_backpatch(lastCheckNode, RIGHT_PORT, GNode, LEFT_PORT);
						}
						str = malloc(sizeof(char)* 1000);
						sprintf(str, "[");
						for(x=0;x<queue_getCount(d->iDependency);x++){
							if(x>0)sprintf(str, "%s,",str);
							queue *tmpQueue = (queue*) queue_getItem(d->iDependency, x);

							sprintf(str, "%s[%s,%s]", str, (char *) queue_getItem(tmpQueue, 0), 
															(char *) queue_getItem(tmpQueue, 1));
							queue_clear(tmpQueue);
						}
						sprintf(str,"%s]",str);
						INode  = (node *) createNode(nodes,'I',str);
						free(str);
						queue_clear(d->iDependency);
						free(d->iDependency);
					
						cg_backpatch(GNode, RIGHT_PORT, INode, LEFT_PORT);
						cg_backpatch(INode, LEFT_PORT, middleUNode,RIGHT_PORT);
						lastCheckNode=INode;
						lastUSub= middleUNode;
					break;
					case DEPENDENCY_I_INDEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_I_INDEPENDENT\n",j,i);
						middleUNode = (node *) createNode(nodes, 'U',NULL);
						cg_backpatch(dependentCopyNode, LEFT_PORT, middleUNode,  LEFT_PORT);
						str = malloc(sizeof(char)* 1000);
						sprintf(str, "[");
						for(x=0;x<queue_getCount(d->iDependency);x++){
							if(x>0)sprintf(str, "%s,",str);
							queue *tmpQueue = (queue*) queue_getItem(d->iDependency, x);

							sprintf(str, "%s[%s,%s]", str, (char *) queue_getItem(tmpQueue, 0), 
															(char *) queue_getItem(tmpQueue, 1));
							queue_clear(tmpQueue);
						}
						sprintf(str,"%s]",str);
						INode  = (node *) createNode(nodes,'I', str);
						free(str);
						queue_clear(d->iDependency);
						free(d->iDependency);
						
						cg_backpatch(lastUSub, LEFT_PORT, INode, LEFT_PORT);
						cg_backpatch(INode, LEFT_PORT, middleUNode, RIGHT_PORT);
						
						if(lastCheckNode != NULL){
							cg_backpatch(lastCheckNode, RIGHT_PORT, GNode, LEFT_PORT);
						}
						lastCheckNode = INode;
						lastUSub     = middleUNode;
					break;
					case DEPENDENCY_INDEPENDENT:
						printf("DEP(%d,%d)= DEPENDENCY_INDEPENDENT\n",j,i);
						// nothing to do here 
					break;
				}
			}
				
			// createApplynode
			node *aNode = (node *) createNode(nodes, 'A', NULL);
			cg_backpatch(lastUSub, LEFT_PORT, aNode, LEFT_PORT);
			if(lastCheckNode != NULL){
				cg_backpatch(lastCheckNode, RIGHT_PORT, aNode, LEFT_PORT);
			}
			
			//createCopyNode
			node  *copyNode = (node *) createNode(nodes, 'C', NULL);
			cg_backpatch(aNode, LEFT_PORT, copyNode, LEFT_PORT);
			queue_enqueue(copyNodes, copyNode);
			
			// create exit U node
			node *exitUNode = (node *) createNode(nodes, 'U', NULL);
			cg_backpatch(copyNode,LEFT_PORT, exitUNode, LEFT_PORT);
			cg_backpatch(lastU, LEFT_PORT, exitUNode, RIGHT_PORT);
			lastU = exitUNode;
		}
		// create return node
		node *returnNode = (node *) createNode(nodes, 'R', NULL);
		cg_backpatch(lastU, LEFT_PORT, returnNode, LEFT_PORT);		
	}
	// write code to file
	cg_write_nodes_to_file(nodes);
	
	//clean up
	
	queue_clear(nodes);
	free(nodes);
}

void cg_write_nodes_to_file(queue *nodes){
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
}
void cg_backpatch(node *source, int side, node *target, int port){
	if(source->type == 'C'){
		copyNode_add((copyNode *)source, target->nr, port);
	} else {
		if(side == LEFT_PORT) {
			source->leftNode = target->nr;
			source->leftNodePort = port;
		} else if( side == RIGHT_PORT){
			source->rightNode = target->nr; 
			source->rightNodePort = port; 
		}
	}
	
}

int cg_get_adicity(char *literal) {
	int adicity =1;
	char *a = literal;
	while(*a != '\0'){
		if(*a==',') adicity++;
		a++;
	}
	return adicity;
	
}

