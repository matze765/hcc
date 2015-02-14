#ifndef VARIABLEQUEUELOGIC_H_
#define VARIABLEQUEUELOGIC_H_

#include "queue.h"
#include <string.h>



int QL_isPartOf(char *var, queue *varQueue);
int QL_isEmpty(queue *q1);


queue *QL_union(queue *q1, queue *q2);
queue *QL_intersection(queue *q1, queue *q2);
queue *QL_without(queue *q1, queue *q2);
queue *QL_copy(queue *q);


#endif /*VARIABLEQUEUELOGIC_H_*/