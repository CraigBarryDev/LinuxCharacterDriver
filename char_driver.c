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
	//The device-driver class struct pointer
	struct class* devClass;
	//The device-driver device struct pointer
	struct device* devPtr;
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

	//Create the device class
	chrDevice.devClass = class_create(THIS_MODULE, CLASS_NAME);
	//If there is an error creating the class
	if(IS_ERR(chrDevice.devClass)) {
		//Unregister the device
		unregister_chrdev(chrDevice.majorNumber, DEVICE_NAME);
		//Print the error to the kernel log
		printk(KERN_ALERT "Failed to register cdriver class\n");
		//Return the error code
		return PTR_ERR(chrDevice.devClass);
	}

	//Register the device driver
	chrDevice.devPtr = device_create(chrDevice.devClass, NULL, MKDEV(chrDevice.majorNumber, 0), NULL, DEVICE_NAME);
	//If there is an error creating the device
	if(IS_ERR(chrDevice.devPtr)) {
		//Destroy the class
		class_destroy(chrDevice.devClass);
		//Unregister the device
		unregister_chrdev(chrDevice.majorNumber, DEVICE_NAME);
		//Print the error to the kernel log
		printk(KERN_ALERT "Failed to create the cdriver device\n");
		//Return the error code
		return PTR_ERR(chrDevice.devPtr);
	}

	//Print success for driver creation
	printk(KERN_INFO "cdriver successfully initialized\n");
	
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
	

	int errCount = copy_to_user(buffer, chrDevice.data, chrDevice.msgSize);
	return 0;
}

static int dev_release(struct inode* inodep, struct file* filep) {
	printk(KERN_INFO "Releasing cdriver\n");
	return 0;
}


module_init(cdriver_init);
module_exit(hello_exit);
