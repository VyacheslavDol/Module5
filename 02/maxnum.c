//максимальное число из четырех чисел
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    float max, numb[argc];
    max = atof(argv[1]);
    for(int i = 2; i <= argc - 1; i++){
        numb[i] = atof(argv[i]);
        if(numb[i] > max){
            max = numb[i];
        }
    }
    printf("Максималоьное число: %.2f\n", max);
    exit(EXIT_SUCCESS);
}