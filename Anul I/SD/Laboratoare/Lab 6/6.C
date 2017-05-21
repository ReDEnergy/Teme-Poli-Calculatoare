#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "queue.h"
/*
typedef struct nod {
	void *data;
	struct nod* next;
} q_nod;

typedef q_nod* Queue;
*/

typedef struct {
    int coef;
    int exp;
} pol;

int enqueue(Queue* q, void* elem){
    Queue a;
    Queue p;
    p=*q;
    a =(q_nod*) malloc (sizeof(q_nod));
    a->data = elem;
    a->next = NULL;
    if (*q) {
        while (p->next)
            p=p->next;
        p->next=a;
    }
    else {
        a->next=NULL;
        *q=a;
    }
return 1;
}

/* Extragere din coada. */
void* dequeue(Queue* q){
    Queue p=*q;

    if (*q) {
        *q =(*q)->next;
    }
    return p->data;
}

/* Inspectarea primului element. */
void* Front(Queue q){
    return q->data;
}

/* Test coada vida. */
int isEmpty(Queue q) {
    if(q)   return 1;
    return 0;
}

/* dimensiunea cozii */
int size(Queue q){
    int nr=0;
    Queue p=q;
    while (p) {
        nr++;
        p=p->next;
    }
    return nr;
}

int cifraX(int nr, int pos){
	return (nr/((int)pow(10,pos)))%10;
}

void init_cozi(Queue *q){
	int i;
	for(i=0;i<10;i++){
		q[i]=NULL;
	}
}

void radix(int *a, int *b, int n){  /*radix sort, folositi functia cifraX pentru a determina coada in care intra fiecare element*/
	Queue cozi[10];
	init_cozi(cozi);
	int i,j;

	for (j=0;j<3;j++) {

        for (i=0;i<8;i++) {
            enqueue(&cozi[cifraX(a[i],j)],&a[i]);
        }
        int k=0;
        for (i=0;i<9;i++) {
            while(size(cozi[i])) {
                b[k]=*((int*)dequeue(&cozi[i]));
                k++;
            }
        }
        for (i=0;i<8;i++)
            a[i]=b[i];
	}

}

int main(){
	int a[]={170, 45, 75, 90, 2, 24, 802, 66} ;
	int b[8];
	int i;
	q_nod *coada=NULL;


	enqueue(&coada,&a[0]);
	enqueue(&coada,&a[2]);
	enqueue(&coada,&a[5]);
	printf("%d\n",*((int*)dequeue(&coada)));
	printf("%d\n",*((int*)Front(coada)));
	printf("%d\n",size(coada));

	radix(a,b,8);

	for(i=0;i<8;i++)
		printf("%d ",b[i]);
	printf("\n");
    return 1;
}



