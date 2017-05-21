#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "stack.h"

Stack stiva=NULL;

int operatie(int a, int b , char op){		/*aplica un operator pe 2 intregi si intoarce rezultatul*/
	switch (op){
	case EXP:
		return pow(a,b);
	case MUL:
		return a*b;
	case DIV:
		return a/b;
	case ADD:
		return a+b;
	default:
		return a-b;
	}
}

int prioritate(char a){
	if(a==EXP)
		return 3;
	return ((a==MUL)||(a==DIV))?2:1;
}

int prec(char a, char b){	/*determina precedenta a 2 operatori*/
    if (prioritate(a)==prioritate(b)){
        if((a==EXP)||(b=='('))
            return 1;
        else
            return -1;
    }
    else
        return prioritate(a)-prioritate(b);
}

int	push(Stack *S, void *elem)	/* adaugarea la stiva */
{
    nod *p;
    p=(nod*)malloc(sizeof(nod));
    p->data=elem;

    if(*S) {
        p->next=*S;
    }
    else {
        p->next=NULL;
    }
    *S=p;
return 0;
}
void pop(Stack *S){	/* stergerea din stiva, nu intoarce elementul sters */
    nod *p;
    p=*S;
    *S=p->next;
    free(p);
}
void* top(Stack S){ /* elementul din varful stivei */
    if(S) return S->data;
return 0;
}
uint	GetSize(Stack S){
    int k=0;
    while(S) {
        k++;
        S=S->next;
    }
    return k;
}
uint	isEmpty(Stack S){
    if(S) return 0;
    return 1;
}

int evaluate(char* sir){

    Stack S=NULL;
    int n=strlen(sir);
    int i;
	for (i=0;i<n;i++)
	{
		if (isDigit(sir[i]))
		{
		    int *c;
            c=(int*)malloc(sizeof(int));
            *c=(int)sir[i]-'0';
			push(&S,c);
		}
		else
		{
            if (GetSize(S) < 2)
				{
					printf("Forma postfixată este eronată\n");
					return 1;
				}
			else
				{
					int a, b, *c;
					a = *((int*)top(S));
					pop(&S);
					b = *((int*)top(S));
					pop(&S);
					c=(int*)malloc(sizeof(int));
					*c=operatie(b,a,sir[i]);
					push(&S,c);
				}
		}
	}
return *((int*)top(S));
}

int main(){

	/*test operatii stiva*/
/*	char a[]="3+4*2/(1−5)^2^3";
	push(&stiva,&a[0]);
	push(&stiva,&a[1]);
	push(&stiva,&a[2]);
	printf("%c\n",*((char*)top(stiva)));
	pop(&stiva);
	printf("%c\n",*((char*)top(stiva)));
	printf("%i\n",GetSize(stiva));
	pop(&stiva);
	pop(&stiva);
*/

	/*test evaluare*/
	printf("Rezultatul este %d\n",evaluate("342*15-23^^/+"));

	return 0;
}
