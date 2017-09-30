#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "char_driver"
#define CLASS_NAME "cdriver"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s3601725");
MODULE_DESCRIPTION("A custom character driver");
MODULE_VERSION("0.1");

typedef struct virtual_device {
	//Local buffer of the device
	char data[256];
	//The device's number, this is determined by the kernel
	int majorNumber;
	//The size of the currently stored data
	int msgSize;
	//Stores the file operations associated with this device
	struct file_operations* fops;
	//This has the effect of allowig other processes to use the device now
	struct semaphore sem;
}Device;

static Device chrDevice;

static int __init cdriver_init(void) {
	printk(KERN_INFO "Initializing cdriver\n");

	//Register the character device and return it's major number
	chrDevice.majorNumber = register_chrdev(0, DEVICE_NAME, chrDevice.fops);

	//If the initialization failed
	if(chrDevice.majorNumber < 0) {
		//Print failure in the kernel log
		printk(KERN_ALERT "cdriver initialization could not register a major number\n");

		//Return the major number for error checking purposes
		return chrDevice.majorNumber;
	}
	
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_INFO "Kernel Module Unloaded\n");
}

static int dev_open(struct inode* inodep, struct file* filep) {
	//TODO: Implement
	return 0;
}

static ssize_t dev_read(struct file* filep, char* buffer, size_t len, loff_t* offset) {
	//TODO: Implement
	return 0;
}

static int dev_release(struct inode* inodep, struct file* filep) {
	//TODO: Implement
	return 0;
}


module_init(cdriver_init);
module_exit(hello_exit);
