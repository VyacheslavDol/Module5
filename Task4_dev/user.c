#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#define BUF_LEN 80
#define FILE_NAME "/dev/chardev"

int main(int argc, char *argv[])
{   
    int p;
    assert(argc > 1);
    char buff[BUF_LEN];
    char i = 0;
    memset(buff, 0, BUF_LEN);
    printf("Input %s\n", argv[1]);

    int fp = open(FILE_NAME, O_RDWR);   /* открытие файла */
    if(fp == -1){
        perror("open:");
        return 1;
    }

    if(write(fp, argv[1], strlen(argv[1])) == -1)   /* запись в файл */
        {
            perror("write");
            return 1;
        };

    while((p = read(fp, buff, sizeof(buff))) > 0);  /* чтение из файла */
    buff[strlen(buff)] = '\0';
    printf("Reverse from chardev: %s\n", buff);
    close(fp);
    return 0;
}