#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_LEN 256

int main() {
	char sendMsg[BUFFER_LEN];
	char receiveMsg[BUFFER_LEN];

	printf("Device is about to be openned\n");

	//Attempt to open the device
	int device = open("/dev/char_driver", O_RDWR);

	printf("Device openning finished\n");

	if(!device) {
		printf("Device could not be openned\n");
		return -1;
	}

	printf("Type a string to write to the driver: \n");
	scanf("%[^\n]%*c", sendMsg);
	
	//Send the message to the device driver
	write(device, sendMsg, strlen(sendMsg));

	//Read the message back from the device driver
	read(device, receiveMsg, BUFFER_LEN);

	printf("Received Message back from device '%s'\n", receiveMsg);
	return 0;
}
