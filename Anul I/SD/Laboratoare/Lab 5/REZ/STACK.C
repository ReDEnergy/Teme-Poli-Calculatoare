#include "stack.h"
#include <stdlib.h>

int push(Stack *S, void *elem)
{
	if (isEmpty(*S))
	{
		*S = (nod *)malloc(sizeof(struct list_head_struct));
		(*S)->data = elem;
		(*S)->next = NULL;
	}
	else
	{
		nod *p;

		p = (nod *)malloc(sizeof(struct list_head_struct));
		p->data = elem;
		p->next = *S;
		*S = p;
	}

	return 1;
}

void pop(Stack *S)
{
	if (isEmpty(*S))
		return;

	nod *p = *S;

	*S = (*S)->next;
	free(p);
}

void *top(Stack S)
{
	if (isEmpty(S))
		return NULL;

	return S->data;
}

uint GetSize(Stack S)
{
	if (isEmpty(S))
		return 0;

	nod *p = S;
	int i = 0;

	for (; p->next != NULL; i++, p = p->next);

	return i + 1;
}

uint isEmpty(Stack S)
{
	if (S != NULL)
		return 0;

	return 1;
}
