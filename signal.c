#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

/*
This file is developed for the 'S (Signal)' process of the system.
The process first creates a named Pipe / Fifo, '/tmp/signalfifo', and opens it.
Then the user is then asked to give a signal.

The allowed signals are:
1. start
2. stop
3. dump log

The received signal is then written on the Signal Fifo, and the cycle repeats.
*/

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{		
	int signalfd;
	char * signalfifo = "/tmp/signalfifo";


	// Making the Fifo
	if (mkfifo(signalfifo, S_IRUSR | S_IWUSR) != 0)
	{
		perror("Cannot create Signal fifo. Already existing?");
	}

	char buffer[256];

	// Opening the Fifo in Read & Write mode
	signalfd = open(signalfifo, O_RDWR);
	if (signalfd == 0) {
		perror("Cannot open fifo");
		unlink(signalfifo);
		exit(1);
	}

	while(1)
	{
		// Taking the input signal from the User
		printf("Please enter the Signal start / stop / dump log : ");
		bzero(buffer,256);
		fgets(buffer,256,stdin);
		
		// Writing the received signal inside Fifo
		int n = write(signalfd,buffer,sizeof(buffer));
		if (n < 0) 
	 		error("ERROR writing to socket");
		
		printf("Signal written to Signal Fifo: %s\n", buffer);
	}

	return 0;

}
