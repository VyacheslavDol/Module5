#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FNAME "numbers"
int main(int argc, char *argv[]){
    pid_t pid;
    int fd[2], count, rv, file_d;
    //FILE *file_name;
    char buf[100];
    srand(time(NULL));
    if(pipe(fd) != 0){  //создание канала
        printf("Error pipe");
        exit(1);
    };  
    
    count = atoi(argv[1]);
    int number[count];

    switch(pid = fork()){
        case 0:         //child process
        close(fd[0]);
        count = atoi(argv[1]);
        for(int i = 0; i <= count - 1; i++){
            number[i] = (rand() % 1000);
            
            sprintf(buf, "%d", number[i]);
            
            if(write(fd[1], buf, strlen(buf) + 1) == 0){
                printf("Ошибка записи в файл\n");
                close(fd[1]);
                return 1;
            }; 
            sleep(2);
        };
        exit;
        break;
    
        default: //parent process
        file_d = open(FNAME, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
        //file_name = fopen(FNAME, "a+");
        for(int i = 0; i <= count - 1; i++){        //цикл с контролем если удалось прочитать то делаем, пока не будет считано необхоиоме количество значений, контроль системных вызовов.
        sleep(1);
        if(read(fd[0], buf, sizeof(buf)) == 0){
                printf("Ошибка чтения в файл\n");
                close(fd[0]);
                return 1;
            };    
        if(write(file_d, buf, sizeof(buf)) == 0){
                printf("Ошибка записи в файл\n");
                close(fd[1]);
                return 1;
            }; 
        //fprintf(file_name, "%s", buf);
        printf("%s\n", buf);
        };
        wait;
        break;
    }
    return 0;
}