#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <string>

#define MAX_CLIENTS	100
#define BUFLEN 256

using namespace std;

void error(char *msg)
{
    perror(msg);
    exit(1);
}


typedef struct {
	int socketfd;
	int ip;
	int port;
	time_t time;
	string name;
} Info;

int sockfd, newsockfd;
map <string, Info > Clients;
fd_set read_fds;	//multimea de citire folosita in select()
int fdmax;			//valoare maxima file descriptor din multimea read_fds


void addNewClient()
{
	struct sockaddr_in cli_addr;
	int client_len = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*) &client_len) == -1;

	if (newsockfd < 0) {
		perror("ERROR in accept");
		return;
	}

	// adaug noul socket intors de accept() la multimea descriptorilor de citire
	FD_SET(newsockfd, &read_fds);
	if (newsockfd > fdmax)
		fdmax = newsockfd;

	printf("New connection from %s, port %d, socket_client %d\n ", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), newsockfd);
}

int getServerPort(int socketfd)
{
	int port = 0;
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if (getsockname(socketfd, (struct sockaddr *)&sin, &len) == -1)
		perror("getsockname");
	else
		port = ntohs(sin.sin_port);

	printf("Server port number id %d\n", port);
	return port;
}

void createServer(int port)
{
	struct sockaddr_in serv_addr;

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;	// foloseste adresa IP a masinii
	serv_addr.sin_port = htons(port);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror ("ERROR opening socket for client connection");
		exit(1);
	}

	if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr)) < 0)
		perror((char*)"ERROR on binding");

	listen(sockfd, MAX_CLIENTS);

}

int main(int argc, char *argv[])
{
	char buffer[BUFLEN];
	int n, i;

	fd_set tmp_fds;		//multime folosita temporar

	if (argc < 2) {
		fprintf(stderr,"Usage : %s port\n", argv[0]);
		exit(1);
	}

	// golim multimea de descriptori de citire (read_fds) si multimea tmp_fds
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	createServer(atoi(argv[1]));

	// adaugam noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
	FD_SET(sockfd, &read_fds);
	fdmax = sockfd;

	// main loop
	while (1)
	{
		tmp_fds = read_fds;
		if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1)
			perror("ERROR in select");

		for(i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &tmp_fds))
			{
				if (i == sockfd)
				{
					printf("Server socket: %d\n",sockfd);
					// a venit ceva pe socketul de ascultare = o noua conexiune
					// actiunea serverului: accept()
					addNewClient();
					printf("Client socket: %d\n",newsockfd);
				}

				else
				{
					// am primit date pe unul din socketii cu care vorbesc cu clientii
					// actiunea serverului: recv()
					memset(buffer, 0, BUFLEN);
					if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0)
					{
						if (n == 0)
						{
							//conexiunea s-a inchis
							printf("selectserver: socket %d hung up\n", i);
						}
						else
							perror("ERROR in recv");

						close(i);
						FD_CLR(i, &read_fds); // scoatem din multimea de citire socketul pe care
					}

					else
					{
						//recv intoarce >0
						printf ("Am primit de la clientul de pe socketul %d, mesajul: %s\n", i, buffer);
					}
				}
			}
		}
	}


	close(sockfd);

	return 0;
}


