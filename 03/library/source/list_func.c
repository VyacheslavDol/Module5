#include "../list_func.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../contact.h"

struct Item* add_to_list(struct Item* head, struct Item* new_item){
    struct Item* tmp; // указатель который идет сначала списка
    tmp = head;
    int flag = 1, i = 0, end_list = 0;

    if(head == NULL)    // вставка в пустой список
    {
        new_item->next = NULL;
        new_item->prev = new_item;
        head = new_item;
        return head;
    }

    while(flag)            //сравнение первой буквы имени
    {
        if(new_item->val_contact.p.firstname[i] < tmp->val_contact.p.firstname[i])
        {
            flag = 0;        //если слово из списка имеет более "позднюю" букву по алфавиту, то вставляем перед tmp
        } 
        else if(new_item->val_contact.p.firstname[i] > tmp->val_contact.p.firstname[i] && tmp->next != NULL)
        {
            tmp = tmp->next;    //если слово из списка имеет более "раннюю букву" по алфавиту, и  если слово не конец списка то сдвигаемся по списку
            i = 0;
        }
        else if(new_item->val_contact.p.firstname[i] > tmp->val_contact.p.firstname[i] && tmp->next == NULL)
        {
            break;      //вставляем в конец списка, после tmp (flag = 1)
        }
        else if(new_item->val_contact.p.firstname[i] = tmp->val_contact.p.firstname[i])
        {
            i++;  //если буквы одинаковые смотри следующую
        }; 
    }
        if(tmp->next == NULL && flag == 1)  //вставка в конец списка, tmp - предыдущий new_item
        {
            tmp->next = new_item;
            new_item->prev = tmp;
            new_item->next = NULL;
        }
        else if(tmp != head && flag == 0)                //если в середину, tmp - после new_item
        {
            new_item->next = tmp;
            new_item->prev = tmp->prev;
            tmp->prev->next = new_item;
            tmp->prev = new_item;
        }
        else if(tmp == head && flag == 0)                //если в начало, tmp - после new_item
        {
            tmp->prev = new_item;
            head = new_item;
            new_item->next = tmp;
            new_item->prev = new_item;
        }
    
    return head;     
}

struct Item* InsertItem(struct Contact* contact, struct Item* head){    //добавление контакта в список

    struct Item* new_item;
    struct Item* tmp;

    if((new_item = malloc(sizeof(struct Item))) == NULL) //выделение памяти для нового элемента и получение указателя на элемент
    {
        printf("Malloc error");
        exit(EXIT_FAILURE) /* ошибка */;
    }

    cont_cpy(&(new_item->val_contact), contact);    //заполнение поля контакт внутри структуры Item, (value)

    //функция перебора списка и вставка в список
    head = add_to_list(head, new_item);

    return head;
}

struct Item* get_tmp_choice(struct Item* head, int choice, int* flag)
{   
    struct Item* tmp;
    tmp = head;
    for(int i = 1; i < choice; i++){
        tmp = tmp->next;
        if(tmp == NULL){
          *flag = 0;
          printf("Incorrect input\n");
          return head;  
        }   
    }
    return tmp;
};

int add_contact(struct Contact* contact, struct Item* head){           //создание контакта (заполнение полей данных о контакте)
    char firstname[FIRST_NAME_SIZE], lastname[LAST_NAME_SIZE], phone[PHONE_NUMBER_SIZE], email[EMAIL_SIZE];
    char ans[2];
    int ansi, name_check = 0;
    struct Item* tmp;

    struct Person p;
    struct Phone t;
    struct Email e;

    memset(firstname, 0, sizeof(firstname));
    memset(lastname, 0, sizeof(lastname));
    memset(phone, 0, sizeof(phone));
    memset(email, 0, sizeof(email));

    do{
    printf("Имя: ");
    input_char(firstname, sizeof(firstname)); 
    } while(strcmp(firstname, "\0") == 0);

    //name check
    tmp = head;
    while(tmp != NULL)
    {
        if((strcmp(firstname, tmp->val_contact.p.firstname) == 0))
        {
            printf("Контакт с таким именем уже существует\n");
            name_check = 1;
            break;
        }
        tmp = tmp->next;
    }
    if(name_check == 1) return -1;
    
    do{
    printf("Фамилия: ");
    input_char(lastname, sizeof(lastname));
    } while(strcmp(lastname, "\0") == 0);
        
    printf("Заполнить номер телефона? 1.да 2.нет\n");
    ansi = get_ans_int();
    if(ansi == 1){
        printf("Номер телефона: ");
        input_char(phone, sizeof(phone));
    }

    printf("Заполнить адрес электронной почты 1.да 2.нет\n");
    ansi = get_ans_int();
    if(ansi == 1){
        printf("Элеткронная почта: ");
        input_char(email, sizeof(email));
    }

    PersonInit(&p, firstname, lastname);
    PhoneInit(&t, phone);
    EmailInit(&e, email);

    ContactInit(contact, p, t, e);
        
}

void print_list(struct Item* head){
    struct Item* tmp;
    tmp = head;
    for(int i = 1; tmp != NULL; i++)
    {
        printf("%d ",i); PersonPrint(&(tmp->val_contact.p));
        tmp = tmp->next;
    }
    return;
}

void print_item(struct Item* head, struct Item* tmp){ 
    ContactPrint(&(tmp->val_contact));
    return;
}

struct Item* delete_item(struct Item* head, struct Item* tmp){
    /*удаление tmp элемента*/
    if(tmp == head)             //удаление из начала списка
    {
        head = tmp->next;
        if(tmp->next != NULL)
        tmp->next->prev = tmp->next;
    }else if(tmp->next == NULL) //удаление конеченого элемента списка
    {
        tmp->prev->next = NULL;
    }else                       //удаление из середины
    {
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
    }
    ContactUnInit(&(tmp->val_contact));
    free(tmp);
    return head;
}

struct Item* edit_item(struct Item* head, struct Item* tmp){

    char firstname[FIRST_NAME_SIZE], lastname[LAST_NAME_SIZE], phone[PHONE_NUMBER_SIZE], email[EMAIL_SIZE];
    int choice;

    ContactPrint(&(tmp->val_contact));
    printf("Выберите, что изменить\n1.Имя\n2.Номер телефона\n3.Адрес\n");
    choice = get_ans_int();
    
    if(choice == 2){
        printf("Введите новый номер: ");
        memset(phone, 0, sizeof(phone));
        input_char(phone, sizeof(phone));
        strcpy(tmp->val_contact.t.number, phone);
    }else if(choice == 3){
        printf("Введите новый адрес: ");
        memset(email, 0, sizeof(email));
        input_char(email, sizeof(email));
        strcpy(tmp->val_contact.e.email, email);
    }else if(choice == 1)
    { 
        /*name chnge*/
        struct Contact edit_contact;                
        memset(firstname, 0, sizeof(firstname));
        memset(lastname, 0, sizeof(lastname));
        printf("Введите новое имя: ");
        input_char(firstname, sizeof(firstname));   //ввод новых значений
        printf("Введите новую фамилию: ");
        input_char(lastname, sizeof(lastname));

        strcpy(edit_contact.t.number, tmp->val_contact.t.number);   //копирование значений почты и телефона из "старого контакта в новый"
        strcpy(edit_contact.e.email, tmp->val_contact.e.email);
        strcpy(edit_contact.p.firstname, firstname);            //вставляем имя и фамилию
        strcpy(edit_contact.p.lastname, lastname);

        head = delete_item(head, tmp);
        head = InsertItem(&edit_contact, head);
        
    }
    return head;
}

int write_book(struct Item* tmp, int fd)
{
    while(tmp != NULL)
    {
    if(write(fd, &(tmp->val_contact), sizeof(tmp->val_contact)) == -1)
    {
        perror("Ошибка записи имени");
        return 1;
    };
    
    tmp = tmp->next;

    }
    return 0;
}  