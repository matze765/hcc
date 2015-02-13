#include <stdlib.h>
#include <stdio.h>
#include "queue.h"


queue* queue_new(){
	queue *q = (queue*) malloc(sizeof(queue)); 
	if(q==NULL) return q;
	q->first = NULL;
	return q;
}
void queue_clear(queue* q){
	queue_item *cur = q->first;
	while(cur != NULL){
		queue_item *ncur = cur->next;
		free(cur->data);
		free(cur);
		cur=ncur;
	}
	q->first = NULL;
}
int queue_enqueue(queue* q,void* item) {
	if(q == NULL) return -1;
	queue_item *new  = (queue_item *) malloc(sizeof(queue_item));
	if(new == NULL) return -1;
	new->data = item;
	new->next = NULL;
	queue_item *cur = q->first;
	if(cur == NULL){
		q->first = new;
	} else {
		while(cur->next != NULL){
			cur = cur->next;
		}
		cur->next = new; 
	}
	return 0;
}

void *queue_dequeue(queue *q) {
   if(q == NULL || q->first == NULL) return NULL;
	queue_item *first = q->first;
	queue_item *second = first->next; 
	
	q->first = second;
	first->next = NULL;
	
	return first; 
}
void *queue_getItem(queue* q, int i){
	if(i >= queue_getCount(q)){
		return NULL;
	} else {
		int j=0;
		queue_item *cur = q->first;	
		while(1){
			if(j==i){
				return cur->data;
			}
			j++;
			cur = cur->next;
		}
	}
}
int queue_getCount(queue* q){
	int i=0; 
	queue_item *cur = q->first;
	while(cur != NULL){
		i++;
		cur = cur->next;
	}
	return i;
}