#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "alarm.h"
#define BUFF 7

typedef struct cardu {
	char m;
	char n;
} Cad, *pCad;


Cad C[BUFF];
int last = 0, ok;
char *message = "Stuff this in your pipe and smoke it!\n";
int R, W;

int readcadre() {
	int i, nr=0;
	int len = strlen(message);
	for (i=0; i<BUFF; i++)
		if (last+i < len) {
			C[i].m = message[last+i];
			C[i].n = last+i;
			nr++;
		}
	return nr;
}

void send(nr) {
	int i;
	printf("Sent:\n");
	for (i=0; i<nr; i++) {
		printf("%d ",C[i].n);
		write(W, &C[i], sizeof(Cad));
	}
	fflush(stdout);
}

void wait(nr) {
	int i,j;

	set_alarm(3);
	
	ok=1;
	printf("\nReceived:\n");

	for (i=0; i<nr,ok; i++) {
		read(R, &last, 1);
		printf("%d ", last);
	}

	fflush(stdout);
	cancel_alarm();

	printf("\nDropped : %d ",nr-i+1);
	fflush(stdout);
	int drop;
	while(i<=nr) {
		read(R, &drop, 1);
		i++;
	}
	last++;
}

int main()
{

	int len = strlen(message);
	int i=0, nr=1;

	R = open ("ack", O_RDONLY);
	W = open ("data", O_WRONLY);

	init_alarm();

	while (nr) {
		nr = readcadre();
		send(nr);
		wait(nr);
		printf("\n");
	}

	cleanup_alarm();

	close(R);
	close(W);
	return 0;
}

