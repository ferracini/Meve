#include <stdio.h>
#include <stdlib.h>
#include "meve.h"

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

const char* GetErrorString(
	MeveCode code)
{
	switch (code)
	{
		case MEVE_ERR_INVALIDPARAM:
		{
			return "Invalid parameter.\n";
		}
		case MEVE_ERR_INVALIDSIZE:
		{
			return "Invalid size.\n";
		}
		case MEVE_ERR_OUTOFSTACK:
		{
			return "Out of stack.\n";
		}
		case MEVE_ERR_PARESNOTBALANCED:
		{
			return "Parenthesis not balanced.\n";
		}
		case MEVE_ERR_INVALIDEXPR:
		{
			return "Invalid expression.\n";
		}
		default:
		{
			/* Explicitly left empty. */
		}
	}
	return NULL;
}

int main()
{
	MeveCode res;
	MEVE_CONTEXT ctx;
	MTOKEN tkBuff[256];
	
	char iStr[512] = { '\0' };

	MeveInit(&ctx, tkBuff, sizeof(tkBuff), iStr);

	while (1)
	{
		printf(">> ");
		if (!fgets(iStr, sizeof(iStr) - 1, stdin))
		{
			return 1;
		}

		MeveUpdate(&ctx);

		if (!STRNICMP(iStr, "clear", strlen("clear")))
		{
			system("cls");
		}
		else if (!STRNICMP(iStr, "exit", strlen("exit")))
		{
			return 0;
		}
		else if ((res = EvalString(&ctx)) == MEVE_ERR_OK)
		{
			printf("\nans =\n   %Lg\n\n", ctx.ans);
		}
		else if (iStr[0] != '\n')
		{
			printf("%s\n", GetErrorString(res));
		}
	}

	return 1;
}