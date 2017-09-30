#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_LEN 256

int main() {
	char sendMsg[BUFFER_LEN];
	char receiveMsg[BUFFER_LEN];

	printf("Attempting to open device\n");

	//Attempt to open the device
	int device = open("/dev/char_driver", O_RDWR);

	if(device < 0) {
		int err = errno;
		printf("Device could not be openned <%d>\n", err);	
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
