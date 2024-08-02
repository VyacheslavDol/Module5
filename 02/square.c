//квадрат числа
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    float a;
    a = atof(argv[1]);
    printf("Квадрат %.3f равен %.3f\n", a, a * a);
    exit(EXIT_SUCCESS);
}