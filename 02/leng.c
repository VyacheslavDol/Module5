//самое длинное слово
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int max, j;
    j = 1;
    max = strlen(argv[1]);
    for(int i = 2; i <= argc - 1; i++){
        if(strlen(argv[i]) > max){
            max = strlen(argv[i]);
            j = i;
        }
    }
    printf("Самое длинное слово: %s\n", argv[j]);
    exit(EXIT_SUCCESS);
}