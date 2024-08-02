#ifndef PERSON_H_
#define PERSON_H_

#include <stdlib.h>

#define FIRST_NAME_SIZE 20
#define LAST_NAME_SIZE 20

struct Person {
	char firstname[FIRST_NAME_SIZE];
	char lastname[LAST_NAME_SIZE];
};

void PersonInit(struct Person* p, const char* firstname, const char* lastname);
void PersonPrint(struct Person* p);
void PersonUnInit(struct Person* p);

#endif