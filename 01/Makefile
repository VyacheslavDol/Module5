all: task_01

task_01: main.o square.o
	gcc main.o square.o -o task_01

main.o: main.c
	gcc -c main.c -o main.o

square.o: square.c
	gcc -c square.c -o square.o

clean:
	rm -f *.o task_01