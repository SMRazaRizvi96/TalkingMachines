#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
This is the G process of the system and it serves as the server side on the Socket.
This process receives the Token from the P process using a Socket,
and writes it into the server Fifo.
First the server creates and binds a socket with the P Process and then starts to receive the Tokens.
*/

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

struct Token {
	double  data;
	time_t t;
};


int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;

	struct Token token;

	char * serverfifo = "/tmp/serverfifo";
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
	 error("ERROR, no port provided\n");
	 exit(1);
	}

	// Creating the socket.
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// Binding the socket to the given IP address and Port
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
	      sizeof(serv_addr)) < 0) 
	      error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, 
		 (struct sockaddr *) &cli_addr, 
		 &clilen);
	if (newsockfd < 0) 
	  error("ERROR on accept");


	// Opening the server Fifo in Write only mode
	int fd = open(serverfifo, O_WRONLY);
	if (fd == 0) {
	perror("Cannot open fifo");
	exit(1);
	}

	while(1)
	{
		// Reading the Token from the Socket
		n = read(newsockfd,&token,256);
	     	if (n < 0) error("ERROR reading from socket");
	
		printf("Token Received From Client: %f\n", token.data);

		// Writnig the received Token in the server Fifo
		int nb = write(fd, &token, sizeof(token));
		if (nb == 0){
		perror("Write error\n");
		}
		printf("Token Sent in Server Fifo: %f\n", token.data);
	}
	return 0; 
}
