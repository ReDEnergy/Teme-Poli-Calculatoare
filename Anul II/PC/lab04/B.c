#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

typedef struct cardu {
	char m;
	char n;
} Cad, *pCad;


int main()
{

	int R,W;
	Cad C;
	char M[100];
	
	W = open ("ack", O_WRONLY);
	R = open ("data", O_RDONLY);

	srand ( time(NULL) );

	while (read(R, &C, sizeof(Cad)) ) {
		printf("%c",C.m);
		M[C.n]=C.m;
		sleep(rand() % 2);
		write(W, &C.n, 1);
	}
	fflush(stdout);
	printf("\n %s",M);

	close(R);
	close(W);
	return 0;
}

