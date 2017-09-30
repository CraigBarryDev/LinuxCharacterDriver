obj-m += char_driver.o

KERNEL_SOURCE:=/lib/modules/$(shell uname -r)/build
DRIVER:=char_driver
DRIVER_TESTER:=driver_tester
TESTER_BIN:=tester

all:
	make -C $(KERNEL_SOURCE) M=$(PWD) modules
	make unload
	make load
	make driver_test

driver_test:
	gcc -c $(DRIVER_TESTER).c -o $(DRIVER_TESTER).o
	gcc $(DRIVER_TESTER).o -o $(TESTER_BIN)

load:
	sudo insmod $(DRIVER).ko
	sudo chmod 777 /dev/$(DRIVER)

unload:
	sudo rmmod $(DRIVER).ko

clean:
	make -C $(KERNEL_SOURCE) M=$(PWD) clean
	rm -rf $(DRIVER_TESTER).o $(TESTER_BIN)
