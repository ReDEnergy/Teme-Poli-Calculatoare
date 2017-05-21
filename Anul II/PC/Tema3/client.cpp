#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFLEN 256

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
//    struct hostent *server;

    char buffer[BUFLEN];

	if (argc < 4)
	{
		fprintf(stderr,"Usage %s client_name server_address server_port\n", argv[0]);
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error((char*)"ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	inet_aton(argv[2], &serv_addr.sin_addr);


	if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
		error((char*)"ERROR connecting");

	while(1)
	{
		//citesc de la tastatura
		memset(buffer, 0 , BUFLEN);
		fgets(buffer, BUFLEN-1, stdin);

		//trimit mesaj la server
		n = send(sockfd,buffer,strlen(buffer), 0);
		if (n < 0)
			error((char*)"ERROR writing to socket");

	}
	return 0;
}


