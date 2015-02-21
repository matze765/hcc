#!/bin/sh


bison -d -v hcc.y
if [ "$?" != 0 ]; then 
	exit 1;
fi

flex hcc.l
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -c hcc.tab.c -o hcc_y.o
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -Wall -c lex.yy.c
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -c hcc.tab.c -o hcc_y.o
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -Wall -c hcc.c 
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -Wall -c queue.c 
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -Wall -c node.c 
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -Wall -c variableQueueLogic.c 
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -Wall -c codeGeneration.c 
if [ "$?" != 0 ]; then 
	exit 1;
fi

gcc -g -o hcc codeGeneration.o hcc.o queue.o node.o variableQueueLogic.o hcc_y.o lex.yy.o -lm -lfl
if [ "$?" != 0 ]; then 
	exit 1;
fi
