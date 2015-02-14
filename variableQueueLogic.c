#include "variableQueueLogic.h"


int QL_isPartOf(char *var, queue *varQueue) {
	if(varQueue==NULL) return 0;
	int i;
	for(i=0;i<queue_getCount(varQueue);i++){
		char *str= (char *)  queue_getItem(varQueue,i);
		if(!strcmp(str, var)) {
			return 1;
		}
	}
	return 0;	
}
int QL_isEmpty(queue *q1){
	return (queue_getCount(q1)==0);
}


queue *QL_union(queue *q1, queue *q2) {
	queue *res = QL_copy(q1);
	int i;
	for(i=0;i<queue_getCount(q2);i++){
		char *str = (char *) queue_getItem(q2, i);
		if(!QL_isPartOf(str, res)){
			char *strdupl = strdup(str);
			queue_enqueue(res, strdupl);
		}
	}	
}
queue *QL_intersection(queue *q1, queue *q2){
	if(q1==NULL|| q2 == NULL) return NULL;
	queue *res = queue_new();
	int i,j;
	for(i=0;i<queue_getCount(q1);i++){
		char *v1 =  (char *)queue_getItem(q1, i);
		for(j=0;j<queue_getCount(q2);j++){
			char *v2 = (char *)queue_getItem(q2, j);
			if(!strcmp(v1,v2)){
				queue_enqueue(res, strdup(v1));
			} 
		}
	}
	return res;
}

queue *QL_without(queue *q1, queue *q2){
	queue *res = queue_new();
	int i;
	for(i=0;i<queue_getCount(q1);i++){
		char *var = (char *) queue_getItem(q1, i);
		if(!QL_isPartOf(var, q2)){
			char *cpy = strdup(var);
			queue_enqueue(res, cpy);
		}
	}
	return res;
}

queue *QL_copy(queue *q){
	queue *res = queue_new();
	int i; 
	for(i=0;i<queue_getCount(q);i++){
		char *str = strdup((char *) queue_getItem(q,i));
		queue_enqueue(res, str);
	}
	return res;
}