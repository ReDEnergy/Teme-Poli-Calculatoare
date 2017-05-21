#include "stack.h"
#include <stdio.h>
#include <string.h>

int isOp(char c)
{
	if (c == '+' ||
		c == '-' ||
		c == '*' ||
		c == '/' ||
		c == '^')
		return 1;

	return 0;
}

int isLt(char a, char b)
{
	if ((a == '+' || a == '-') && (b == '*' || b == '/'))
		return 1;

	if (a != '^' && b == '^')
		return 1;

	return 0;
}

int calc(char a, char b, char op)
{
	if (op == '+')
		return a + b;

	if (op == '-')
		return a - b;

	if (op == '*')
		return a * b;

	if (op == '/')
		return a / b;

	if (op == '^')
	{
		char i = 0;

		for (; i < b; i++)
			a *= a;

		return a;
	}
}

int main()
{
	/*Stack S = NULL;
	int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, i = 0;

	for (; i < 10; i++)
		push(&S, &array[i]);

	printf("Size: %d\nTop: %d\n", GetSize(S),*(int *)top(S));*/

	Stack S = NULL;
	char infix[256], postfix[256];
	int n = 0;

	printf("Introduceti o expresie in forma infixata:\n");
	scanf("%s", infix);

	int i = 0;

	for (; i < strlen(infix); i++)
	{
		if (infix[i] >= '0' && infix[i] <= '9')
			postfix[n++] = infix[i];

		if (isOp(infix[i]))
		{
			while (!isEmpty(S) && isOp(*(char *)top(S)) && isLt(infix[i], *(char *)top(S)))
			{
				postfix[n++] = *(char *)top(S);
				pop(&S);
			}

			push(&S, &infix[i]);
		}

		if (infix[i] == '(')
			push(&S, &infix[i]);

		if (infix[i] == ')')
		{
			while (*(char *)top(S) != '(')
			{
				if (isEmpty(S))
				{
					fprintf(stderr, "Expresia introdusa are parantezele gresite!\n");

					return 1;
				}

				postfix[n++] = *(char *)top(S);
				pop(&S);
			}

			pop(&S);
		}
	}

	while (!isEmpty(S))
	{
		if (*(char *)top(S) != '(' &&
			*(char *)top(S) != ')')
			postfix[n++] = *(char *)top(S);
		else
		{
			fprintf(stderr, "Expresia introdusa are parantezele gresite!\n");

			return 1;
		}

		pop(&S);
	}

	postfix[n] = '\0';

	printf("\nExpresia in forma postfixata:\n%s\n", postfix);

	for (i = 0; i < strlen(postfix); i++)
	{
		if (postfix[i] >= '0' && postfix[i] <= '9')
			push(&S, &postfix[i]);
		else
			if (isOp(postfix[i]))
				if (GetSize(S) < 2)
				{
					fprintf(stderr, "Forma postfixata este eronata!\n");

					return 1;
				}
				else
				{
					char a, b, c;

					b = *(char *)top(S) - '0';
					pop(&S);
					a = *(char *)top(S) - '0';
					pop(&S);
					c = calc(a, b, postfix[i]);
					push(&S, &c);
				}
	}

	if (GetSize(S) == 1)
		printf("Rezultat: %d\n", *(char *)top(S));
	else
	{
		fprintf(stderr, "Forma postfixata este eronata!\n");
		return 1;
	}
	return 0;
}
