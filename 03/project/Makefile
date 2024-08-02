all: task_03

task_03: main.o
	gcc -o task_03 *.o -L../library -lmy -Wl,-rpath,../library/
	rm *.o

main.o: main.c
	gcc -c main.c

clean:
	rm -f task_03

clean_book:
	rm -f contact_book