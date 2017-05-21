#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

/*	depth = nr de taburi offset la afisare
 *	Cand se acceseaza un director : depth ++;
 *	Cand se iese din director (dupa afisare) depth --;
 *	arg	= suma argumentelor apelate de program
 */

int depth=0;
int arg;

/*	Structura FISA retine informatiile despre fiecare fisier in parte
 *	type este necesar pentru a testa daca fisierul este un director sau nu
 */

typedef struct {
	char	*name;
	int		UID;
	int 	GID;
	long 	size;
	long	time;
	int 	type;
} FISA;

/* 	Default run , compara numele fisierelor
 *	arg = {4, 8, 12, 16}
 *	apel : none
 */

long comparename (const void *a, const void *b) {
	FISA *x = (FISA*) a;
	FISA *y = (FISA*) b;
	return strcmp ( x->name, y->name);
}

/*	Functia de comparare pentru size
 *	arg % 4 = 2
 *	apel : -s
 */

long comparesize (const void *a, const void *b) {
	long diff;
	FISA *x = (FISA*) a;
	FISA *y = (FISA*) b;
	diff = x->size - y->size;
	if (diff)
		return diff;
	return strcmp ( x->name, y->name);
}

/*	Functia de comparare pentru ultima data a modificarii fisierului
 *	arg % 4 = 1
 *	apel : -t
 */

long comparetime (const void *a, const void *b) {
	long diff;
	FISA *x = (FISA*) a;
	FISA *y = (FISA*) b;
	diff = x->time - y->time;
	if (diff)
		return diff;
	return strcmp ( x->name, y->name);
}

/*	Comparename reverse
 *	arg = {-4, -8, -12, -16}
 *
 */

long comparename_r(const void *a, const void *b) {
	return comparename(b,a);
}

/*	Comparezise reverse
 *	arg %4 =-2
 */

long comparesize_r(const void *a, const void *b) {
	return comparesize(b,a);
}

/*	Comparetime reverse
 *	arg %4 =-1
 */

long comparetime_r(const void *a, const void *b) {
	return comparetime(b,a);
}

/*	Functia inverseaza adresele la 2 pointeri void
 */

void swap ( void **a ,void **b ) {
	void *aux;
	aux=*a;
	*a=*b;
	*b=aux;
}

/*	Functia quicksort ordoneaza un vector de pointeri void folosind
 *	functia de comparare generica COMP
 */

void quicksort (void **arr, int left, int right, long (*comp)() ) {
	int i = left;
	int j = right;
	void *m = arr[(left + right) / 2];

	do {
		while( comp( m, arr[i] ) > 0 ) i++;
		while( comp( arr[j], m ) > 0 ) j--;
		if(i <= j)
		{
			swap(&arr[i],&arr[j]);
			i++;
			j--;
		}
	} while(i <= j);

	if(left < j)	quicksort(arr, left, j, comp);
	if(i < right)	quicksort(arr, i, right, comp);
}

/*	Functia ordoneaza decide ce apel de functie este necesar pentru a ordona
 * 	vectorul de pointeri FILES.
 *	Fiecare valoare a argumentului ARG corespunde cu o metoda de sortare
 */

void ordoneaza (FISA ** files, int nr) {

	if (arg %4 ==  1)	quicksort((void**)files,0,nr-1,comparetime);
	if (arg %4 ==  2)	quicksort((void**)files,0,nr-1,comparesize);

	if (arg %4 == -1)	quicksort((void**)files,0,nr-1,comparetime_r);
	if (arg %4 == -2)	quicksort((void**)files,0,nr-1,comparesize_r);

	if (arg %4 ==  0)  {
		if (arg > 0) quicksort((void**)files,0,nr-1,comparename);
		else quicksort((void**)files,0,nr-1,comparename_r);
	}

}

/*	Functia verifica validitatea apelarii si calculeaza valoarea argumentului
 *	Apelare gresita :	return 0 si afisare USAGE
 *	Ficare apelare corect are un corespondent o valoarea a argumentului
 *	ARG = ( a*8 + R*4 + s*2 + t*1 ) * r ;
 *	ARG ia valoare negativa cand se apeleaza : -r
 *	Folosinduse multiplicatori puterile ale lui 2 consecutive, se asigura unicitatea pentru ARG
 */

int verify_input (int nr, char **arg) {

	int i,a=0,R=0,r=0,s=0,t=0;

	if(nr==1)
		return 16;

	for(i=1;i<nr;i++) {
		if(strlen(arg[i])>2)
			return 0;
		if(arg[i][0]!='-')
			return 0;

		switch (arg[i][1]) {
			case 'a': a++; break;
			case 'R': R++; break;
			case 'r': r++; break;
			case 's': s++; break;
			case 't': t++; break;
			default : return 0;
		}
		if(a>1 || R>1 || r>1 || s>1 || t>1 || (s==1 && t==1))
			return 0;
	}

	if ( r == 1 )
		if( nr == 2 ) return -16;
		else r = -1;
	else r = 1;

	return ( a*8 + R*4 + s*2 + t*1 )*r;
}

/*	Functia formateaza data primita ca valoarea int
 */

void disp_date (long time) {
	char buf[30];
	struct tm * timeinfo;

	timeinfo = localtime(&time);
	strftime (buf, 30, "%H:%M:%S, %d %B %Y", timeinfo);
	printf ("%s\n", buf);
}

/*	Fucntia principala ce citeste fisierele dintr-un director
 */
void read_files (char *directory);

/*	Functia returneaza calea absoluta a fisierului
 *	Se concatedeaza numele fisierului cu path-ul directorului curent
 */

char* fullpath (char *directory, char *filename) {
	char *path;
	int a=strlen(directory);
	int b=strlen(filename);

	path=(char*)malloc(a+b+2);
	sprintf(path,"%s/%s",directory,filename);
	return path;
}

/*	Functia afiseaza ordonat fisierele din directorul curet
 *	In cazul afisarii RECURSIVE se verifica fiecare fiser ce este afisat
 *	Daca este un DIRECTOR  ( type = 4 ) se apeleaze READ_FILES(DIRECTOR)
 */

void afiseaza (char *directory, FISA **files, int nr,  int align) {
	int i,offset;

/*	Afisare offset taburi
 */
	for(i=0;i<nr;i++) {
		offset=depth;
		while (offset) {
			printf("\t");
			offset--;
		}

/*	Afisare informatii fisier
 */
		printf("%*s\t", align, files[i]->name);
		printf("%d:", files[i]->UID);
		printf("%d\t", files[i]->GID);
		printf("%ld\t", files[i]->size);
		disp_date(files[i]->time);

/*	Verificare apelare recursiva
 *	Daca este Document se incrementeaza DEPTH;
 *	Se determina calea absoluta a fisierului
 *	Se apleaza CITIREA fisierelor din cadrul DIRECTORULUI
 */
		int R=(abs(arg))% 8;

		if( R == 4 || R==5 || R==6) {
			if (files[i]->type == 4) {
					depth++;
					char *path;
					path=fullpath(directory, files[i]->name);
					read_files(path);
				}
		}

	}
}

/*	Functia salveaza informatiile despre fisier in structura acestuia
 *	Se utilizeaza calea absoluta a fisierlui
 */

void add_info (char *directory, FISA ***files, char *file, int nr) {
	struct stat		info;
	char *path;

	path=fullpath(directory, (*files)[nr]->name);
	stat(path, &info );

	(*files)[nr]->UID = info.st_uid;
	(*files)[nr]->GID = info.st_gid;
	(*files)[nr]->size = info.st_size;
	(*files)[nr]->time = info.st_mtime;
}

/*	Functia realoca memorie pentru a salva un nou fisier in structura **FILES
 *	Se salveaza numele si tipul acestuia
 */

void add_file (FISA ***files, char *file, int nr, int type ) {
	(*files) 	= (FISA**) realloc ( *files, (nr+1) * sizeof (FISA*));
	(*files)[nr]= (FISA*) malloc ( sizeof (FISA) );

	(*files)[nr]->name = file;
	(*files)[nr]->type = type;
}

/*	Functia verifica si exclude fisierele ce nu corespund salvarii
 *	!= "." , !=".." si !"="lf" programul testat
 */

int exclude ( char *a ) {
	if ( strlen(a)==1 && a[0]=='.')
		return 0;
	if ( strcmp(a,"..") == 0 )
		return 0;
	if ( strcmp(a,"lf") == 0 )
		return 0;
	return 1;
}

/*	Functia este necesara pentru a verifica daca un fisier este hidden sau nu
 */

int hide_exclude (char *a) {
	if (strlen(a) > 1 && a[0] == '.' && ( abs(arg) < 8 || abs(arg)==16 ) )
		return 0;
	return 1;
}

/*	Functia citeste fisierele din cadrul directorului primit ca parametru
 */

void read_files (char *directory) {

/*	NR 		= numarul de fisiere din directorul curent
 *	ALIGN 	= numarul de spatii necesare pentru alinierea la dreapta
 * 	aux 	= pointerul corespunzator fisierului citit din director
 *	DIRECT	= directorul ce urmeaza a fi deschis pentru citire
 */
	int nr=0, align=0, lg;
    struct dirent	*aux;
	FISA			**files;
	DIR             *Direct;

/*	Alocare memorie pentru retinerea fisierelor
 */
	files=(FISA**) malloc ( sizeof(FISA) );

    Direct = opendir(directory);

/*	Se citesc pe rand fisierele din directorul curent si in caz de validitate
 *	se salveaza in FILES
 */
	while ( ( aux=readdir(Direct) ) )
		{
			char *fname = aux->d_name;
			if ( exclude(fname) && hide_exclude(fname))
			{
				add_file (&files, fname, nr, aux->d_type );
				add_info (directory, &files, fname, nr );

/*	Se calculeaza lungimea maxima a offsetului (a fisierului din director)
 */
				lg = strlen(files[nr]->name);
				if( lg > align ) align = lg;
				nr++;
			}
		}
/*	Daca s-au citit fisiere se apeleaza ordoarea acestora
 *	Se afiseaza valorile ordonate
 *	Se inchide directorul
 *	Se deincrementeaza adancimea de afisare DEPTH
 */
	if (nr) {
	ordoneaza(files,nr);
	afiseaza(directory, files, nr, align);
	closedir(Direct);
	depth--;
	}
}

int main (int argc , char **argv) {

/*	Se determina directorul in care se afla programul apelat
 */
	char Dir[100];
	getcwd(Dir,100);

/*	Se calculeaza valoarea argumentului
 */
 	arg=verify_input(argc,argv);

/* 	Se verifica validitatea apelarii si se incepe citirea fisierelor
*/
	if(arg) {
		read_files(Dir);
	}
/*	In caz de eroare la apelare se afiseaza USAGEUL
 */
	else {
		printf("Usage: ./lf [-a|-R|-r] <-s|-t>\n");
	}

	return 0;
}
