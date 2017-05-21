/* fisierul stack.h */
#ifndef _STACK_H
#define _STACK_H

#define EXP '^'
#define MUL '*'
#define DIV '/'
#define ADD '+'
#define SUB '-'

#define isDigit(a) (a >= '0' && a <= '9')
#define isOp(a) (a == EXP || a == MUL || a== DIV || a==ADD || a== SUB)

typedef unsigned int uint;

typedef struct list_head_struct {
	void *data;
	struct list_head_struct *next;
} nod;

typedef nod* Stack;

int	    push(Stack *S, void *elem);	/* adaugarea la stiva */
void	pop(Stack *S);			/* stergerea din stiva, nu intoarce elementul
							sters */
void*	top(Stack S);			/* elementul din varful stivei */
uint	GetSize(Stack S);			/* numatul de elemente din stiva (pentru
							implementarea acestei functii se poate face
							parcurgerea stivei) */
uint	isEmpty(Stack S);			/* testul de stiva goala */


#endif
