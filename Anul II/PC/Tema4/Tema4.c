#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>

#pragma pack(push, 1)

#include "dns_header.h"
#define BUFFDIM 1000

// JUMP POINTER - byte	1100 0000
#define POINTER 192


FILE *in, *out;

/** Variabile de header si raspuns pentru intergare respectiv cereri
 **/
dns_header_t querry;
dns_header_t header;
dns_rr_t answer;

/**	Bufferul de trimitere si receptionare mesaje
 **/
char send_buff[BUFFDIM];
char recv_buff[BUFFDIM];
char *ptr_buff = send_buff;		// pointer la bufferul de trimitere

/** Socket file descriptors **/
int fdsock, nrsock;
int indx;

/**	Seteaza header pentru interogare DNS
 **/
void setQuerryHeader() {
	memset(&querry, 0 , sizeof(querry));

	querry.id = 1;
	querry.rd = 1;
	querry.qdcount = htons(1);

	/** Copiaza headerul in buffer **/
	memcpy(send_buff, &querry, sizeof(querry));
	ptr_buff += sizeof(querry);
}

/** Returneaza QTYPE
 **/
short int getQTYPE(const char *type) {
	if (strcmp(type, "A") == 0)
		return htons(A);

	if (strcmp(type, "NS") == 0)
		return htons(NS);

	if (strcmp(type, "MX") == 0)
		return htons(NS);

	if (strcmp(type, "PTR") == 0)
		return htons(PTR);

	if (strcmp(type, "CNAME") == 0)
		return htons(CNAME);

	return htons(1);
}

/** Seteaza campul QUESTION din mesaj
 **/
void createQuestion(char *name, const char *type) {
	int i, k=-1, len;
	unsigned int addr;
	char QNAME[100];
	char host[100];
	short int QTYPE, QCLASS;

	memcpy(host, name, strlen(name)+1);

	/** Save the QNAME **/
	if (strcmp(type, "PTR") == 0) {
		//	Conversie de la network order la host order utilizand ntohl
		struct in_addr temp;
		inet_aton(host, &temp);
		addr = ntohl(*(unsigned int*)&temp);
		temp = *(struct in_addr*)&addr;

		sprintf(host,"%s.in-addr.arpa", inet_ntoa(temp));
	}

	len = strlen(host);

	/** Construieste addresa conform cerintei pentru QNAME **/

	QNAME[0]='.';
	QNAME[len+1] = '\0';
	memcpy(QNAME + 1, host, len);

	len++;
	for (i=len; i>=0; i--) {
		if (QNAME[i] == '.') {
			QNAME[i] = k;
			k = -1;
		}
		k++;
	}

//	printf("QNAME : %s  => size %d\n", QNAME, len);
	memcpy(ptr_buff, QNAME, len + 1);
	ptr_buff += len + 1;

	/**	SET Question TYPE **/
//	printf("TYPE: %s\n", type);
	QTYPE = getQTYPE(type);
	memcpy(ptr_buff, &QTYPE, 2);
	ptr_buff += 2;
//	printf("QTYPE : %d\n", QTYPE);

	/**	SET Question CLASS **/
	QCLASS = htons(1);
	memcpy(ptr_buff, &QCLASS, 2);
	ptr_buff+= 2;
//	printf("QCLASS : %d\n", QCLASS);
}

/** Returneaza labelul ce incepe la pozitia indx din buffer
 **/
char* getLabel () {
	int i, k = 0;
	int pos = indx;
	char *label = calloc(100, 1);
	unsigned char p;			// pentru a verifica daca este sau nu pointer
	unsigned short pointer;		// pentru offset pointer
	unsigned short mask = (1<<14) - 1;	// masca pentru obtinerea pointerului

	// Cat timp nu e sfarsit de string
	while (recv_buff[pos])
	{
		// Pointer = 11xxxxx >= 192
		p = recv_buff[pos];
		if (p >= POINTER)
		{
			memcpy(&pointer, recv_buff+pos, 2);
			pointer = ntohs(pointer);
			pointer = pointer & mask;
			if (pos+2 > indx)
				indx = pos+2;

			// Jump la pozitia indicata de pointer
			pos = pointer;
			continue;
		}

		// In cazul in care nu este un pointer citim in continuare
		// Dimensiunea ce urmaza a fi citita
		int dim = recv_buff[pos++];

		// Pozitie de unde citesc labelul
		for (i=0; i<dim; i++)
			label[k++] = recv_buff[pos++];
		label[k++] = '.';

		if (indx < pos)
			indx = pos;
	}
	return label;
}

/** Transforma informatiile despre server din BIG in LITTLE Endian
 **/
void bigToLittle() {
	answer.type     = ntohs(answer.type);
	answer.class    = ntohs(answer.class);
	answer.ttl      = ntohs(answer.ttl);
	answer.rdlength = ntohs(answer.rdlength);
}

/** Returneaza tipul raspunsului primit
 **/
char* toType(int type) {
	if (type == NS)		return "NS";
	if (type == PTR)	return "PTR";
	if (type == CNAME)	return "CNAME";
	return;
}

void printAnswer(int records, char *section) {
	if (records)
		fprintf(out, ";; %s SECTION :\n", section);

	int i, j;
	for (i=0; i< records; i++) {

		/** Extragere label de la pozitia indexului **/
		char *label = getLabel();

		memcpy(&answer, recv_buff+indx, sizeof(dns_rr_t));
		bigToLittle();

		/** Incrementare pozitie buffer si salvare pentru revenire **/
		indx += sizeof(answer);
		int indx_back = indx + answer.rdlength;


		/** Afisare mesaj formatat tip A **/
		if (answer.type == A) {
			fprintf(out, "%s\tIN\tA\t", label);
			for (j=0; j<4; j++) {						// construieste IP
				fprintf(out, "%hhu", recv_buff[indx + j]);
				if(j<3) fprintf(out, ".");
			}
			fprintf(out, "\n");
		}

		/** Afisare mesaj formatat tip NS, CNAME, PTR **/
		if (answer.type == NS || answer.type == CNAME || answer.type == PTR) {
			char *label2 = getLabel();
			fprintf(out, "%s\tIN\t%s\t%s\n", label, toType(answer.type), label2 );
		}

		/** Afisare mesaj formatat tip MX **/
		if (answer.type == MX) {

			/** Cinteste Preference **/
			unsigned short pref;
			memcpy(&pref, recv_buff+indx, 2);
			indx += 2;
			char *label2 = getLabel();

			fprintf(out, "%s\tIN\tMX\t%d\t%s\n", label, ntohs(pref), label2);
		}

		/** Revenire poxitie buffer **/
		indx = indx_back;
	}

	fprintf(out, "\n");
}

/** Creare socket si trimiterea pachetului catre server DNS prin UDP
 **/
int sendQuerry( char *dns ) {
	struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(53);
	inet_aton(dns, &serv_addr.sin_addr);

	if ((fdsock = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		perror("ERROR opening socket");

	if (connect(fdsock,(struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		perror("ERROR connecting");

	int buff_dim = ptr_buff - send_buff;		// actualizare dimensiune buffer
	send(fdsock, &send_buff, buff_dim, 0);

	fd_set tmp_fds;

	FD_ZERO(&tmp_fds);
	FD_SET(fdsock, &tmp_fds);

	/** Timeval pentru timeout-ul conexiunii, daca nu se primeste nimic **/
	struct timeval T;
	T.tv_sec = 3;
	T.tv_usec = 0;

	if ((nrsock = select(fdsock+1, &tmp_fds, NULL, NULL, &T)) == -1)
		perror("ERROR on select!\n");

	if (!nrsock) {
		printf(" time out for: %s \n", dns);
		close(fdsock);
		return 0;
	}

	/**	Citeste mesajul primit pe socket **/
	int nr_recv = recv(fdsock, &recv_buff, BUFFDIM, 0);

	if (nr_recv <= 0) {
		printf(" timeout from %s.\n", dns);
		close(fdsock);
		return 0;
	}

	/**	Salveaza headerul mesaj primit **/
	memcpy(&header, recv_buff, sizeof(header));

	/** Verificare existenta raspuns querry **/
	if (header.rcode != 0) {
		printf("no answer from %s.\n", dns);
		return 0;
	}

//	printf("answer from %s\n", dns);

	/** Index folosit pentru parsarea mesajului **/
	indx = buff_dim;

	/** Afisarea informatiilor cerute **/
	printAnswer(ntohs(header.ancount), "ANSWER");
	printAnswer(ntohs(header.nscount), "AUTHORITY");
	printAnswer(ntohs(header.arcount), "ADDITIONAL");

	close(fdsock);
	return 1;
}

/**	MAIN FUNCTION
 **/
int main(int argc, char **argv) {
	if (argc !=3) {
		printf("Usage: ./Tema4 host type\n");
	}

	char dns[100];
	in  = fopen("dns_servers.conf", "r");
	out = fopen("logfile", "a");

	/** Seteaza cu 0 buferul de trimitere **/
	memset(send_buff, 0, sizeof(send_buff));

	/** Seteaza Querry DNS **/
	setQuerryHeader();
	createQuestion(argv[1], argv[2]);

	fprintf(out, "Trying %s\n", argv[1]);
	printf("Trying %s\n", argv[1]);

	/** Se testeaza pe rand fiecare DNS din .conf pana se primeste un raspuns **/
	while (1) {
		if(fgets(dns, 256, in) == NULL)
			return;

		if (dns[0]=='#' || dns[0]==' ' || dns[0]=='\n' || strlen(dns) < 7)
			continue;

		dns[strlen(dns)-1] = '\0';

		int ok = sendQuerry(dns);
		if (ok == 1)
			return 0;
	}

	return 0;
}

#pragma pack(pop)
