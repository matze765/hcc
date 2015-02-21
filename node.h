#ifndef NODE_H_
#define NODE_H_

#include "queue.h"
#include <string.h>
#include <stdlib.h>

typedef struct node {
	int nr;
	char type;
	int leftNode;
	int leftNodePort;
	int rightNode;
	int rightNodePort;
	char *str;
} node;

typedef struct copyNode {
	int nr;
	char type; // == C
	queue *nextNodes;
} copyNode;

node* node_new();
copyNode *copyNode_new();

void *createNode(queue *nodeQueue, char type, char *str);
				 
void copyNode_add(copyNode *cn, int target, int port);

#endif /*NODE_H_*/