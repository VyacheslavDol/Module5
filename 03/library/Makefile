all: libmy.so

libmy.so: contact.o email.o input_char.o list_func.o person.o phone.o
	gcc -shared -o libmy.so *.o
	rm -f *.o

contact.o: ./source/contact.c
	gcc -c -fPIC ./source/contact.c 

email.o: ./source/email.c
	gcc -c -fPIC ./source/email.c

input_char.o: ./source/input_char.c
	gcc -c -fPIC ./source/input_char.c

list_func.o: ./source/list_func.c
	gcc -c -fPIC ./source/list_func.c

person.o: ./source/person.c
	gcc -c -fPIC ./source/person.c

phone.o: ./source/phone.c
	gcc -c -fPIC ./source/phone.c

clean:
	rm -f *.so

	
	
	
