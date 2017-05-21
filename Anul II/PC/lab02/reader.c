#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

FILE *f1;

int main ()
{
  int R, W;
  char buffer [100];
  char bit = 0;
  char randbit;

  R = open ( "data", O_RDONLY );
  W = open ( "ack", O_WRONLY );
  f1 = fopen ("out", "w");

  srand ( time(NULL) );

  while (read(R, buffer, 5) ){

  	randbit = rand() % 2;

  	if (randbit == bit) {
  		buffer[4] = '\0';
  		printf("%s", buffer);

  	}

  	write(W, &randbit, 1);
  }

  close (R);
  close (W);
}

