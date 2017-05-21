#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define N 658959 //image size

char buf1[N], buf2[N];

int main(int argc, char** argv){
	int fd1, fd2, bytes_read, bytes_left , diff, max_diff;
	char c1, c2;
	int i;
	char* ptr;

	if (argc != 3){
		printf("Usage: %s image1.pnm image2.pnm\n", argv[0]);
		return 0;
	}

	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_RDONLY);

	if (fd1 < 0 || fd2 < 0){
		printf("Can't open input files\n");
		return 0;
	}

	//read the first image
	ptr = buf1;
	bytes_left = N;
	while (read(fd1, ptr, bytes_left) > 0){
		ptr += bytes_read;
		bytes_left -= bytes_read;
	}

	//read the second image
	ptr = buf2;
	bytes_left = N;
	while (read(fd2, ptr, bytes_left) > 0){
		ptr += bytes_read;
		bytes_left -= bytes_read;
	}

	//compare
	max_diff = 0;
	for (i=0; i<N; i++){
		c1 = buf1[i];
		c2 = buf2[i];
		diff = ((int)c1) - ((int)c2);
		if (diff < 0)
			diff *= -1;
		if (diff > max_diff)
			max_diff = diff;
	}

	printf("max_diff = %d\n", max_diff);

	close(fd1);
	close(fd2);

	return 0;
}

