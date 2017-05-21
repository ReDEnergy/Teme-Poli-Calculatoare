#include <stdio.h>
#include "pack.h"
#include "crc.h"

CRCPack *CRC;
FILE *out;

/**	Functie pentru a calcula CRC-ul unui cadru
 **/
void ComputeCRC (Pack *X, word *tabel) {
	int i;
	CRC = (CRCPack*) X;
	CRC->crc = 0;
	for (i=0;i<CRC_PAYLOAD;i++)
		crctabel(CRC->payload[i], &CRC->crc, tabel);
}

/** DEBUG: Afisare informatii despre un cadru
 **/
void InfoPack (Pack *X) {
	fprintf(out, "TYP: %d\t", X->type);
	fprintf(out, "NR : %d\t", X->no);
	fprintf(out, "LEN: %d\t", X->len);
//	fprintf(out, "PAY: %s\t", X->payload);
	fprintf(out, "CRC: %d\t", X->crc);
	fprintf(out, "\n");
	fflush(out);
}

/** DEBUG: Afisare statistici
 **/
void InfoStats (Stats *X) {
	fprintf(out, "Stats:\t");
	fprintf(out, "WIN: %d\t", X->window);
	fprintf(out, "SEQ: %d\t", X->seq);
//	fprintf(out, "SPD: %d\t", X->speed);
//	fprintf(out, "DLY: %d\t", X->delay);
	fprintf(out, "TOP: %d\t", X->top);
	fprintf(out, "NUMBER:  %d\t", X->number);
	fprintf(out, "RD: %d\t", X->toRead/1000);
	fprintf(out, "SD: %d\t", X->toSend);
	fprintf(out, "\n");
}
