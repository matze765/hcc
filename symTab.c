#include "symTab.h"

symbolTable *symTable;


int st_init_symTab(){
	symTable = malloc(sizeof(symbolTable));
	if(symTable == NULL) return -1;
	symTable->symTabEntries = queue_new();
	if(symTable->symTabEntries == NULL) return -1;
	
	symTabFile	 = fopen(ST_SYMTAB_FILE, "w+");
	if(symTabFile == NULL) return -1;
	
	return 0;
}

void st_close_files(){
	if(symTabFile != NULL){
		fclose(symTabFile);
	}
	symTabFile = NULL;
}

void st_add_predicate(char *pName, int adicity, int entryLine){
	printf("adding %s\n", pName);
	symTabEntry *item;
	int *intcopy = malloc(sizeof(int));
	int i;
	for(i=0;i<queue_getCount(symTable->symTabEntries);i++){
		item = (symTabEntry*) queue_getItem(symTable->symTabEntries,i);
		if( item->adicity == adicity &&
		    strcmp(item->predicateName, pName)==0){
			// predicate does already exist ... add entry node
			*intcopy =entryLine;
			queue_enqueue(item->entryNodes, intcopy);
			return;
		}
	}
	// predicate does not exist ... add new predicate to symTable
	item = (symTabEntry*) malloc(sizeof(symTabEntry));
	item->predicateName 	= strdup(pName);
	item->adicity			= adicity;
	item->entryNodes 		= queue_new();
	*intcopy				= entryLine;
	queue_enqueue(item->entryNodes,intcopy);
	queue_enqueue(symTable->symTabEntries, item);
}

void st_print_symTab(){
	printf("printing %d\n",  queue_getCount(symTable->symTabEntries));
	FILE *f = symTabFile;
	
	symTabEntry *item = (symTabEntry *) queue_dequeue(symTable->symTabEntries);
	while(item != NULL){
		fprintf(f, "%s\t%d\t[", item->predicateName, item->adicity);
		int i;
		for(i=0;i<queue_getCount(item->entryNodes);i++){
			int *line = (int*) queue_getItem(item->entryNodes,i);
			if(i!=0) fprintf(f, ",");
			fprintf(f,"%d",*line);
		}
		fprintf(f,"]\n");
		free(item->predicateName);
		queue_clear(item->entryNodes);
		free(item->entryNodes);
		free(item);
		item = (symTabEntry *) queue_dequeue(symTable->symTabEntries);
	}
	free(symTable->symTabEntries);
	free(symTable);
}
