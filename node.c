#include "node.h"


node *node_new() {
	node *n = (node *) malloc(sizeof(node));
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
	cn->type = 'C';
	cn->nextNodes = queue_new();
	return cn;
}

void *createNode(int nr, char type, int leftNode, int leftNodePort,
				 int rightNode, int rightNodePort, char *str){
	node *n; copyNode *cn;
	
	if(type == 'C'){
		cn = copyNode_new();
		cn->nr = nr; 
		return cn;
	} else {
		n = node_new();
		n->nr   = nr;
		n->type = type;
		n->leftNode  = leftNode;
		n->leftNodePort = leftNodePort;
		n->rightNode = rightNode;
		n->rightNodePort = rightNodePort;
		if(str != NULL) {
			char *strcp = strdup(str);
			n->str = strcp;
		} else {
			n->str = NULL;
		}
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