#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <time.h>


/*
This file is developed for the Log Process of the system.
It reads the character array present in the logfifo and appends
the received characters inside the log file, 'log.log'.
*/


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
	char buffer[256];
	FILE * fp;
	fp = fopen("log.log", "a");
	char * logfifo = "/tmp/logfifo";
	int logfd = open(logfifo, O_RDONLY);
	if (logfd == 0)
		perror("Cannot open Log fifo");
	
	
	while(1)
	{
		// Opening the log File
		fp = fopen("log.log", "a");

		// Opening the log Fifo in Read Only mode
		int logfd = open(logfifo, O_RDONLY);
		if (logfd == 0)
			perror("Cannot open Log fifo");
		
		bzero(buffer,256);
	

		// Reading data from the Log Fifo
		int n = read(logfd,buffer,256);
		if (n < 0) 
			error("ERROR reading from Log Fifo");

		// Appending the data read from the Fifo, inside the log file
		fputs(buffer,fp);
		printf("Characters written to the log file: %s\n",buffer);

		close(logfd);
		fclose(fp);
	}
	return 0;
}

