#ifndef PHONE_H_
#define PHONE_H_

#include <stdlib.h>

#define PHONE_NUMBER_SIZE 20
#define MAX_PHONE_COUNT 3  // не реализовано
struct Phone {
	char number[PHONE_NUMBER_SIZE];
};

void PhoneInit(struct Phone* p, const char* number);
void PhonePrint(struct Phone* p);
void PhoneUnInit(struct Phone* p);

#endif