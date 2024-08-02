#include <string.h>
#include <stdio.h>
#include "../person.h"


void PersonInit(struct Person* p, const char* firstname, const char * lastname){
	if(p == NULL) return;
	PersonUnInit(p);
	if(firstname != NULL)
		strncpy(p->firstname, firstname, FIRST_NAME_SIZE);
	if(lastname != NULL)	
		strncpy(p->lastname, lastname, LAST_NAME_SIZE);
}	
	
void PersonPrint(struct Person* p){
	if(p == NULL) return;
	printf("%s %s\n", p->firstname, p->lastname);
}

void PersonUnInit(struct Person* p){
	if(p == NULL) return;
	memset(p, 0, sizeof(struct Person));
}