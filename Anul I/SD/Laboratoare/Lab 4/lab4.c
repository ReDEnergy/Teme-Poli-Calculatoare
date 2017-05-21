#include<stdio.h>
#include<stdlib.h>


typedef struct nod {
	void* value;
	struct nod *next;
	struct nod *prev;
}Nod;

typedef Nod *List;


/* adaugare intr-o pozitie data (idem, pentru valoarea intoarsa)*/
int insertAt(List *l, void* elem, int poz){
        int i;
        Nod *q;
        q=(Nod*)malloc(sizeof(Nod));
        q->value=elem;

    if(*l==NULL) {
        q->next=q;
        q->prev=q;
        *l=q;
    }
    else {
        List p;
        p=*l;
        for(i=0;i<poz-1;i++) p=p->next;
        q->next=p;
        p->prev=q;
        q->prev=p->prev;
        if(poz==1) *l=q;
    }

return 0;
}
/* stergerea elementului dintr-o pozitie data */
void* delAt(List *l, int poz){
    int i;
    List p,q;
    p=*l;
    for(i=0;i<poz;i++) p=p->next;
    p->prev->next=p->next;
    p->next->prev=p->prev;

}
/* intoarce elementul dintr-o pozitie data (NULL daca pozitia nu e in lista)*/
void* elementAt(List l, int poz){
    List p;
    int i;
    p=l;
    for(i=0;i<poz;i++) p=p->next;
    return p->value;
}
/* intoarce lungimea listei*/
unsigned int sizeOfList(List l){
    List p;
    p=l;
    unsigned int k=1;
    do {
        k++;
        p=p->next;
    } while(p!=l);

    return k;
}
/* test lista vida, 1 daca e vida, 0 altfel*/
unsigned int isEmpty(List l){
    if(l) return 0;
    return 1;
}

int main()
{
	List l = NULL;
	int i,nr=0;

	int valori[] = {0,1,2,4,8,16,32,64};

	for(i=0;i<8;i++) {
		insertAt(&l,&valori[i],1);
        nr++;
	}

 //    int n = sizeOfList(l);

        Nod *p ;
        p = (Nod*)delAt(&l,4);
        nr--;
        printf("Deleted=%d\n",*(int*)p->value);

	for(i=0;i <nr;i++)
	{
		void* x = elementAt(l,i);
		printf("%d ",*(int*)x);
	}
 //   printf("\nSize of list : %d",sizeOfList(l));
    return 0;
}
