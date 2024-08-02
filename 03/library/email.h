#ifndef EMAIL_H_
#define EMAIL_H_
#define EMAIL_SIZE 40
#include <stdlib.h>

struct Email {
    char email[EMAIL_SIZE];
};

void EmailInit(struct Email* e, const char* email);
void EmailPrint(struct Email* e);
void EmailUnInit(struct Email* e);

#endif