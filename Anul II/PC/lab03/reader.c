#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "crc.h"

int main ()
{ 
  	int fd1, fd2, i, j, dim=256;
	char *block = (char*) malloc(256);
	word *tabel = tabelcrc(CRCCCITT);
	word acum, acum2 = 0;
	

  	fd1 = open ( "ack", O_WRONLY ); 
	fd2 = open ( "data" , O_RDONLY );
	
	while (read(fd2, block, 256) > 0 ) {
		acum2 = 0;
		read(fd2, &acum, 2);
		
		for (i=0; i<dim; i++) 
			crctabel(block[i], &acum2, tabel);
		
		printf("acum : %x \n", acum);
		printf(" acum : %x \n",acum2);
		
		if (acum == acum2)
			printf("egal\n");
		
		write(fd1, &acum2, 2);
		
	}

  	close (fd1);
	close (fd2);
}
