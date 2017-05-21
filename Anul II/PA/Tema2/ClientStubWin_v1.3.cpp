/*
Client connection stub inspired from: http://www.tidytutorials.com/2009/12/c-winsock-example-using-client-server.html
*/

#include <winsock2.h>
#include <windows.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int main(int argv, char** argc){
	if (argv < 5) {
		printf("Usage: ./client server_hostname server_port opponent_level(1=dumb, 5, 7, 8) own_level(1=dumb, 5, 7, 8)\n");
		return 1;
	}
    //Datele pentru conexiune
    char* host_name = argc[1];
	int host_port = atoi(argc[2]);
	char c;

    //Initializare socket (WINDOWS ONLY!)
    unsigned short wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup( wVersionRequested, &wsaData );
    if (err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2)) {
        printf("Could not find useable sock dll %d\n",WSAGetLastError());
		return 1;
    }

    //Creare socket
    int hsock;
    int *p_int ;
    hsock = socket(AF_INET, SOCK_STREAM, 0);
    if (hsock == -1){
        printf("Error initializing socket %d\n",WSAGetLastError());
		return 1;
    }

    p_int = (int*)malloc(sizeof(int));
    *p_int = 1;
    if ((setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
			(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 )){
        printf("Error setting options %d\n", WSAGetLastError());
        free(p_int);
		return 1;
    }
    free(p_int);

    //Conectare la server
    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(host_port);
    memset(&(my_addr.sin_zero), 0, 8);
    my_addr.sin_addr.s_addr = inet_addr(host_name);
    if (connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR){
        printf("Error connecting socket %d\n", WSAGetLastError());
		closesocket(hsock);
		return 1;
    }

    //Trimitem primul mesaj - dificultatea adversarului
    char buffer[1024];
    int buffer_len = 1024;
    int bytecount;
    memset(buffer, '\0', buffer_len);
	buffer[0] = 1;
	buffer[1] = atoi(argc[3]);
	// daca nu pui enter la final, sunt sanse mari ca mesajul sa nu fie trimis cand vrei tu
	buffer[2] = '\r';
	buffer[3] = '\n';
	printf("%c\n",buffer[0]);
	// trimit primul mesaj - dificulatea adversarului
    if( (bytecount=send(hsock, buffer, strlen(buffer), 0))==SOCKET_ERROR){
        printf("Error sending data %d\n", WSAGetLastError());
		closesocket(hsock);
		return 1;
    }

	// primesc raspuns cu culoarea
    if((bytecount = recv(hsock, buffer, buffer_len, 0))==SOCKET_ERROR){
        printf("Error receiving data %d\n", WSAGetLastError());
		closesocket(hsock);
		return 1;
    }
	if (bytecount < 1 || buffer[0] > 1){
		// mesaj invalid; eroare!
	}else if (buffer[1] == 0){
		// sunt jucatorul alb
		printf("Sunt alb!\n");
	}else if (buffer[1] == 1){
		// sunt jucatorul negru
		printf("Sunt negru!\n");
	}
	while (true) {
		if((bytecount = recv(hsock, buffer, buffer_len, 0))==SOCKET_ERROR){
			printf("Error receiving data %d\n", WSAGetLastError());
			closesocket(hsock);
			return 1;
		}
		// Primesti un mesaj de la server care contine miscarile adversarului si zarul tau
		// Daca sunteti primii la mutare, atunci nu primiti mutari din partea adversarului
		// Daca adversarul nu a putut sa mute, atunci nu primiti mutari din partea adversarului

		// la un moment dat jocul se va termina
        int gameOver = 1;
        if (gameOver){
			break;
        }

        // Compune mesajul tau; dimensiunea lui depinde de ceea ce vei trimite
        char yourResponse[1024];
		buffer[0] = 1;				// depinde ce mesaj compui; dimensiunea lui poate fi mai mare
		yourResponse[1] = '\r';
		yourResponse[2] = '\n';
		if( (bytecount=send(hsock, yourResponse, strlen(yourResponse), 0))==SOCKET_ERROR){
			printf("Error sending data %d\n", WSAGetLastError());
			closesocket(hsock);
			return 1;
		}
	}

	// inchidere socket
    closesocket(hsock);

	scanf("%c", &c);
}
