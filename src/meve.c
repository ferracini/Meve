#include "meve.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mevemath.h"

#define SPS_DECIMAL				'.'	/* Decimal symbol.					*/
#define SPS_COMMA				','	/* Parameter delimiter symbol.		*/
#define SPS_OBRKT				'('	/* Open brackets.					*/
#define SPS_CBRKT				')'	/* Close brackets.					*/

#define AS_LTR					0	/* Left to right associavity.		*/
#define AS_RTL					1	/* Right to left associavity.		*/

#define MEVE_MAX_FUN_ARITY		2	/* Maximum function arity.			*/

static const MEVE_FUN gFunList[] =
{
	/*
	 *	name		function ptr	arity	associavity
	*/
	{	"exp",		&Exp,			1,		AS_RTL	},
	{	"sqrt",		&Sqrt,			1,		AS_RTL	},
	{	"abs",		&Abs,			1,		AS_RTL	},
	{	"sin",		&Sin,			1,		AS_RTL	},
	{	"cos",		&Cos,			1,		AS_RTL	},
	{	"tan",		&Tan,			1,		AS_RTL	},
	{	"arcsin",	&ArcSin,		1,		AS_RTL	},
	{	"arccos",	&ArcCos,		1,		AS_RTL	},
	{	"arctan",	&ArcTan,		1,		AS_RTL	},
	{	"log10",	&Log10,			1,		AS_RTL	},
	{	"log",		&Log,			1,		AS_RTL	},
	{	"min",		&Min,			2,		AS_RTL	},
	{	"max",		&Max,			2,		AS_RTL	},
	{	"!",		&Factorial,		1,		AS_LTR	},
};

enum OperatorPrecedence
{
	OPR_P_ADD,					/* Additive.							*/
	OPR_P_MULT,					/* Multiplicative.						*/
	OPR_P_STA,					/* Statistic.							*/
	OPR_P_UNARY,				/* Unary.								*/
	OPR_P_POW					/* Power.								*/
};

static const MEVE_OPR gOprList[] =
{
	/*
	 *	symbol		function ptr	precedence		associativity
	*/
	{	"+",		&Add,			OPR_P_ADD,		AS_LTR	},
	{	"-",		&Sub,			OPR_P_ADD,		AS_LTR	},
	{	"*",		&Mult,			OPR_P_MULT,		AS_LTR	},
	{	"/",		&Div,			OPR_P_MULT,		AS_LTR	},
	{	"mod",		&Mod,			OPR_P_MULT,		AS_LTR	},
	{	"choose",	&Choose,		OPR_P_STA,		AS_LTR	},
	{	"+",		&UnAdd,			OPR_P_UNARY,	AS_RTL	},
	{	"-",		&UnSub,			OPR_P_UNARY,	AS_RTL	},
	{	"^",		&Pow,			OPR_P_POW,		AS_RTL	},
};

static const MEVE_CONST gConstList[] =
{
	/*
	 *	name	constant number
	*/
	{	"e",	2.718281828459045235360287471352662498L	},
	{	"pi",	3.141592653589793238462643383279502884L	},
	{	"phi",	1.618033988749894848204586834365638118L },
};

#define STACKISDONE(sTop, tk)	sTop								&&\
								sTop->type != MEVE_TT_OBRKT			&&\
								(sTop->type == MEVE_TT_FUN			||\
								(sTop->type == MEVE_TT_OPR			&&\
								(tk.data.opr->prec != OPR_P_UNARY	&&\
								(GetPrec(sTop) > GetPrec(&tk)		||\
								(GetPrec(sTop) == GetPrec(&tk)		&&\
								sTop->data.opr->as == AS_LTR)))))

#define UNARYCHECK(out, index)	(!index										||\
								(index										&&\
								(out->tk[index - 1].type == MEVE_TT_OBRKT	||\
								out->tk[index - 1].type == MEVE_TT_OPR		||\
								out->tk[index - 1].type == MEVE_TT_PARAMDELIM)))

#define GETSTACK_MAXLEN(s)		(s.sz / sizeof(MTOKEN))

int32_t GetPrec(
	const MTOKEN* tk)
{
	return tk->data.opr->prec;
}

bool IsDigit(
	const char symb)
{
	return (((uint32_t)symb - '0') < 10u) || (symb == SPS_DECIMAL);
}

const MEVE_OPR* GetOperatorPtr(
	const char* str,
	const bool unary)
{
	for (size_t i = 0; i < sizeof(gOprList) / sizeof(MEVE_OPR); i++)
	{
		if (!STRNICMP(str, gOprList[i].str, strlen(gOprList[i].str)) &&
			unary == (gOprList[i].prec == OPR_P_UNARY))
		{
			return &gOprList[i];
		}
	}
	return NULL;
}

bool IsOperator(
	const MEVE_OPR** const opr,
	const char* str,
	const bool isUnary)
{
	*opr = GetOperatorPtr(str, isUnary);
	if (*opr)
	{
		return true;
	}
	return false;
}

bool IsOpenBrackets(
	const char symb)
{
	return symb == SPS_OBRKT;
}

bool IsDelimiter(
	const char symb)
{
	return symb == SPS_COMMA;
}

bool IsCloseBrackets(
	const char symb)
{
	return symb == SPS_CBRKT;
}

bool IsConstant(
	const MEVE_CONST** c,
	const char* str)
{
	for (int i = 0; i < (sizeof(gConstList) / sizeof(MEVE_CONST)); i++)
	{
		if (!STRNICMP(str, gConstList[i].str, strlen(gConstList[i].str)))
		{
			*c = &gConstList[i];
			return true;
		}
	}
	return false;
}

bool IsWhitespace(
	const char smb)
{
	return isspace(smb);
}

bool IsFunction(
	const MEVE_FUN** fun,
	const char* str)
{
	for (int i = 0; i < (sizeof(gFunList) / sizeof(MEVE_FUN)); i++)
	{
		if (!STRNICMP(str, gFunList[i].str, strlen(gFunList[i].str)))
		{
			*fun = &gFunList[i];
			return true;
		}
	}
	return false;
}

bool ImplicitMulNum(
	MEVE_CONTEXT* ctx)
{
	const size_t len = ctx->expr.len;

	if (!len)
	{
		return true;
	}

	if (ctx->expr.tk[len - 1].type == MEVE_TT_CBRKT ||
		ctx->expr.tk[len - 1].type == MEVE_TT_CONST ||
		(ctx->expr.tk[len - 1].type == MEVE_TT_FUN &&
			ctx->expr.tk[len - 1].data.fun->as == AS_LTR))
	{
		if (len + 2 > GETSTACK_MAXLEN(ctx->expr))
		{
			return false;
		}

		ctx->expr.tk[len].type = MEVE_TT_OPR;
		ctx->expr.tk[len].data.opr = GetOperatorPtr("*", false);
		ctx->expr.len++;
	}
	return true;
}

bool ImplicitMulBrkt(
	MEVE_CONTEXT* ctx)
{
	const size_t len = ctx->expr.len;

	if (!len)
	{
		return true;
	}

	if (ctx->expr.tk[len - 1].type == MEVE_TT_CBRKT ||
		ctx->expr.tk[len - 1].type == MEVE_TT_NUM ||
		ctx->expr.tk[len - 1].type == MEVE_TT_CONST ||
		(ctx->expr.tk[len - 1].type == MEVE_TT_FUN &&
			ctx->expr.tk[len - 1].data.fun->as == AS_LTR))
	{
		if (len + 2 > GETSTACK_MAXLEN(ctx->expr))
		{
			return false;
		}

		ctx->expr.tk[len].type = MEVE_TT_OPR;
		ctx->expr.tk[len].data.opr = GetOperatorPtr("*", false);
		ctx->expr.len++;
	}

	return true;
}

bool ImplicitMulFun(
	MEVE_CONTEXT* ctx)
{
	const size_t len = ctx->expr.len;

	if (!len)
	{
		return true;
	}

	if (ctx->expr.tk[len].data.fun->as != AS_LTR)
	{
		if (ctx->expr.tk[len - 1].type == MEVE_TT_NUM ||
			ctx->expr.tk[len - 1].type == MEVE_TT_CBRKT ||
			(ctx->expr.tk[len - 1].type == MEVE_TT_FUN &&
				ctx->expr.tk[len - 1].data.fun->as == AS_LTR))
		{
			if (len + 2 > GETSTACK_MAXLEN(ctx->expr))
			{
				return false;
			}

			ctx->expr.tk[len + 1].data.fun = ctx->expr.tk[len].data.fun;
			ctx->expr.tk[len].data.opr = GetOperatorPtr("*", false);
			ctx->expr.tk[len].type = MEVE_TT_OPR;
			ctx->expr.len++;
		}
	}
	return true;
}

bool ImplicitMulConst(
	MEVE_CONTEXT* ctx)
{
	const size_t len = ctx->expr.len;

	if (!len)
	{
		return true;
	}

	if (ctx->expr.tk[len - 1].type == MEVE_TT_CBRKT ||
		ctx->expr.tk[len - 1].type == MEVE_TT_NUM ||
		ctx->expr.tk[len - 1].type == MEVE_TT_CONST ||
		(ctx->expr.tk[len - 1].type == MEVE_TT_FUN &&
			ctx->expr.tk[len - 1].data.fun->as == AS_LTR))
	{
		if (len + 2 > GETSTACK_MAXLEN(ctx->expr))
		{
			return false;
		}

		ctx->expr.tk[len + 1].data.cst = ctx->expr.tk[len].data.cst;
		ctx->expr.tk[len].data.opr = GetOperatorPtr("*", false);
		ctx->expr.tk[len].type = MEVE_TT_OPR;
		ctx->expr.len++;
	}

	return true;
}

size_t StrToNum(
	num_t* res,
	const char* strInput)
{
	char* endPtr = NULL;
	*res = strtold(strInput, &endPtr);
	return endPtr ? (size_t)(endPtr - strInput) : 0U;
}

void StackInit(
	TSTACK* stack,
	MTOKEN* tkBuff,
	size_t buffSz)
{
	stack->len = 0;
	stack->top = NULL;	
	stack->tk = tkBuff;
	stack->sz = buffSz;
}

void StackClear(
	TSTACK* stack)
{
	stack->len = 0;
	stack->top = NULL;
}

void StackPush(
	TSTACK* stack,
	MTOKEN* tk)
{
	if (tk)
	{
		stack->tk[stack->len] = *tk;
	}

	stack->len++;
	stack->top = &stack->tk[stack->len - 1];
}

void StackPushNum(
	TSTACK* stack,
	const num_t val)
{
	stack->tk[stack->len].data.val = val;
	stack->tk[stack->len].type = MEVE_TT_NUM;
	stack->len++;
	stack->top = &stack->tk[stack->len - 1];
}

void StackPop(
	TSTACK* const stack)
{
	if (stack->len > 0)
	{
		stack->len--;
		stack->top = stack->len > 0 ? &stack->tk[stack->len - 1] : NULL;
	}
}

MeveCode CreateTokens(
	MEVE_CONTEXT* ctx)
{
	if (!ctx || !ctx->iStr)
	{
		return MEVE_ERR_INVALIDPARAM;
	}

	if (!ctx->strLen)
	{
		return MEVE_ERR_INVALIDSIZE;
	}

	for (size_t i = 0; i < ctx->strLen; i++)
	{
		if (!IsWhitespace(ctx->iStr[i]))
		{
			if (ctx->expr.len == GETSTACK_MAXLEN(ctx->expr))
			{
				return MEVE_ERR_OUTOFSTACK;
			}
			else if (IsDigit(ctx->iStr[i]))
			{
				size_t strLen;

				if (!ImplicitMulNum(ctx))
				{
					return MEVE_ERR_OUTOFSTACK;
				}

				ctx->expr.tk[ctx->expr.len].type = MEVE_TT_NUM;
				strLen = StrToNum(&ctx->expr.tk[ctx->expr.len].data.val, &ctx->iStr[i]);

				i += strLen - 1;
				ctx->expr.len++;
			}
			else if (IsOperator(
				&ctx->expr.tk[ctx->expr.len].data.opr,
				&ctx->iStr[i],
				UNARYCHECK((&ctx->expr), ctx->expr.len)))
			{
				ctx->expr.tk[ctx->expr.len].type = MEVE_TT_OPR;
				i += strlen(ctx->expr.tk[ctx->expr.len].data.opr->str) - 1;
				ctx->expr.len++;
			}
			else if (IsOpenBrackets(ctx->iStr[i]))
			{
				if (!ImplicitMulBrkt(ctx))
				{
					return MEVE_ERR_OUTOFSTACK;
				}

				ctx->expr.tk[ctx->expr.len].type = MEVE_TT_OBRKT;
				ctx->expr.len++;
			}
			else if (IsCloseBrackets(ctx->iStr[i]))
			{
				ctx->expr.tk[ctx->expr.len].type = MEVE_TT_CBRKT;
				ctx->expr.len++;
			}
			else if (IsDelimiter(ctx->iStr[i]))
			{
				ctx->expr.tk[ctx->expr.len].type = MEVE_TT_PARAMDELIM;
				ctx->expr.len++;
			}
			else if (IsFunction(&ctx->expr.tk[ctx->expr.len].data.fun, &ctx->iStr[i]))
			{
				i += strlen(ctx->expr.tk[ctx->expr.len].data.fun->str) - 1;

				if (!ImplicitMulFun(ctx))
				{
					return MEVE_ERR_OUTOFSTACK;
				}

				ctx->expr.tk[ctx->expr.len].type = MEVE_TT_FUN;
				ctx->expr.len++;
			}
			else if (IsConstant(&ctx->expr.tk[ctx->expr.len].data.cst, &ctx->iStr[i]))
			{
				i += strlen(ctx->expr.tk[ctx->expr.len].data.cst->str) - 1;

				if (!ImplicitMulConst(ctx))
				{
					return MEVE_ERR_OUTOFSTACK;
				}

				ctx->expr.tk[ctx->expr.len].type = MEVE_TT_CONST;
				ctx->expr.len++;
			}
			else
			{
				return MEVE_ERR_INVALIDEXPR;
			}
		}	
	}

	ctx->expr.top = &ctx->expr.tk[ctx->expr.len - 1];

	return MEVE_ERR_OK;
}

MeveCode InfixToPostfix(
	MEVE_CONTEXT* ctx)
{
	TSTACK stack;
	MTOKEN tBuffer[MAXLEN_TBUFFER];

	if (!ctx)
	{
		return MEVE_ERR_INVALIDPARAM;
	}

	StackInit(&stack, tBuffer, MAXSIZE_TBUFFER);

	size_t len = 0;

	for (size_t i = 0; i < ctx->expr.len; i++)
	{
		switch (ctx->expr.tk[i].type)
		{
			case MEVE_TT_CONST:
			case MEVE_TT_NUM:
			{
				ctx->expr.tk[len++] = ctx->expr.tk[i];
				break;
			}
			case MEVE_TT_FUN:
			{
				StackPush(&stack, &ctx->expr.tk[i]);
				break;
			}
			case MEVE_TT_OPR:
			{
				while (STACKISDONE(stack.top, ctx->expr.tk[i]))
				{
					ctx->expr.tk[len++] = *stack.top;
					StackPop(&stack);					
				}

				StackPush(&stack, &ctx->expr.tk[i]);
				break;
			}
			case MEVE_TT_OBRKT:
			{
				StackPush(&stack, &ctx->expr.tk[i]);
				break;
			}
			case MEVE_TT_CBRKT:
			{
				while (stack.top && stack.top->type != MEVE_TT_OBRKT)
				{
					/* Discard parameter delimiters */
					if (stack.top->type != MEVE_TT_PARAMDELIM)
					{
						ctx->expr.tk[len++] = *stack.top;
					}
					StackPop(&stack);
				}
				StackPop(&stack);
				break;
			}
			case MEVE_TT_PARAMDELIM:
			{
				while (stack.top &&
					stack.top->type != MEVE_TT_OBRKT &&
					stack.top->type != MEVE_TT_PARAMDELIM)
				{
					ctx->expr.tk[len++] = *stack.top;
					StackPop(&stack);
				}
				break;
			}
			default:
			{
				/* Explicitly left empty */
			}
		}
	}

	while (stack.top)
	{
		ctx->expr.tk[len++] = *stack.top;
		StackPop(&stack);
	}
	ctx->expr.top = len ? &ctx->expr.tk[len - 1] : NULL;
	ctx->expr.len = len;

	return MEVE_ERR_OK;
}

MeveCode PostfixEval(
	MEVE_CONTEXT* ctx)
{
	if (!ctx)
	{
		return MEVE_ERR_INVALIDPARAM;
	}

	TSTACK stack;
	MTOKEN tBuffer[MAXLEN_TBUFFER];
	num_t arg[MEVE_MAX_FUN_ARITY];

	StackInit(&stack, tBuffer, MAXSIZE_TBUFFER);

	for (size_t i = 0; i < ctx->expr.len; i++)
	{
		switch (ctx->expr.tk[i].type)
		{
			case MEVE_TT_CONST:
			{
				StackPush(&stack, &ctx->expr.tk[i]);
				stack.top->data.val = stack.top->data.cst->cVal;
				stack.top->type = MEVE_TT_NUM;
				break;
			}
			case MEVE_TT_NUM:
			{
				StackPush(&stack, &ctx->expr.tk[i]);
				break;
			}
			case MEVE_TT_OPR:
			{
				if (ctx->expr.tk[i].data.opr->prec != OPR_P_UNARY)
				{
					if (!stack.top)
					{
						return MEVE_ERR_INVALIDEXPR;
					}

					arg[1] = stack.top->data.val;
					StackPop(&stack);
				}

				if (!stack.top)
				{
					return MEVE_ERR_INVALIDEXPR;
				}

				arg[0] = stack.top->data.val;
				StackPop(&stack);

				StackPushNum(&stack, ctx->expr.tk[i].data.opr->fPtr(arg));
				break;
			}
			case MEVE_TT_FUN:
			{
				if (stack.len < ctx->expr.tk[i].data.fun->arity)
				{
					return MEVE_ERR_INVALIDEXPR;
				}

				for (int j = ctx->expr.tk[i].data.fun->arity - 1; j >= 0; j--)
				{
					arg[j] = stack.top->data.val;
					StackPop(&stack);
				}

				StackPushNum(&stack, ctx->expr.tk[i].data.opr->fPtr(arg));
				break;
			}
			default:
			{
				/* Explicitly left empty. */
			}
		}
	}

	if (stack.len != 1)
	{
		return MEVE_ERR_INVALIDEXPR;
	}

	ctx->ans = stack.top->data.val;
	return MEVE_ERR_OK;
}

bool AreParanthesesBalanced(
	const char* iStr,
	const size_t len)
{
	size_t pCount = 0;

	for (size_t i = 0; i < len; i++)
	{
		if (iStr[i] == SPS_OBRKT)
		{
			pCount++;
		}
		else if (iStr[i] == SPS_CBRKT)
		{
			if (pCount == 0)
			{
				return false;
			}

			pCount--;
		}
	}
	return (bool)!pCount;
}

MeveCode EvalString(
	MEVE_CONTEXT* ctx)
{
	MeveCode res;

	if (!ctx || !ctx->iStr || !ctx->strLen)
	{
		return MEVE_ERR_INVALIDPARAM;
	}

	if (!AreParanthesesBalanced(ctx->iStr, ctx->strLen))
	{
		return MEVE_ERR_PARESNOTBALANCED;
	}

	if ((res = CreateTokens(ctx)) != MEVE_ERR_OK)
	{
		return res;
	}

	if ((res = InfixToPostfix(ctx)) != MEVE_ERR_OK)
	{
		return res;
	}

	if ((res = PostfixEval(ctx)) != MEVE_ERR_OK)
	{
		return res;
	}

	return res;
}

void MeveInit(
	MEVE_CONTEXT* ctx,
	MTOKEN* tkBuff,
	size_t buffSz,
	const char* iStr)
{
	ctx->iStr = iStr;
	ctx->strLen = strlen(ctx->iStr);
	ctx->ans = 0;
	ctx->expr.tk = tkBuff;
	ctx->expr.sz = buffSz;
	StackClear(&ctx->expr);
}

void MeveUpdate(
	MEVE_CONTEXT* ctx)
{
	StackClear(&ctx->expr);
	ctx->strLen = strlen(ctx->iStr);
}