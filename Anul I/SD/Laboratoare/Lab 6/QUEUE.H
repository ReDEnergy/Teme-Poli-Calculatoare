#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct nod {
	void *data;
	struct nod* next;
} q_nod;

typedef q_nod* Queue;

/* Adaugare in coada. */
int enqueue(Queue*, void*);

/* Extragere din coada. */
void* dequeue(Queue*);

/* Inspectarea primului element. */
void* Front(Queue);

/* Test coada vida. */
int isEmpty(Queue);

/* dimensiunea cozii */
int size(Queue);

#endif
