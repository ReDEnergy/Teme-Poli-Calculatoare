#ifndef Header
#define Header

#include <stdint.h>

#define MAX_FILENAME_SIZE 30
typedef unsigned short int 	sint;
typedef unsigned char	uchar;

/**	Structuri necesare pentru salavarea arhivei
 **/
typedef struct _FileEntryHeader {
    char file_name[MAX_FILENAME_SIZE];
    int32_t file_size;
    int32_t bytes_n;
    uint32_t checksum;
}__attribute__((__packed__))  FileEntryHeader;

typedef struct _ArchiveHeader {
    int16_t huffman_nodes_n;
    uint16_t huffman_root_node;
    int16_t file_entries_n;
}__attribute__((__packed__))  ArchiveHeader;

typedef struct _HuffmanNode {
    unsigned char value;
    int16_t left, right;
}__attribute__((__packed__))  HuffmanNode;

/** Structura este necesara pentru retinerea unei tabele de translatie pentru
 **	encoding. Se retin bitii ce trebuie scrisi in arhiva pentru fiecare cod
 **	ASCII 0-255 si numarul de biti pe care este reprezentat acesta.
 **	Bitii se retin intr-un unsigned int. Lungimea maxima a unui encoding
 ** suportat este de 32 de biti. Degenerarea arborelui Huffman poate sa fie mult
 ** mai mare de 32 de biti pentru reprezentarea unui cracter dar in practica
 ** se intampla foarte rar acest lucru si pentru fisiere de dimensi mari
 **/

typedef struct _encoding {
	unsigned int val;
	unsigned int nrbits;
} EncodeTable;

/**	Header de functii
 **/

int compress(int argc, char **argv);
void list (char *arhiva);
int extract (char **argv);

#endif
