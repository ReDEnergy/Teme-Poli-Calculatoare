// Ivanica Gabriel 313CA	-	Tema 3

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*	Functia ord() va ordona descrescator secventele de biti in functie de numarul de aparitii al acestora
	Functia primeste 3 parametri :
	**secv 	- adresa blocului de memorie unde sunt retinute secventele de biti
	*aparitii	- vectorul de aparitii dupa care v-a fi sortat vectorul
	nr_secv	- numarul de elemente din vector/numarul de secvente de biti */	

void ord (char **secv,int *aparitii,int nr_secv)
{
	int i,g=1,aux;
	char *aux2;
	while(g){
	g=0;
	for(i=0;i<nr_secv-1;i++) 
		if(aparitii[i]<aparitii[i+1])
		{
			g=1;
			aux=aparitii[i];
			aparitii[i]=aparitii[i+1];
			aparitii[i+1]=aux;
			aux2=secv[i];
			secv[i]=secv[i+1];
			secv[i+1]=aux2;
		}
	}
}

/*	Functia verifica daca o secventa de biti *q se regaseste in tabloul de secvente **secv
	Se parcurge pe taboul de secvente si se compara secventele cu *secv[i] cu ajutorul functiei strcmp()
	Parcurgerea tabloului se face doar pentru secventele de lungime egala cu a subsirului *q pentru eficienta
	Astfel variabila first retine pozitia primei secvente de lungime strlen(q), ce a fost adaugata in tabloul **secv.
	 
	Functia returneaza pozitia i a secventei cautate in tablou pentru a incrementa numarul de aparitii al acesteia.
	In caz ca subsirul nu se gaseste se returneaza -1 */
	
int cauta(char *q, char **secv, int nr_secv, int first)
{
	int i;
	for( i = nr_secv-1; i > first ; --i)
	{	
		if(strcmp(secv[i],q)==0) 
			return i;
  	}
	return -1;
}

/*	Functia adauga un subsir nou in tabloul de secvente
	Pentru a se face alocarea de memorie necesara secventei se transmite prin parametri lungimea sirului : lung
	Pointerul de aparitii si Tabloul de secvente (*aparitii si **secv) sunt trimise prin referinta deoarece in urma realocarii de memorie
	se poate modifica adresa spre care pointeaza acestea.
	Functia aloca dinamic memoria necesara pentru memorarea secventei *subsir in tabloul **secv.
	De asemenea se aloca memorie si in vectorul de aparitii si se initializeaza cu 1.*/
	
void adauga(char *subsir, int lung, char ***secv, int **aparitii, int *nr_secv)
{
	*secv = (char**) realloc(*secv, (*nr_secv+1) * sizeof(char*));
	*aparitii = (int*) realloc(*aparitii, (*nr_secv+1) * sizeof(int));
	(*secv)[*nr_secv] = (char*) malloc(lung+1);
	(*aparitii)[*nr_secv] = 1;
	strcpy((*secv)[*nr_secv],subsir);
	++*nr_secv;
}


// Functia afiseaza rezultatul programului in modul formatat cerut de cerinta

void afisare(char **secv,int aparitii[],int nr_secv)
{
	int i;
	printf("%d secvente de %s",aparitii[0],secv[0]);
	
	for(i=1;i<nr_secv && aparitii[i]>1;i++)
	{
		if(aparitii[i]<aparitii[i-1])	printf("\n%d secvente de ",aparitii[i]);
		if(aparitii[i]==aparitii[i-1] && i<nr_secv-1) printf(",");
		printf("%s",secv[i]);
	}
}

int main ()
{
	/* 	Declarare variabile necesare
  		a,b -	reprezinta capetele de interva de lungime al secventelor
  		p	-	dimensiunea secventelor
  		i	- 	contor de parcurgere
  		n	-	lungimea secventei de biti initiala (*sir)
  		
  		nr_secv	-	aparitiiul secventelor memorate in tablou
  		aparitii-	vector necesar retinerii numarului de aparitii al secventelor 
		first	-	locatia in tablou a primei secvente avand lungimea curenta
		pos		-	retine valoarea returnata de functia cauta()	*/
		
	int a, b, p, i, n, nr_secv=0, first=-1, pos, *aparitii = NULL;
  
	/*
  		*sir	-	pointer char pentru retinerea secventei initiale de biti (0 si 1)
  		*it		-	pointer pentru retinerea adresei sirului *sir.
  		*subsir	-	necesar pentru a extrace un subsir de lungime p din sirul de biti
  		**secv	-	tabloul de pointeri pentru retinerea secventelor
	*/
	char *sir, *it, *subsir, **secv = NULL;
  
	sir = (char*) malloc(32010);

	//	Declarare fisiere de citire scriere
	freopen("secv.txt","r",stdin); 
	freopen("rez.txt", "w",stdout);
  
	//	Citire a si b
	scanf("%d %d ",&a,&b);
	subsir=(char*)malloc(b+1);
  
	//	Citire secventa biti si calcularea lungimii sirului (se retine in variabila n)
	scanf("%s",sir);
	n = strlen(sir);
  
	//	Eliberarea memorie libera din sir
	sir = (char*) realloc(sir,n+1);

	//	Calucularea aparitii numar de secvente
	for(p=a; p<=b ;p++)
	{
		for(it = sir, i=0; i<=n-p; ++i, ++it)	//	Se parcurge sirul de biti prin intermediul pointerului *it reinitializat la fiecare parcurgere de dimensiune p
		{										//	Sirul de caractere se parcurge pana cand i=n-p, ce reprezinta pozitia ultimei secvente de dimensiune p
												//	Se incrementeaza pointerul it pentru a putea folosi functia strncpy(), in selectarea subsirurilor
												
		strncpy(subsir,it,p);	//	Selectia subsirului de dimensiune p	
		pos = cauta(subsir, secv, nr_secv, first);	//	Se verifica existenta secventei in tablou. 
													//	Variabila pos reprezinta pozitia la care a fost gasita secventa in tablou. In caz contrar retine -1; 
	  
		if( pos != -1 )
			++aparitii[pos];	//	Incrementarea numarul de aparitii cu 1
	  
		else
			adauga( subsir, p, &secv, &aparitii, &nr_secv);  //	In caz ca subsirul nu se gaseste in tablou se memoreaza ca o secventa separata
		}
	
	first = nr_secv-1;	//	Pozitia primei secvente din taboul de dimensiunea p
	}

	//	Ordonarea secvente dupa numarul de aparitii
	ord(secv,aparitii,nr_secv);
  
	//	Afisare formatata a secventelor
	afisare(secv,aparitii,nr_secv);
  
	return 0;
}
