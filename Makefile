KERNEL_SOURCE:=/lib/modules/$(shell uname -r)/build
DRIVER_TESTER:=driver_tester
TESTER_BIN:=tester

DRIVER:=char_driver
obj-m += char_driver.o

#DRIVER:=keyboard_driver
#obj-m += keyboard_driver.o

#DRIVER:=keyboard_usb_driver
#obj-m += keyboard_usb_driver.o

all:
	make compile
	make unload
	make load
	make driver_test

compile:
	make -C $(KERNEL_SOURCE) M=$(PWD) modules

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
