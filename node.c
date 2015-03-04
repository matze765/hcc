#include "node.h"


int nodeNr=1;

node *node_new() {
	node *n = (node *) malloc(sizeof(node));
	if(n == NULL) return n;
	n->nr = 0;
	n->type = 'X';
	n->leftNode = 0;
	n->leftNodePort = 0;
	n->rightNode = 0;
	n->rightNodePort = 0; 
	n->str = NULL;
	return n;
}

copyNode *copyNode_new(){
	copyNode *cn = (copyNode *) malloc(sizeof(copyNode));
	if(cn == NULL) return cn;
	cn->type = 'C';
	cn->nextNodes = queue_new();
	return cn;
}

void *createNode(queue *nodeQueue, char type,  char *str){
	node *n; copyNode *cn;
	if(type == 'C'){
		cn = copyNode_new();
		if(cn == NULL) return cn;
		cn->nr = nodeNr;
		nodeNr++;
		queue_enqueue(nodeQueue, cn);
		return cn;
	} else {
		n = node_new();
		if(n==NULL) return n;
		n->nr   = nodeNr;
		nodeNr++;
		n->type = type;
		n->leftNode  		= 0;
		n->leftNodePort 	= 0;
		n->rightNode 		= 0;
		n->rightNodePort 	= 0;
		if(str != NULL) {
			char *strcp = strdup(str);
			n->str = strcp;
		} else {
			n->str = NULL;
		}
		queue_enqueue(nodeQueue, n);
		return n;
	}
}

void copyNode_add(copyNode * cn, int target, int port) {
	int *val = (int *) malloc(2*sizeof(int));
	val[0] = target;
	val[1] = port;
	if(cn->nextNodes == NULL ) {
		cn->nextNodes = queue_new();
	}
	queue_enqueue(cn->nextNodes, val);
}