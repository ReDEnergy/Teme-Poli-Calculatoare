#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "lib.h"
#include "pack.h"
#include "crc.h"

#define PORT 10000
#define DELEY_CONST (400 + (4*STS.window) + (20*STS.speed))


/**	Functia calculeaza dimensiunea ferestrei
 **/
void GetwindowSize () {
	STS.window = (unsigned int)(((double) STS.speed * STS.delay / 8 * (1 << 20))/(1000 * 1408) + 1 );
	if (STS.window > 1000)
		STS.window = 999;
	if (STS.window <10)
		STS.window = 10;
	STS.seq = 2 * STS.window;
}


/**********************************
 **     Send File Information
 **********************************/

/** Functia trimite informatiile despre fisier
 ** cat si dimensiunea ferestrei
 **/
void SendFileInfo(char* filename) {
	/** FileInfo **/
	Pack *FInfo = calloc(1, sizeof(Pack));
	msg* r = NULL;

	struct stat buf;
	if (stat(filename,&buf)<0) {
		perror("Stat failed");
		return;
	}

	FID = open(filename, O_RDONLY);

	if (FID<0){
		perror("Couldn't open file");
		return;
	}

	/** Initializare **/
	FInfo->type = STS.window;		/** In type retin dimensiunea ferestre **/
	FInfo->no = (int)buf.st_size;
	FInfo->len = strlen(filename);
	sprintf(FInfo->payload,"%s",filename);

	/** Calculare CRC **/
	ComputeCRC(FInfo, tabel);

	/** Bucla trimitere informatii
	 ** Atat timp cat nu primim raspunul ACK (type = 1) sau nu se primeste un
	 ** mesaj retrimitem mesajul
	 **/
 	while (1) {
		send_message(((msg*)FInfo));
		r = receive_message_timeout(2 * STS.delay + DELEY_CONST);
		if (r) {
			if (r->type == 1) {
				if (r->len) {
					STS.window = r->len;
					STS.seq = 2 * STS.window;
				}
				free(FInfo);
				free(r);
				return;
			}
		}
		free(r);
	};
}

/************************
 **     Send Messages
 ************************/

/**	Incrementarea varfului TOP din buffer - circular
 **/
void NextPack() {
	STS.top = (STS.top + 1) % STS.window;
}


/** Gasirea urmatoarei pozitii TOP valida (ce contine un cadru)
 ** Se parcurce bufferul atata timp cat nu se gaseste un pachet numerotat -1
 ** Intoarce un numar mai mare decat dimensiunea ferestrei in cazul in care
 ** bufferul este gol - pachete '-1'
 **/
int NextValidPack() {
	int valid = 0;
	while(MSG[STS.top].no < 0) {
		NextPack();
		valid++;
		if (valid > STS.window)
			return valid;
	}
	return valid;
}

/**	Citirea unui pachet in varful bufferului
 **/
int ReadPack () {
	int pos = STS.top;
	int len = read(FID, &MSG[pos].payload, PAYLOAD);
	if (len) {
		MSG[pos].type = 2;		/** Tip mesaj **/
		MSG[pos].len = len;		/** Lungime **/
		MSG[pos].no = STS.number % STS.seq;		/** Numar de secventa cadru **/
		MSG[pos].crc = 0;		/** CRC **/
		ComputeCRC(&MSG[pos],tabel);	/** Calcul CRC **/
		STS.number++;			/** Incrementare numar pachet **/
		STS.toSend++;			/** Incrementare pachete de trimis **/
		return 1;
	}
	return 0;	/** Nu se mai poate citi nici un pachet **/
}

/**	Trimitrea bufferului plin catre receiver
 **/
void SendBuffer() {
	int i;
	for (i=0; i< STS.window; i++) {
		STS.toRead = ReadPack();
		if (STS.toRead) {
			send_message((msg*)&MSG[i]);
			NextPack();
		}
		else
			break;
	}
	STS.top = 0;
}

/**	Retrimiterea tuturor pachetelor de la varful TOP al bufferului pana la
 ** pachetul receptionat cu numarul de cadru (RECV->no)
 **/
void ResedPacksBefore(int no) {
	STS.toSend--;
	if (STS.toSend)
		while (MSG[STS.top].no != no) {
			if (MSG[STS.top].no >= 0)
				send_message ( (msg*) &MSG[STS.top]);
			NextPack();
		}
}

/**	Adauga un nou pachet pe pozitia TOP
 ** Daca nu se mai poate citi - pozitia este considerata goala: numerotat -1
 **/
void FillBuffer() {
	if (STS.toRead) {
		STS.toRead = ReadPack();
		if (STS.toRead)
			send_message ( (msg*) &MSG[STS.top]);
		else
			MSG[STS.top].no = -1;
	}
	else
		MSG[STS.top].no = -1;
	NextPack();
}

/**	In cazul in care nu se primeste nici un mesaj pana la timeout se tetrimit
 ** toate cadrele valide din fereastra
 **/
void ResendOnTimeOut() {
	int i;
	perror("timeout");
	for (i=0;i<STS.window;i++) {
		if (MSG[i].no >= 0)
			send_message ( (msg*) &MSG[i]);
	}
}


/**	DEBUG:	Printare cadre din buffer - numar de cadru
 **/
void MsgStatus() {
	int i;
	for (i=0;i<STS.window;i++) {
		fprintf (out, "%d ", MSG[i].no);
	}
	fprintf(out,"\n");
	fflush(out);
}

/**	Alocare memorie pentru buffer
 **	Initial toate pachetele din cadru sunt invalide -1
 **/
void CreateBuffer() {
	int i;
	MSG = (Pack*) malloc( (STS.window+1) * sizeof(Pack));
	for (i=0;i<STS.window;i++)
		MSG[i].no = -1;
}

/**************************
 **     Send Content
 **************************/

/**	Trimiterea continutului fisierului
 **/
void SendFileContent() {
	CreateBuffer();		/** Creeare buffer **/
	SendBuffer();		/** Trimitere buffer **/

	do {
		msg* r = NULL;
		/** Se asteapta primirea unui raspuns **/
		r = receive_message_timeout(STS.delay * 2 + DELEY_CONST);
		if (r) {
			RECV = (Pack*) r;
			/** Se verifica tipul raspusului **/
			/** Type:3 - ACK **/
			if (RECV->type == 3) {
				ResedPacksBefore(RECV->no);
				FillBuffer();
				NextValidPack();
			}
			/** Type 4 - NACK **/
			if (RECV->type == 4) {
				send_message ((msg*) &MSG[STS.top]);
				NextPack();
			}

			/** Daca bufferul este gol se iese din program **/
			if (NextValidPack() > STS.window) {
				free(r);
				return;
			}
		}
		/** Daca nu se primesc mesaje se retrimite intreaga fereastra **/
		else
			ResendOnTimeOut();

		/** Eliberare memorie **/
		free(r);
	} while (1);

	/** Eliberare memorie **/
	free(MSG);
}

int main(int argc,char** argv){
	/** Initializare server **/
	init(HOST,PORT);

	if (argc<2){
		printf("Usage %s filename\n",argv[0]);
		return -1;
	}

//	out = fopen("outsend","w");

	/** Tabelul necesar pentru calcularea CRC **/
	tabel = tabelcrc(CRCCCITT);

	/** Initializare statistici la 0 **/
	memset(&STS, 0 , sizeof(Stats));


	/** Citire informatii apel **/
	sscanf(argv[1], "speed=%d", &STS.speed);
	sscanf(argv[2], "delay=%d", &STS.delay);
	if (STS.delay == 1)
		STS.delay = 2;

	/** Calcularea dimensiunii ferestrei **/
	GetwindowSize();

	/** Trimiterea informatiilor despre fisier si fereastra **/
	SendFileInfo(argv[5]);

	/** Trimiterea continutului fisierului **/
	SendFileContent(argv[5]);

	/** Inchidere fisiere **/
//	fclose(out);
	close(FID);
	return 0;
}
