#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
    struct hostent *server;

    char buffer[BUFLEN];
    int len;
    if (argc < 3) {
       fprintf(stderr,"Usage %s server_address server_port\n", argv[0]);
       exit(0);
    }  
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &serv_addr.sin_addr);
    
    
    if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");    
    
    fd_set read_set;
    FD_ZERO(&read_set);
    while(1){
        FD_SET(sockfd, &read_set);
        FD_SET(STDIN_FILENO, &read_set);
        select(sockfd + 1, &read_set, NULL, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &read_set)) {
      		//citesc de la tastatura
        	memset(buffer, 0 , BUFLEN);
        	fgets(buffer, BUFLEN-1, stdin);

        	//trimit mesaj la server
        	n = send(sockfd,buffer,strlen(buffer), 0);
        	if (n < 0) 
            	 error("ERROR writing to socket");
        } else if (FD_ISSET(sockfd, &read_set)) {
            /* primit mesaj de la server */
            //printf("PRIMIT\n");
            len = recv(sockfd, buffer, BUFLEN, 0);
            if (len <= 0) {
                break;
            }
            puts(buffer);
        }

    }
    close(sockfd);
    return 0;
}


