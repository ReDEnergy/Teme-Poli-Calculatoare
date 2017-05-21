#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
 
 
/*
struct  hostent {
	char    *h_name;	     // official name of host
	char    **h_aliases;    // alias list
	int     h_addrtype;     // host address type
	int     h_length;	     // length of address
	char    **h_addr_list;  // list of addresses from name server
};

struct hostent* gethostbyname(const char *name);
 
struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type);
*/

int main (int argc, char **argv) {
	int i=0;

	if (argc < 3) {
		printf("Usage: ./lab8 -command host");
		return 0;
	}

	struct hostent *x;

	
	if ( argv[1][1] == 'n') {
		x = gethostbyname (argv[2]);
	}
	

	if ( argv[1][1] == 'a') {
		struct in_addr A;
		inet_aton(argv[2], &A);
		x = gethostbyaddr((const void *)&A, sizeof(struct in_addr), AF_INET);
	}


	if (x == NULL) {
		printf ("Address not found!");
		return 0;
	}
	
	printf("%s\n", x->h_name);
	printf("%d\n", x->h_addrtype);
	printf("%d\n", x->h_length);

	while (x->h_aliases[i] !=NULL ) {
		printf("%s\n", x->h_aliases[i]);
		i++;
	}

	i=0;

	struct in_addr **list = (struct in_addr **)x->h_addr_list;
	while (list[i] !=NULL) {
		printf("%s\n", inet_ntoa (*list[i]));
		i++;
	}
	
	return 0;
}
