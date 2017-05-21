#ifndef _STACK_H
#define _STACK_H

typedef unsigned int uint;

typedef struct list_head_struct
{
	void *data;
	struct list_head_struct *next;
} nod;

typedef nod *Stack;

int	push(Stack *S, void *elem);

void pop(Stack *S);

void *top(Stack S);

uint GetSize(Stack S);

uint isEmpty(Stack S);
 
#endif
