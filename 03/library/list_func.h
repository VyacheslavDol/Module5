#ifndef LIST_FUNC_H_
#define LIST_FUNC_H_
#include "contact.h"
#include "input_char.h"


struct Item{
    struct Contact val_contact;
    struct Item* next;
    struct Item* prev;
};

int add_contact(struct Contact* contact, struct Item* head);        //создание контакта (заполнение полей данных о контакте)

struct Item* add_to_list(struct Item* head, struct Item* new_item);

struct Item* InsertItem(struct Contact* contact, struct Item* head);   //добавление контакта в список

struct Item* get_tmp_choice(struct Item* head, int choice, int* flag);

void print_list(struct Item* head);

void print_item(struct Item* head, struct Item* tmp);

struct Item* delete_item(struct Item* head, struct Item* tmp);

struct Item* edit_item(struct Item* head, struct Item* tmp);

int write_book(struct Item* tmp, int fd);   //запись контактной книги в файл

#endif