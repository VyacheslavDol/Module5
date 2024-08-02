//task 01
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "square.h"

int main(int argc, char *argv[])
{
  float side;
  pid_t pid;
  switch(pid=fork()) {
  case -1:
          perror("fork"); /* произошла ошибка */
          exit(1); /*выход из родительского процесса*/
  case 0:  //child process
          for(int i = 1; i <= (argc / 2); i++){
          side = atof(argv[i]);  
          printf("Child: Площадь квадрата со стороной %.3f равна %.3f\n", side, square(side));
          };
          break;
  default: //parent process
          for(int i = (argc / 2 + 1); i < argc; i++){
          side = atof(argv[i]);  
          printf("Parent: Площадь квадрата со стороной %.3f равна %.3f\n", side, square(side));
          };
          break;
  }
  return 0;
}