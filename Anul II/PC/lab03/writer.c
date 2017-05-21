#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "crc.h"

FILE *f1;

int main ()
{ 
  	int fd1, fd2; 
	int i, dim = 256;
	int randbit;
	  
  	f1 = fopen ( "in", "r");  
 	fd1 = open ( "ack", O_RDONLY ); 
	fd2 = open ( "data" , O_WRONLY );

	char *block = (char*) malloc(256);
	word *tabel = tabelcrc(CRCCCITT);
	word acum = 0, acum2, nr =0, ver;
	word fail = 8;
	

	srand ( time(NULL) );

	while (fread(block, 1, dim, f1)) {
		acum = 0;
		
		for (i=0; i<dim; i++) 
			crctabel(block[i], &acum, tabel);

		nr = 0;
		do {
			ver = acum;
			randbit = rand() % 2;
			printf("Try nr %d\n", nr);

			write(fd2, block, strlen(block));
			
			if (randbit == 1)
				ver = fail;
	
			write(fd2, &ver, 2);			
			read(fd1, &acum2, 2);
			nr++;
		
		} while(ver != acum2);
		
	}


	close (f1);
	close (fd1);
	close (fd2);
}
