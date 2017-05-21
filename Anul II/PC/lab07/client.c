#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

FILE *out;

int main () {

	int Ssocket = socket (AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in myaddr, clientaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(4000);
	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
  int	nfd = connect(Ssocket, (struct sockaddr*) &myaddr, sizeof(struct sockaddr));
  if (nfd == -1) {
 		printf("Fatal Error, Please Shut Down Your Computer\n");
  	return 1;
  }
  
	char buff[1000];

	out = fopen ("in","r");
	fgets(buff, 200, out);
	int l = strlen(buff)+1;
	int rsize = send(Ssocket, buff, l, 0);
	
	close(Ssocket);
	fclose(out);
	return 0;
}
