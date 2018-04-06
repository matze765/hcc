
CC = gcc
CCFLAGS = -Wall 
LD = gcc
LDFLAGS = -lm 
PARSER = bison
PARSERFLAGS = -d 
LEXER = flex
OBJS = hcc.tab.o hcc.yy.o symTab.o codeGeneration.o hcc.o queue.o node.o variableQueueLogic.o

all: hcc


hcc: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)   

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<	  

%.yy.c: %.l %.tab.c
	$(LEXER) -o $@ $<

%.tab.c: %.y
	$(PARSER) $(PARSERFLAGS) $<


clean: 
	rm -f *.o
	rm -f *.output
	rm -f *.tab.c
	rm -f *.tab.h
	rm -f *.yy.c
	rm -f hcc
	rm -f out.tab
	rm -f out
