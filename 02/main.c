#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_WORDS 10
#define MAX_LENGTH 20
int main(){
    char word[MAX_WORDS][MAX_LENGTH], input[100], *marker;
    int rv, flag = 1, i = 0;
    pid_t pid;
    while(flag == 1){
    printf("Введите команду и аргументы (exit to leave):\n");
    fgets(input, sizeof(input)/sizeof(input[0]), stdin);
    if(strcmp(input, "exit\n") == 0)
        return 0;
    marker = strtok(input, " ");
    i = 0;
    while(marker != NULL && i < MAX_WORDS){
        strncpy(word[i], marker, MAX_LENGTH);
        marker = strtok(NULL, " ");
        i++;
    };
    if(strcmp(input, "exit") == 0)
        return 0;
switch(pid = fork()){

case -1:
perror("fork"); /* произошла ошибка */
exit(EXIT_FAILURE);
break;

case 0:
    if(strcmp(word[0], "sum") == 0){                    //сумма двух чисел
    execl(word[0], word[0], word[1], word[2], NULL);
    }
    else if(strcmp(word[0], "square") == 0){            //квадрат числа
    execl(word[0], word[0], word[1], NULL);
    }
    else if(strcmp(word[0], "maxnum") == 0){            //максимальное число из четырех чисел
    execl(word[0], word[0], word[1], word[2], word[3], word[4], NULL);
    }           
    else if(strcmp(word[0], "leng") == 0){              //самое длинное слово из четырех слов
    execl(word[0], word[0], word[1], word[2], word[3], word[4], NULL);
    };
    exit(EXIT_SUCCESS);
default:
    wait(&rv);
};
}
   return 0;
}