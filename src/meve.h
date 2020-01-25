#ifndef MEVE_H
#define MEVE_H
#include "mevenum.h"
#include <stdint.h>
#include <string.h>
#ifdef __GNUC__
#include <stddef.h>
#include <strings.h>
#define STRNICMP(a, b, m)	strncasecmp(a, b, m)
#else
#define STRNICMP(a, b, m)	strnicmp(a, b, m)
#endif

/* Maximum length of the temporary internal buffer.	*/
#define MAXLEN_TBUFFER		256

/* Maximum size of the temporary buffer, in bytes.	*/
#define MAXSIZE_TBUFFER		(MAXLEN_TBUFFER * sizeof(MTOKEN))

/* Token types.							*/
typedef enum MeveTokenType
{
	MEVE_TT_OBRKT = 1,			/* Open brackets.						*/
	MEVE_TT_CBRKT,				/* Close brackets.						*/
	MEVE_TT_PARAMDELIM,			/* Parameter delimiter.					*/
	MEVE_TT_NUM,				/* Number.								*/
	MEVE_TT_OPR,				/* Operator.							*/
	MEVE_TT_FUN,				/* Function.							*/
	MEVE_TT_CONST,				/* Constant.							*/
}MeveTokenType;

/* Operator structure.					*/
typedef struct MEVE_OPR
{
	const char* str;			/* Pointer to a null-terminated string.	*/
	num_t(*fPtr)(const num_t*);	/* Callback function pointer.			*/
	int8_t prec;				/* Operator precedence.					*/
	int8_t as;					/* Operator associativity.				*/
}MEVE_OPR;

/* Function structure.					*/
typedef struct MEVE_FUN
{
	const char* str;			/* Pointer to a null-terminated string.	*/
	num_t(*fPtr)(const num_t*);	/* Operator callback function pointer.	*/
	uint8_t arity;				/* Function arity.						*/
	int8_t as;					/* Function associativity.				*/
}MEVE_FUN;

/* Constant structure.					*/
typedef struct MEVE_CONST
{
	const char* str;			/* Pointer to a null-terminated string.	*/
	num_t cVal;					/* The value of the constant.			*/
}MEVE_CONST;

/* Token data.							*/
typedef union MTOKEN_EVAL
{
	num_t val;					/* Token value.							*/
	const MEVE_OPR* opr;		/* Pointer to an operator structure.	*/
	const MEVE_FUN* fun;		/* Pointer to a function structure.		*/
	const MEVE_CONST* cst;		/* Pointer to a constant structure.		*/
}MTOKEN_EVAL;

/* Token structure.						*/
typedef struct MTOKEN
{
	MTOKEN_EVAL data;			/* Token data.							*/
	MeveTokenType type;			/* Token type.							*/
}MTOKEN;

/* Stack structure.						*/
typedef struct TSTACK
{
	MTOKEN* tk;					/* Pointer to the buffer.				*/
	size_t len;					/* Current buffer length.				*/
	size_t sz;					/* Buffer size.							*/
	MTOKEN* top;				/* Pointer to the top-of-stack.			*/
}TSTACK;

/* Meve context.						*/
typedef struct MEVE_CONTEXT
{
	const char* iStr;			/* Pointer to a null-terminated string.	*/
	size_t strLen;				/* Input-string length.					*/
	TSTACK expr;				/* Stack used to store the tokens.		*/
	num_t ans;					/* Most recent answer.					*/
}MEVE_CONTEXT;

/* Error messages.						*/
typedef enum MeveErrorMessage
{
	MEVE_ERR_OK,				/* No errors.							*/
	MEVE_ERR_INVALIDPARAM,		/* Invalid parameters.					*/
	MEVE_ERR_INVALIDSIZE,		/* Invalid input size.					*/
	MEVE_ERR_OUTOFSTACK,		/* Out of stack.						*/
	MEVE_ERR_PARESNOTBALANCED,	/* Parentheses not balanced.			*/
	MEVE_ERR_INVALIDEXPR,		/* Invalid expression.					*/
}MeveCode;

/* Create tokens by reading an input string.
 *
 * If the function succeeds, the return value is MEVE_ERR_OK.
 *
 * Remarks:
 * The Meve context must be properly initialized.
 * The ctx->iStr must represent a valid infix expression.
*/
MeveCode CreateTokens(
	MEVE_CONTEXT* ctx);			/* Pointer to a MEVE_CONTEXT structure.	*/

/* Converts an infix expression into an equivalent postfix expression.
 *
 * If the function succeeds, the return value is MEVE_ERR_OK.
 *
 * Remarks:
 * This function must be called after CreateTokens().
 * The Meve context must be properly initialized and the stack ctx->expr
 * must be a valid infix expression.
*/
MeveCode InfixToPostfix(
	MEVE_CONTEXT* ctx);			/* Pointer to a MEVE_CONTEXT structure.	*/

/* Evaluates a postfix expression.
 *
 * If the function succeeds, the return value is MEVE_ERR_OK.
 *
 * Remarks:
 * This function must be called after InfixToPostfix().
 * The stack ctx->expr must be a valid postfix expression.
*/
MeveCode PostfixEval(
	MEVE_CONTEXT* ctx);			/* Pointer to a MEVE_CONTEXT structure.	*/

/* Evaluates an expression in a string.
 *
 * If the function succeeds, the return value is MEVE_ERR_OK.
 *
 * Remarks:
 * The ctx pointer cannot be null.
*/
MeveCode EvalString(
	MEVE_CONTEXT* ctx);			/* Pointer to a MEVE_CONTEXT structure.	*/

/* Initializes a Meve context.
 *
 * Remarks:
 * Both ctx and iStr pointers cannot be null.
*/
void MeveInit(
	MEVE_CONTEXT* ctx,			/* Pointer to a Meve context.			*/
	MTOKEN* tkBuff,				/* Pointer to token buffer.				*/
	size_t buffSz,				/* Buffer size.							*/
	const char* iStr);			/* Pointer to the input-string.			*/

/* Updates Meve context entries.
 *
 * Remarks:
 * This function should be call every time the input-string changes.
 * The ctx pointer cannot be null and must be properly initialized. 
*/
void MeveUpdate(
	MEVE_CONTEXT* ctx);			/* Pointer to a Meve context.			*/
#endif