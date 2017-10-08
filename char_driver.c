#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mutex.h>
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
	//This has the effect of allowig other processes to use the device now
	struct mutex sem;
	//The device-driver class struct pointer
	struct class* devClass;
	//The device-driver device struct pointer
	struct device* devPtr;
}Device;

static int device_open(struct inode* inodep, struct file* filep);
static int device_release(struct inode* inodep, struct file* filep);
static ssize_t device_read(struct file* filep, char* buffer, size_t len, loff_t* offset);
static ssize_t device_write(struct file* fp, const char* buffer, size_t len, loff_t* offset);	

//Stores the file operations associated with this device
static struct file_operations fops =
{
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_release,
};


static Device chrDevice;

static int __init cdriver_init(void) {
	//Print that driver initialization is starting
	printk(KERN_INFO "Initializing cdriver\n");

	//Initialize the semaphore to a mutex lock
	mutex_init(&chrDevice.sem);

	//Register the character device and return it's major number
	chrDevice.majorNumber = register_chrdev(0, DEVICE_NAME, &fops);

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

	printk(KERN_INFO "Major Number %d\n", chrDevice.majorNumber);
	
	return 0;
}

static void __exit cdriver_exit(void) {
	//Destroy the mutex lock as it is no longer required
	mutex_destroy(&chrDevice.sem);

	//Cleanup allocated memory
	device_destroy(chrDevice.devClass, MKDEV(chrDevice.majorNumber, 0));
	class_unregister(chrDevice.devClass);
	class_destroy(chrDevice.devClass);
	unregister_chrdev(chrDevice.majorNumber, DEVICE_NAME);
	//Print that driver is being unloaded
	printk(KERN_INFO "Kernel Module Unloaded\n");
}

static int device_open(struct inode* inodep, struct file* filep) {
	//Lock the mutex to only allow one process to access device at one time
	mutex_lock(&chrDevice.sem);
	//Print that the driver has been openned
	printk(KERN_INFO "cdriver openned\n");
	//Return 0 on success
	return 0;
}

static ssize_t device_read(struct file* filep, char* buffer, size_t len, loff_t* offset) {
	
	//Copy read data to the buffer
	int err = !copy_to_user(buffer, chrDevice.data, chrDevice.msgSize);
	
	//If an error occured
	if(err) {
		//Report that an error has occured
		printk(KERN_INFO "Failed to send characters\n");
		//Return the fault address (negative to indicate its an error)
		return -EFAULT;
	}

	//Print that characters were sent to the user
	printk(KERN_INFO "Sent characters to user\n");
	//Reset the size of the message
	chrDevice.msgSize = 0;
	//Return 0 on success
	return 0;
}

static ssize_t device_write(struct file* fp, const char* buffer, size_t len, loff_t* offset) {
	//Store the size of the message
	chrDevice.msgSize = strlen(buffer);
	//Store the message in the buffer
	sprintf(chrDevice.data, "%s", buffer);
	//Print that a message was received
	printk(KERN_INFO "A message has been received from the user <%s>\n", buffer);
	//Return the length of the message
	return len;
}

static int device_release(struct inode* inodep, struct file* filep) {
	//Release the mutex lock to allow other processes to use the device
	mutex_unlock(&chrDevice.sem);	
	printk(KERN_INFO "Releasing cdriver\n");
	return 0;
}

//Initialize functions as module init and exit functions
module_init(cdriver_init);
module_exit(cdriver_exit);
