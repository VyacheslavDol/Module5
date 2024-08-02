#ifndef CONTACT_H_
#define CONTACT_H_

#include "person.h"
#include "email.h"
#include "phone.h"

struct Contact {
    struct Person p;
    struct Phone t;
    struct Email e;
};

void ContactInit(struct Contact* c,
                            struct Person p, struct Phone t, struct Email e);
void ContactPrint(struct Contact* c);
void ContactUnInit(struct Contact* c);
void cont_cpy(struct Contact* new_contact, struct Contact* contact);

#endif