// Ivanica Gabriel 313CA Tema1

#include<stdio.h>

int main ()
{
double n,eps=.000001;
long p1,p2=0,i=1,fp,grad,el1=1,el2=1,mem;

//Citirea numarului in format double

scanf("%lf",&n);

// Verificare daca numarul e conform cerintei , nr nat.  >2 si <1.000.000
// Pentru ca nummarul citit sa nu fie citit ca int in cazul unui numar de forma x.999999 se va compara modului diferentei (int)n-n cu variabila eps 

if(n>1 && n<1000001 && abs((int)n-n)<eps)
{

/* Verificare numar palindrom
   Sunt necesare 2 variabile :
   p1=n pentru a nu pierde valoarea variabilei n in timpul generarii numarului in oglinda
   p2 pentru a retine numarul in oglina */

p1=n;
while (p1){
	p2=p2*10+p1%10; 
	p1=p1/10;
	}
if (p2==n) printf("Numarul este palindrom\n");
else printf ("Numarul nu este palindrom\n");

/* Descompunerea in factori primi
   Se verifica pe rand la ce numar natural se imparte n. 
   Daca se imparte la numarul x: se va imparti succesiv la x si se va creste gradul pana cand restul este diferit de 0.
   Algoritmul se repeta pentru fiecare numar pana cand n=1; */

fp=n; // se retine numarul n intro variablia pentru a nu pierde valoarea acestuia

while (fp!=1) {
	i++; // i reprezinta un factor prim
	grad=0;
	while(fp%i==0) { 
		grad++; 
		fp=fp/i;
		}
	// Afisare factor si putere
	if (grad) {
		printf("%ld^%ld",i,grad);
		if(fp!=1) printf (" * ");
		}	
	}
printf("\n");		

/* Verificare numar fibonacii
   Folosind primele 2 numere Fibonacci se genereaza pe rand numerele pana cand numarul generat e mai mare ca n
   el1 si el2 reprezinta ultimele 2 numere Fibonacii generate
   variabila mem e folosita in generarea numerelor pentru a nu se pierde valoarea primului element cand se calculeaza al doilea
*/
while(n>el2) { 
	mem=el1;
	el1=el2;
	el2=el2+mem;
	}
if (n==el2) printf("Numarul este un numar Fibonacci\n");
else printf("Numarul nu este un numar Fibonacci\n");	

}
// Se iese direct din program daca numarul nu e conform cerintei
return 1;
}