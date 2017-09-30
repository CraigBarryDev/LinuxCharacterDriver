obj-m += char_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo rmmod char_driver.ko
	sudo insmod char_driver.ko
	gcc -c driver_tester.c -o driver_tester.o
	gcc driver_tester.o -o tester

load:
	sudo insmod char_driver.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf driver_teser.o tester
