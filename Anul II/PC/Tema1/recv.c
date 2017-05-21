#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"
#include "pack.h"
#include "crc.h"

#define PORT 10001
int BUFF[2000];

/**************************************
 **		Get FileName and fileSize
 **************************************/

/**	Primire informatii fisier si dimensiune cadru
 **/
void GetFileInfo() {
	char fn[2000];
	msg *r = NULL;
	int window;
	while (1) {
		while ((r = receive_message()) == NULL);

		/**	Calculeaza CRC informatii fisier **/
		RECV = (Pack*) r;
		crc = RECV->crc;
		ComputeCRC(RECV, tabel);
		if (crc != RECV->crc)
			continue;

		/** Salveaza informatiile necesare **/
		window = RECV->type;
		STS.fileSize = RECV->no;

		/** Trimite dimensiunea ferestrei la sender - TASK D **/
		RECV->len = STS.window;
		RECV->type = 1;
		send_message((msg*)RECV);

		/** Seteaza dimensiunea ferestrei receiverului **/
		if (STS.window == 0)
			STS.window = window;
		STS.seq = 2 * STS.window;

		/** Deschide fisierul de scriere **/
		sprintf(fn,"recv_%s", RECV->payload);
		FID = open(fn, O_WRONLY|O_CREAT,0644);
		if (FID<0)
			perror("Failed to open file\n");
		return;
	}
	return;
}


/****************************
 **		Receive Messages
 ****************************/

/**	Trimite ACK sau NACK
 **	Type: 3  - ACK
 ** Type: 4  - NACK
 **/
void SendAck (int type, char *why) {
	RECV->type = type;
	send_message((msg*)RECV);
}

/**	Scrie Cadrul X in fisier
 **/
void WriteToFile(Pack *X) {
	write(FID, X->payload, X->len);
	STS.fileSize -= X->len;
	STS.top = (STS.top+1) % STS.seq;
}

/**	Scrie toate cadrele consecutive incepand cu TOP daca au fost acceptate
 ** in buffer precedent cadrului TOP
 **/
void WriteFromBuffer() {
	while(BUFF[STS.top]) {
		BUFF[STS.top] = 0;
		WriteToFile(&MSG[STS.top % STS.window]);
	}
}

/**	Adauga un cadru in buffer in cazul in care nu este deja adaugat
 **	Se trimte - ACK indiferent daca cadrul este deja adaugat sau nu pentru a
 ** nu mai fi trimis inca o data de catre sender
 **/
void AddToBuffer() {
	if (BUFF[RECV->no] == 0) {
		BUFF[RECV->no] = 1;
		memcpy(&MSG[RECV->no % STS.window], RECV, sizeof(Pack));
		SendAck(3, "Added in buffer");
	}
	else
		SendAck(3, "Found in buffer");
}

/**	Verifica daca cadrul primit poate fi adaugat in buffer sau nu
 ** Cadrul poate fi adaugat doar daca este la o dimensiunea de maxim 10 cadre
 ** de ultimul cadru nereceptionat, altfel cadrul este respins
 **/
void ScanBuffer() {
	int diff = (RECV->no - STS.top);
	if (diff > (int)STS.window) {
		SendAck(4, "Not in sequence");
		return;
	}
	if (diff > 0) {
		AddToBuffer();
		return;
	}
	diff = abs(diff);
	if (diff >= (int)STS.window) {
		AddToBuffer();
		return;
	}
	if (diff < (int)STS.window) {
		SendAck(4, "Not in sequence");
		return;
	}
}

/**	DEBUG:	Afiseaza informatii legate de acoperirea bufferului
 **/
void PrintBuffer() {
	int i;
	for (i=0; i<STS.seq;i++)
		fprintf(out,"%d",BUFF[i]);
	fprintf(out,"\n");
	fflush(out);
}

/**	Receptioneaza continutul unui fisier si il scrie la destinatie
 **/
void GetFileContent () {
	int crc;
	msg *r;
	MSG = (Pack*) calloc(STS.window+1, sizeof(Pack));

	/** Se receptioneaza mesaje pana cand dimensiunea fisierului corespunde **/
	while (STS.fileSize) {
		while ((r = receive_message()) == NULL);

		/** Receptionare pachet si verificare validitate **/
		RECV = (Pack*)r;
		crc = RECV->crc;
		ComputeCRC(RECV, tabel);

		/** Trimit NAK daca pachetul este corupt **/
		if (RECV->crc != crc) {
			SendAck(4, "Message Corrupt");
			continue;
		}

		/** Daca cadrul acceptat corect este urmatorul din secventa se scrie in
		 ** fisier, iar apoi se verifica bufferului pentru eventuale cadre
		 ** consecutive, ce au fost acceptate precedent
		 **/
		if (STS.top == RECV->no) {
			SendAck(3, "In sequence");
			WriteToFile(RECV);
			WriteFromBuffer();
		}

		/** In cazul in care nu corespunde cadrului acceptat se incearca
		 ** adaugarea lui in Buffer **/
		else
			ScanBuffer();
	}
	free(MSG);
	free(r);
}



int main(int argc,char** argv){
	/** Initializare server **/
	init(HOST,PORT);

//	out = fopen("outrecv","w");

	/** Tabelul necesar pentru calcularea CRC **/
	tabel = tabelcrc(CRCCCITT);

	/** Initializare statistici la 0 **/
	memset(&STS, 0 , sizeof(Stats));

	/** Citire informatii apel **/
	sscanf(argv[1], "window=%d", &STS.window);

	/** Primire nume fisier **/
	GetFileInfo();

	/** Primire continut fisier **/
	GetFileContent();

	/** Inchidere fisiere **/
//	fclose(out);
	close(FID);
	return 0;
}
