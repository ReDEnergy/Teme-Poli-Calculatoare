#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFSZ 4

char * phrase = "Stuff this in your pipe and smoke it \n";

int main ()
{
  int W, R, nr;
  char ack;
  char bit = 0;
  char buf[100];
  char *p = phrase;

  W = open ( "data", O_WRONLY );
  R = open ( "ack", O_RDONLY );

  int len = strlen(phrase);
  int packs = len / BUFFSZ;

  while (packs--) {
  	memcpy(buf, p, BUFFSZ);
  	buf[BUFFSZ] = bit;

  	nr = 0;
  	do{
  		printf(" retry nr: %d\n", nr);
  		write (W, buf, 5 );
  		read (R, &ack, 1);
  		nr++;
  	} while(ack != bit);
  	p = p + BUFFSZ;
  }

  close (W);
  close (R);
}

