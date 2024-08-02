#include <stdio.h>
#include <string.h>
#include "../contact.h"


void ContactInit(struct Contact* c,
                            struct Person p, struct Phone t, struct Email e){
if(c == NULL) return;
ContactUnInit(c);
c->e = e;
c->p = p;
c->t = t;

}

void ContactPrint(struct Contact* c){
    if(c == NULL) return;
    printf("Name "); PersonPrint(&(c->p));
    printf("Phone: "); PhonePrint(&(c->t));
    printf("Email: "); EmailPrint(&(c->e));
}

void ContactUnInit(struct Contact* c){
    if(c == NULL) return;
    PersonUnInit(&(c->p));
    PhoneUnInit(&(c->t));
    EmailUnInit(&(c->e));
}

void cont_cpy(struct Contact* new_contact, struct Contact* contact){
    strcpy(new_contact->p.firstname, contact->p.firstname);
    strcpy(new_contact->p.lastname, contact->p.lastname);
    strcpy(new_contact->t.number, contact->t.number);
    strcpy(new_contact->e.email, contact->e.email);
}
