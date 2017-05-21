//
// fisier crc.h
//
// contine directivele si definitiile utile programelor
// de calcul al codului de control ciclic CRC-CCITT
//

#define CRCCCITT 0x1021

#include <stdio.h>
#include <stdlib.h>

typedef unsigned short int word;
typedef unsigned char byte;

word calculcrc (word, word, word);
word* tabelcrc (word);
void crctabel (word data, word* acum, word* tabelcrc);
