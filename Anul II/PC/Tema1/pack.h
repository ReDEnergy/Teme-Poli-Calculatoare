#define HOST "127.0.0.1"
#define PAYLOAD 1394		/** Dimensiune payload mesaj **/
#define CRC_PAYLOAD 1406	/** Dimensiune payload pentru CRC **/

typedef unsigned short int word;

/**	Structura necesara pentru transiterea unui cadru cu verificare de erori
 **/

typedef struct {
    int type;					/**  Tipul mesajului **/
    int len;					/**  Lungimea mesajului **/
    int no;						/**  Numarul de cadru al mesajului **/
    char payload[PAYLOAD];		/**  Mesajului pr-zis **/
    unsigned short int crc;		/**  CRC cadru pentru verificare **/
} Pack;


/**	Structura necesara pentru calcularea CRC a  unui cadru
 **/

typedef struct {
	char payload[CRC_PAYLOAD];	/** Dimensiunea pe care se va calcula CRC **/
    unsigned short int crc;		/** CRC cadru**/
} CRCPack;


/**	Structura necesara pentru informatii despre fisier si parametri
 **/

typedef struct {
	unsigned int window;	/** Dimensiune fereastra **/
	unsigned int seq;		/** Dimensiune numar secventa **/
	unsigned int speed;		/** Speed **/
	unsigned int delay;		/** Delay **/
	unsigned int top;		/** Varful ferestrei(Buffer) **/
	unsigned int number;	/** Numarul real de cadru al unui pachet **/
	unsigned int toRead;	/** Bytes cititi la ultimima citire din fisier **/
	unsigned int toSend;	/** Numarul de cadre ce mai trebuie trimise **/
	unsigned int fileSize;	/** Dimensiunea fisierului **/
} Stats;


/**	Functii necesare
 **/
void ComputeCRC (Pack *X, word *tabel);
void InfoPack (Pack *X);
void InfoStats (Stats *X);


/** Declarare variablile globale atat pentru send cat si pentru receiver
 **/
Stats STS;			/** Statistici fisier transmis **/
FILE *out;			/** Fisier debug **/
word *tabel, crc;	/** Pentru calculare CRC **/
Pack *MSG;			/** Buffer fereastra **/
Pack *RECV;			/** Pentru receptionarea unui mesaj **/
int FID;			/** Fisier de sursa/destinatie **/

