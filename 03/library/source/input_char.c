#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../input_char.h"

void input_char(char* word, long size_word){            //функция корректного и удобного ввода с помощью fgets с очисткой буффера
    int c;
    memset(word, 0, size_word);
    fgets(word, size_word, stdin);
    word[strcspn(word, "\n")] = '\0';
    if(strcmp(&word[size_word-2], "\0"))
    while ((c = fgetc(stdin)) != '\n' && c != EOF);
};

int get_ans_int(){                                              //функция получения ответа в виде чисел (от 0 до 9)
    int ansi;
    char ans[2];
    input_char(ans, sizeof(ans));
    ansi = atoi(ans);
    return ansi;
}