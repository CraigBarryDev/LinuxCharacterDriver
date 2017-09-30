obj-m += char_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -c driver_tester.c -o driver_tester.o
	gcc driver_tester.o -o tester

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
