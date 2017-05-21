// Ivanica Gabriel 313CA
#include<stdio.h>

//	Functia va returna adresa ip convertita in format int
unsigned int iptoint (int a,int b,int c,int d)
{
	unsigned int adresa;
	adresa=(1<<24)*a+(1<<16)*b+(1<<8)*c+d;
	return adresa;
}

//	Functia este necesara pentru a calcula netmask-ul in format int
unsigned int inttomask (int net)
{
	unsigned int mask;
	mask=(~((1<<(32-net))-1));
	return mask;
}

/*	Functia va ordona descrescator in functie de netmask, reprezentat de vectorul mask[],
adresa cartierelor
	 Pentru afisarea rezultatului corespunzator este necesara salvarea numarului cartierului intr-un vetor separat
	 nrcartier[];
*/
void ordonare(int M,unsigned adr[],unsigned mask[],int nrcartier[])
{	int i,gasit=1;
	unsigned aux;
	while(gasit){
		gasit=0;
		for (i=0;i<M-1;i++) {
			if(mask[i]<mask[i+1]) { 
				// Interschimb maska de cartier
				aux=mask[i];  
				mask[i]=mask[i+1];  
				mask[i+1]=aux;  
				// Interschimb adresa de cartier
				aux=adr[i];  
				adr[i]=adr[i+1];  
				adr[i+1]=aux; 
				// Interschimb numrar de cartier 
				aux=nrcartier[i];  
				nrcartier[i]=nrcartier[i+1];
				nrcartier[i+1]=aux;
				// Initializarea iesire din while
				gasit=1;
				}
			}
		}	
}

// 	Functia va returna adresele de cartier si netmask-ului acestora precum si initializarea numarului de cartier
//	Adresele si netmask sunt retinute in format int, fiind prelucrate prin intermediul functiilor iptoint() si inttomask()
void cartiere(int M,unsigned adr[],unsigned mask[],int nrcartier[])
{
	int A,B,C,D,net,i;
	for (i=0;i<M;i++) {
		scanf("%d %d %d %d %d",&A,&B,&C,&D,&net);
		adr[i]=iptoint(A,B,C,D);
		mask[i]=inttomask(net);
		nrcartier[i]=i;
		}
}
/*	Functie apelabila la citirea fiecarui colet.
	Functia va calcula adresa cartierului de destinatie prin compararea adresei gateway-ului coletului cu cel al cartierului
	adr1 si adr2 reprezinta gateway-ul cartierului si al coletului ce sunt comparate
*/
void send(unsigned int colet,unsigned int mask[],unsigned int adr[],int M,int nrcartier[])
{
	int i=0,gasit=0;
	unsigned int adr1,adr2;
	while(gasit==0 && i<M)
		{
		adr1=adr[i]&mask[i];
		adr2=colet&mask[i];
		if (adr1==adr2)
			{
			printf("%d\n",nrcartier[i]);			
			gasit=1;
			}
		i++;
		}
	if(gasit==0) printf("-1\n");			
}

int main ()
{
unsigned int adr[1000],mask[1000],colet,N;
int M,X,Y,Z,T,i,nrcartier[1000];

//	Vectorul adr va retine adresa cartierului in format unsigned int
//	Vectorul mask este necesar pentru a memora netmask iar 

scanf("%d",&M);
cartiere(M,adr,mask,nrcartier);
ordonare(M,adr,mask,nrcartier);
scanf("%u",&N);

//	Se va citi cate un colet pe rand si se va apela functia send () pentru a afla destinatia 

for (i=0;i<N;i++) {
	scanf("%d %d %d %d",&X,&Y,&Z,&T);
	colet=iptoint(X,Y,Z,T);
	send(colet,mask,adr,M,nrcartier);
	}

return 1;
}
