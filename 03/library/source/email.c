#include <stdio.h>
#include <string.h>

#include "../email.h"

void EmailInit(struct Email* e, const char* email){
    if(e == NULL) return;
    EmailUnInit(e);
    if(email != NULL)
        strncpy(e->email, email, EMAIL_SIZE);
}

void EmailPrint(struct Email* e){
    if(e == NULL) return;
    printf("%s\n", e->email);
}

void EmailUnInit(struct Email* e){
    if(e == NULL) return;
    memset(e, 0, sizeof(struct Email));
}