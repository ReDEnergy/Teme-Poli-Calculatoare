#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

//NU AVETI VOIE SA MODIFICATI acest fisier!

// Macro de verificare a erorilor

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(EXIT_FAILURE);				\
		}							\
	} while(0)


#define BUFSIZE 1024
#define KIDS	10
#define FALSE 0
#define TRUE 1
#define LINE_SIZE 100


typedef struct {
	int type;
	int sender;
	int next_hop;
	int timp;
	int join;
	int creator;   //id-ul ruterului care a creat mesajul
	int nr_secv;  //id unic al mesajului in cadrul ruterului creator
	int len;
	char payload[1400];
} msg;




