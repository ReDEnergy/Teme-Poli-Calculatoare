#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

FILE *out;

int main () {

	int Ssocket = socket (AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in myaddr, clientaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(4000);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	
	
	bind(Ssocket, (struct sockaddr*)&myaddr, sizeof ( struct sockaddr_in));
	listen(Ssocket, 20);
	
	int nSock;
	char buff[1000];
	int rsize = 0;
	int nfd;
	
	while ( nfd = accept(Ssocket, (struct sockaddr*) &clientaddr, &nSock) ) {
		if (nfd == -1) {
			printf("Fatal Error, Please Shut Down Your Computer");
			return 1;
		}

		rsize = recv(nfd, buff, 200, 0);
		printf("%s",buff);
	}
	close(nSock);
	return 0;
}
