#include <string.h>
#include <stdio.h>
#include "../phone.h"


void PhoneInit(struct Phone* t, const char* number){

	if(t == NULL) return;
	PhoneUnInit(t);
	if(number!= NULL)
		strncpy(t->number, number, PHONE_NUMBER_SIZE);

}	
	
void PhonePrint(struct Phone* t){
	if(t == NULL) return;
	printf("%s\n", t->number);
}

void PhoneUnInit(struct Phone* t){
	if(t == NULL) return;
	memset(t, 0, sizeof(struct Phone));
}