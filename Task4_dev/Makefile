obj-m += chardev.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc user.c -o user

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f user