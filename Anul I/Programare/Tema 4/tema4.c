/*
 *	Ivanica Gabriel 313 CA
 *	Tema 4 Programare
 *	19-12-2010
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
 *	Structura retine adresa ip si numele paginii ce urmeaza a fi procesate
 *	Se adauga elemente in acceasta structura doar daca DATA si TIMPUL inregistrarii
 *	sunt valide, adica mai mari decat DATA si TIMPUL de inceput
 */
typedef struct{
	char *ip;
	char *pag;
} accesari;

/*
 *	Structura top este necesara pentru a retine informatiile despre fiecare
 *	pagina din topul accesarilor: numele(*pag), si numarul accesarilor unice si totale
 */
typedef struct{
	char *pag;
	int unique;
	int total;
} top;

/*
 *	Functia adauga o noua intrare la pointerul de accesari *acc
 *	Parametrii primiti sunt:
 *		*acc	- transmis prin referinta pentru a nu se pierde adresa acestuia in urma realocarii
 *		nr		- numarul de elemente din pointerul *acc
 *		*host	- pagina web care se va adauga
 *		*adresa	- adresa ip a paginii web necesara pentru a incomi topul accesarilor
 */
void adauga(accesari **acc,int nr,char *host,char *adresa)
{
	(*acc)=(accesari*)realloc((*acc),(nr+1)*sizeof(accesari));

	(*acc)[nr].pag=(char*)malloc(strlen(host)+1);
	strcpy((*acc)[nr].pag,host);

	(*acc)[nr].ip=(char*)malloc(strlen(adresa)+1);
	strcpy((*acc)[nr].ip,adresa);
}

/*	Functia alcatuieste topul accesarilor pe baza informatiilor luate din pointerul *acc
 *	Se parcurge pointerul *acc (ordonat in prealabil dupa numele paginilor web iar apoi dupa ip)
 *	Astfel se poate alcatui topul accesarilor doar printr-o singura parcurgere.
 *	Parametrii functiei sunt:
 *		*rank	- pointer spre structura top in care se retin informatiile
 *				- transmis prin referinta pentru a nu se pierde adresa in urma realocarii
 *		*acc	- pointer pentru structura accesari, ce contine informatiile necesare
 *		k		- numarul de pagini din top accesari,transmis prin referinta
 *				- initial are valoare -1; se incrementeaza pentru fiecare adresa diferita din *acc
 *		nr		- numarul de elemente din pointerul *acc
 */
void genereaza_top(accesari *acc,top **rank,int *k,int nr)
{
	int i=0;	//	Contor necesar pentru parcurgerea pointerului *acc
	do
	{
		/*
		 *	Pentru fiecare pagina web cu denumire diferita se incrementeaza k++
		 *	Se aloca memoria necesara pentru a retine elementele structurii top
		 */
		(*k)++;
		(*rank)=(top*)realloc((*rank),(*k+1)*sizeof(top));
		(*rank)[*k].pag=malloc(strlen(acc[i].pag));
		/*
		 *	Se copiaza pagina web, si se initializeaza numarul de accesari unice si totale cu 1
		 */
		strcpy((*rank)[*k].pag,acc[i].pag);
		(*rank)[*k].unique=1;
		(*rank)[*k].total=1;
		/*
		 *	Se verifica urmatoarele adrese din pointerul *acc
		 *	Daca au acceasi adresa ca si pagina K si se compara ip-urile
		 *	Se incrementeaza numarul accesarilor in functie de compararea ip-urilor
		 */
		while(i<nr-1 && strcmp((*rank)[*k].pag,acc[i+1].pag)==0)
			{
				if(strcmp(acc[i].ip,acc[i+1].ip)!=0)
				{
					(*rank)[*k].unique++;
					(*rank)[*k].total++;
				}
				else (*rank)[*k].total++;
				/*
				 *	Se trece la verificarea urmatoarei pagini web din *acc
				 */
				if(i<nr) i++;
			}
		i++;
	}
	while(i<nr);
}
/*
 *	Functia compar1 este necesara pentru a ordona pointeru accesari **acc astfel:
 *		criteriu 1	- alfabetic dupa numele paginii
 *		criteriu 2	- crescator dupa ip
 *	Functia va fi apelata in cadrul apelarii QSORT
 */
int compar1(const void *x,const void *y)
{
	accesari *a=(accesari*)x;
	accesari *b=(accesari*)y;

	int dif;
	dif=strcmp(a->pag,b->pag);
	if (dif)
		return dif;
	return (strcmp(a->ip,b->ip));
}
/*
 *	Functia compar2 este necesara pentru a alcatui topul accesarilor
 *	Se ordoneaza astfel:
 *		criteriu 1	- dupa numarul de accesari unice
 *		criteriu 2	- dupa numarul de accesari totale
 *		criteriu 3	- alfabetic dupa numele paginii web
 *	Functia va fi apelata in cadrul apelarii QSORT pentru a ordona descrescator *top
 */
int compar2(const void *x,const void *y)
{
	top *a=(top*)x;
	top *b=(top*)y;
	int dif1, dif2;
	dif1=(a->unique)-(b->unique);
	dif2=(a->total)-(b->total);

	if(dif1>0) return -1;
	if(dif1<0) return 1;
	if(dif1==0)
	{
		if(dif2>0) return -1;
		if(dif2<0) return 1;
		if(dif2==0)
			return strcmp(a->pag,b->pag);
	}
	return 0;
}
/*
 *	Functia va afisa rezultatul cerut de problema
 *	Se citesc pozitiile din top ce v-or fi afisate
 *	Intrucat *top este ordonat descrecator se afiseaza direct informatiile
 *	de la *top[pozitie-1] (topul porneste de la nivelul 0 corespunzator pozitiei 1)
 */
void afisare(top *rank,int k,int Q)
{
	int i,poz;
	for(i=0;i<Q;i++)
	{
		scanf("%d",&poz);
		poz--;
		if(poz<=k)
			{
				printf("%s ",rank[poz].pag);
				printf("%d ",rank[poz].unique);
				printf("%d\n",rank[poz].total);
			}
		else printf("-1\n");
	}
}


int main( int argc, char *argv[] )
{
	freopen(argv[1],"r",stdin);
	freopen(argv[2],"w",stdout);

	/*
	 *	Declarere variabile
	 *	N	- numarul de adrese web citite
	 *	Q	- numarul de pozitii din topul accesarior
	 *	nr	- numarul de elemente din pointerul accesari *acc
	 *	k	- numarul de elemente din topul accesarilor
	 */
	int N,Q,i,nr=0,k=-1;
	/*
	 *	*host	- pagina web curenta citita
	 *	*adresa	- adresa ip curenta cutita
	 *	*data , *timp 	- informatiile curente despre accesare pagina
	 *	*DATA , *TIMP 	- DATA si ORA limita pentru a verifica validitatea accesarilor
	 */
	char *host,*adresa,*data,*timp,*DATA,*TIMP;
	/*
	 *	*acc	- pointer spre ACCESARI pentru a retine informatiile despre paginile valide
	 *	*rank	- pointer spre TOP necesar pentru a intocmi TOPUL accesarilor
	 */
	accesari *acc;
	top *rank;
	/*
	 *	Alocari memorie pointeri
	 */
	DATA=(char*)malloc(10);
	TIMP=(char*)malloc(5);
	data=(char*)malloc(10);
	timp=(char*)malloc(5);
	adresa=(char*)malloc(15);
	host=(char*)malloc(101);

	acc=(accesari*)malloc(1*sizeof(accesari));
	rank=(top*)malloc(1*sizeof(top));

	//	Se citeste numarul de accesari N, si numarul de pozitii Q ce vor fi afisate
	scanf("%d",&N);
	scanf("%d",&Q);

	//	Se citeste data si timpul limita
	scanf("%s",DATA);	scanf("%s",TIMP);

	/*
	 *	Se citesc pe rand informatiile despre cele N pagini web
	 *	Se verifica validitatea: data>=DATA si timp>=TIMP
	 *	Se adauga informatiile despre pagina in pointerul *acc
	 *	Se incrementeaza numarul de pagini retinute: nr++;
	 */
	for (i=0;i<N;i++)
	{
		scanf("%s",data);
		scanf("%s",timp);
		scanf("%s",adresa);
		scanf("%s",host);

		if (strcmp(data,DATA)>=0 && strcmp(timp,TIMP)>=0)
		{
			adauga(&acc,nr,host,adresa);
			nr++;
		}
	}
	/*
	 *	Se sorteaza pointerul de structuri *acc corespunzator functiei compar1
	 */
	qsort(acc,nr,sizeof(accesari),compar1);
	/*
	 *	Se alcatuieste topul accesarilor (nesortat)
	 */
	genereaza_top(acc,&rank,&k,nr);
	/*
	 *	Se sorteaza descrescator dupa cele 3 categorii topul accesarilor
	 */
	qsort(rank,k+1,sizeof(top),compar2);
	/*
	 *	Se afiseaza pozitiile cerute prin intermediul functiei afisare
	 */
	afisare(rank,k,Q);

	return 0;
}
