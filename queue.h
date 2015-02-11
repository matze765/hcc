#ifndef QUEUE_H_
#define QUEUE_H_
typedef struct queue_item {
	struct queue_item *next;
	void *data;
} queue_item;

typedef struct queue {
	struct queue_item *first;
} queue;

queue* queue_new();
void queue_clear(queue*);
int queue_enqueue(queue*,void*);
void *queue_getItem(queue*, int);
int queue_getCount(queue*);




#endif /*COMPILER_H_*/
