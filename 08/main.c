#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define FNAME "numbers"

union semun {
int val; /* значение для SETVAL */
struct semid_ds *buf; /* буферы для IPC_STAT, IPC_SET */
unsigned short *array; /* массивы для GETALL, SETALL */
/* часть, особенная для Linux: */
struct seminfo *__buf; /* буфер для IPC_INFO */
};

int main(int argc, char *argv[]){

    if(argc != 2) 
        if(argc != 2)
    {
        printf("Incorrect input\n");
        exit(EXIT_FAILURE);
    };

    int count = atoi(argv[1]);

    pid_t pid;
    key_t key;
    int sem, fd[2], number, fd_read, fd_write, read_number, value;
    union semun arg;
    
    struct sembuf wait_0 = {0, 0, 0};       //"ожидание нуля"
    struct sembuf open_res = {0, -1, 0};    //уменьшение значения семофора на -1
    struct sembuf close_res = {0, 1, 0};    //увеличение значения семофора на +1

    srand(time(NULL));

    //создание канала
    if(pipe(fd) != 0){  
        printf("Error pipe");
        exit(1);
    };

    //Cоздание уникального ключа
    key = ftok("/home", 'q');       
    if(key == -1){
        printf("Ошибка создания ключа подключения\n");
        exit(1);
    }

    // Создание семафора 
    sem = semget(key, 1, IPC_CREAT | 0666);
    if (sem == -1) 
    {
        perror("semget");
        exit(1);
    }

    if((value = semctl(sem, 0, GETVAL, arg)) != 1){
    arg.val = 1;
    semctl(sem, 0, SETVAL, arg);
    };
    //создание подпроцесса
    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;

    case 0: // Child proccess

        close(fd[0]); //закрываем канал на чтение

    for(int i = 0; i <= count - 1; i++){

        number = (rand() % 1000);   //генерация случайных чисел

        if(write(fd[1], &number, sizeof(number)) == 0){     //запись случайных числе в канал
            printf("Ошибка записи в файл\n");
            close(fd[1]);
            exit(EXIT_FAILURE);
        };
        
        printf("Child: Я записал в канал %d\n", number);
        /* printf("Child: я жду пока значение семафора не станет 0\n"); */

        if(semop(sem, &wait_0, 1) == -1) {    //ожидание 0(разрешения на чтение из файла)
            perror("semop:wait_0");    
            exit(EXIT_FAILURE);
        };

        /* printf("Child: значение семафора = %d\n", semctl(sem, 0, GETVAL, arg)); */
        
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
        semop(sem, &close_res, 1); //закрытие семафора
    };  
        close(fd[1]);
        exit(EXIT_SUCCESS);

    default: // Parent proccess

        close(fd[1]); //закрытие канала на запись

        for(int i = 0; i <= count - 1; i++){        
        sleep(1);
        if(read(fd[0], &number, sizeof(number)) == 0){      //чтение числа из канала
                printf("Ошибка чтения в файл\n");
                close(fd[0]);
                return 1;
        };
        printf("Parrent: Я прочитал из канала  %d\n", number);
        
        if((value = semctl(sem, 0, GETVAL, arg)) != 1){
            arg.val = 1;
            semctl(sem, 0, SETVAL, arg);    //закрытие семафора (принудитльеная установка значения 1 (arg.val = 1))
        };
        /* printf("Parrent: semval = %d\n", value); */

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
        close(fd_write);    //закрытие файла

        semop(sem, &open_res, 1);   //открытие семафора (перевеодение значения в 0)

        /* printf("Parrent: semval = %d\n", semctl(sem, 0, GETVAL, arg)); */

        printf("Parrent: Доступ открыт\n");
        sleep(2);
        };

        wait(NULL);
        semctl(sem, 0, IPC_RMID);   //удаление семафора
        printf("Семафор удален\n");
        exit(EXIT_SUCCESS);
    }
}