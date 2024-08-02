#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../library/contact.h"
#include "../library/input_char.h"
#include "../library/list_func.h"

#define ADD 1
#define LOOK_C 2
#define LOOK_L 5
#define EDIT 3
#define DELETE 4
#define EXIT 0
#define FILENAME "contact_book"

int main(){
    
    struct Item* head = NULL;
    struct Item* tmp;
    int choice, exit_flag = 1, flag, fd, a = 1;
    struct Contact new_contact;

/*     fd = open(FILENAME, O_CREAT, 0660); //создание файла
    close(fd); */

    fd = open(FILENAME, O_CREAT | O_RDONLY, 0660);      //открытие (создание) файла для чтения контактной книги
    //printf("fd = %d\n", fd);
    if(fd == -1)
    {
        perror("Ошибка открытия файла");
        return 1;
    };

    //чтение контактной книги из файла        
    while(read(fd, &new_contact, sizeof(new_contact)) != 0)
    {
        head = InsertItem(&new_contact, head);
    }

    if(close(fd) == -1)     //закрытие файла
    {
        perror("Ошибка закрытия файла");
        return 1;
    };

    while(exit_flag){
    printf("\nВыберите действие:\n1.Создать контакт\n2.Просмотреть контакт\n3.Редактировать контакт\n4.Удалить контакт\n5.Список контактов\n0.Выход\n");
    choice = get_ans_int();
        
    switch(choice){

        case ADD:                       // созданиe контакта
            if(add_contact(&new_contact, head) == -1) break;         //функция заполнения полей данных
            head = InsertItem(&new_contact, head);                   //функция добавления контакта в список
        break;

        case LOOK_C:            //просмотр контакта
            if(head == NULL)
            {
                printf("Список пуст\n");
                break;
            }
            flag = 1;
            print_list(head);
            printf("Выберите номер контакта по порядку\n");
            choice = get_ans_int();
            tmp = get_tmp_choice(head, choice, &flag);
            if(flag == 0) break;
            print_item(head, tmp);
        break;
        
        case DELETE:            //удаление контакта
        if(head == NULL)
        {
            printf("Список пуст\n");
            break;
        }        
            flag = 1;
            print_list(head);
            printf("Выберите номер контакта по порядку\n");
            if((choice = get_ans_int()) == 0) break;
            //printf("choice = %d\n", choice);
            tmp = get_tmp_choice(head, choice, &flag);
            if(flag == 0) break;
            head = delete_item(head, tmp);
        break;

        case LOOK_L:        //просмотр списка контактов
        if(head == NULL)
        {
            printf("Список пуст\n");
            break;
        }        
            print_list(head);
        break;

        case EDIT:          //редактирование контакта
        if(head == NULL)
        {
            printf("Список пуст\n");
            break;
        }        
            flag = 1;
            print_list(head);
            printf("Выберите номер контакта по порядку\n");
            choice = get_ans_int();
            tmp = get_tmp_choice(head, choice, &flag);
            if(flag == 0) break;

            head = edit_item(head, tmp);
        break;

        case EXIT:          //выход из программы
            exit_flag = 0;
        break;

        default:
            printf("Incorrect input\n");
        break;
    }

}

    fd = open(FILENAME, O_TRUNC | O_WRONLY | O_APPEND); //открытие файла c перезаписыванием данных для записи
    if(fd == -1)
    {
        perror("ошибка открытия");
        return 1;
    }

    tmp = head;

    write_book(tmp, fd);    //запись контактной книги

    if(close(fd) == -1){
        perror("ошибка закрытия");
        return 1;
    }

    //очистка занимаемой динамической памяти (кучи)
    while(head != NULL)     
    {
        tmp = head->next;
        free(head);
        head = tmp;
    };
    printf("Память освобождена\n");

    return 0;
}