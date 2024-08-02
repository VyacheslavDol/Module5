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
#include <signal.h>
#define FNAME "numbers"

int flag = 0;  //глобальная переменная, овтечабщая за доступ дочернему процессу к чтению / записи

void access_1(){    //REACTION ON SIUSR2
    flag = 1; 
}

void access_0(){    //REACTION ON SIUSR1
    flag = 0;
}

int main(int argc, char *argv[]){
    pid_t pid;
    int fd[2], count, rv, fd_write, fd_read, number, read_number;
    srand(time(NULL));
    char read_buff[20];
    flag = 0;

    signal(SIGUSR1, access_0);     //изменение рееакций на сигнал по функциям                             
    signal(SIGUSR2, access_1);
    
    if(pipe(fd) != 0){  //create stream проверка на ошибку
        printf("Error pipe");
        exit(1);
    };  
    
    count = atoi(argv[1]);
    
    switch(pid = fork()){   

        case 0:         //child process
        close(fd[0]);
        count = atoi(argv[1]);
        for(int i = 0; i <= count - 1; i++){
            number = (rand() % 1000);
            if(write(fd[1], &number, sizeof(number)) == 0){
                printf("Ошибка записи в файл\n");
                close(fd[1]);
                exit(EXIT_FAILURE);
            };
            printf("Child: Я записал в канал %d\n", number);

        while(flag == 0){
            //ожидание получения доступа на чтение файла
        };
        if(flag == 1){
        
        fd_read = open(FNAME, O_RDONLY);        //открытие файла на чтение
        if (fd_read == -1) {
        perror("Ошибка открытия файла для чтения");
        exit(EXIT_FAILURE);
        }                       

        if(read(fd_read, &read_number, sizeof(read_number)) == -1){
            printf("Прочитать информацию из файла не удалось");
            exit(EXIT_FAILURE);
        };
        printf("Child: Я прочитал из файла %d\n", read_number);
        close(fd_read);

        kill(getpid(), SIGUSR1);                //отправка сигнала закрывающего доступ "самому себе"

        };
        };
        close(fd[1]);
        exit(EXIT_SUCCESS);
        break;
    
        default: //parent process

        for(int i = 0; i <= count - 1; i++){        //цикл с контролем если удалось прочитать то делаем, пока не будет считано необхоиоме количество значений, контроль системных вызовов.
        sleep(1);
        if(read(fd[0], &number, sizeof(number)) == 0){
            printf("Ошибка чтения в файл\n");
            close(fd[0]);
            return 1;
        };
        printf("Parrent: Я прочитал из канала  %d\n", number);

        kill(pid, SIGUSR1);        //запрет детскому процессу на чтение файла

        fd_write = open(FNAME, O_WRONLY | O_CREAT, 0644);       //открытие файла для записи
        if (fd_write == -1) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }
        printf("Parrent: Доступ закрыт\n");
        if(write(fd_write, &number, sizeof(number)) == 0){    //запись полученного числа в файл
                printf("Ошибка записи в файл\n");
                close(fd[1]);
                return 1;
            };
        printf("Parrent: Я записал в файл %d\n", number);
        close(fd_write);
        kill(pid, SIGUSR2);        //разрешение детскому процессу на чтение файла
        printf("Parrent: Доступ открыт\n");
        
        sleep(3);
        };
        printf("Я жду пока завершитя дочерний процесс\n");
        wait(&rv);
        printf("parrent: Я завершаюсь\n");
        break;
    }
    return 0;
}