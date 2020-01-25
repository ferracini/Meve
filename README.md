# Meve

Meve is an implementation written in C of the modified Shunting Yard algorithm.

## Usage

### Meve interface
```C
/* Create tokens by reading an input string.
 *
 * If the function succeeds, the return value is MEVE_ERR_OK.
 *
 * Remarks:
 * The Meve context must be properly initialized.
 * The ctx->iStr must represent a valid infix expression.
*/
MeveCode CreateTokens(
	MEVE_CONTEXT* ctx);			/* Pointer to a MEVE_CONTEXT structure.		*/

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
	MEVE_CONTEXT* ctx);			/* Pointer to a MEVE_CONTEXT structure.		*/

/* Evaluates a postfix expression.
 *
 * If the function succeeds, the return value is MEVE_ERR_OK.
 *
 * Remarks:
 * This function must be called after InfixToPostfix().
 * The stack ctx->expr must be a valid postfix expression.
*/
MeveCode PostfixEval(
	MEVE_CONTEXT* ctx);			/* Pointer to a MEVE_CONTEXT structure.		*/

/* Evaluates an expression in a string.
 *
 * If the function succeeds, the return value is MEVE_ERR_OK.
 *
 * Remarks:
 * The ctx pointer cannot be null.
*/
MeveCode EvalString(
	MEVE_CONTEXT* ctx);			/* Pointer to a MEVE_CONTEXT structure.		*/

/* Initializes a Meve context.
 *
 * Remarks:
 * Both ctx and iStr pointers cannot be null.
*/
void MeveInit(
	MEVE_CONTEXT* ctx,			/* Pointer to a Meve context.			*/
	MTOKEN* tkBuff,				/* Pointer to token buffer.			*/
	size_t buffSz,				/* Buffer size.					*/
	const char* iStr);			/* Pointer to the input-string.			*/

/* Updates Meve context entries.
 *
 * Remarks:
 * This function should be call every time the input-string changes.
 * The ctx pointer cannot be null and must be properly initialized. 
*/
void MeveUpdate(
	MEVE_CONTEXT* ctx);			/* Pointer to a Meve context.			*/
```

### Math functions

```C
typedef struct MEVE_FUN
{
	const char* str;		/* Pointer to a null-terminated string.	*/
	num_t(*fPtr)(const num_t*);	/* Operator callback function pointer.	*/
	uint8_t arity;			/* Function arity.			*/
	int8_t as;			/* Function associativity.		*/
}MEVE_FUN;
```

Math functions can be used by implementing a callback function and storing their pointers and names in the global variable gFunList[].
```C
static const MEVE_FUN gFunList[] =
{
	/*
	 *	name		function ptr	arity	associavity
	*/
	{	"exp",		&Exp,		1,	AS_RTL	},
	{	"sqrt",		&Sqrt,		1,	AS_RTL	},
	{	"abs",		&Abs,		1,	AS_RTL	},
	{	"sin",		&Sin,		1,	AS_RTL	},
	{	"cos",		&Cos,		1,	AS_RTL	},
	{	"tan",		&Tan,		1,	AS_RTL	},
	{	"arcsin",	&ArcSin,	1,	AS_RTL	},
	{	"arccos",	&ArcCos,	1,	AS_RTL	},
	{	"arctan",	&ArcTan,	1,	AS_RTL	},
	{	"log10",	&Log10,		1,	AS_RTL	},
	{	"log",		&Log,		1,	AS_RTL	},
	{	"min",		&Min,		2,	AS_RTL	},
	{	"max",		&Max,		2,	AS_RTL	},
	{	"!",		&Factorial,	1,	AS_LTR	},
};
```

### Math operators

```C
typedef struct MEVE_OPR
{
	const char* str;		/* Pointer to a null-terminated string.	*/
	num_t(*fPtr)(const num_t*);	/* Callback function pointer.		*/
	int8_t prec;			/* Operator precedence.			*/
	int8_t as;			/* Operator associativity.		*/
}MEVE_OPR;
```
Math operators can be used by implementing a callback function and storing their pointers and symbols in the global variable gOprList[].
```C
static const MEVE_OPR gOprList[] =
{
	/*
	 *	symbol		function ptr	precedence	associativity
	*/
	{	"+",		&Add,		OPR_P_ADD,	AS_LTR	},
	{	"-",		&Sub,		OPR_P_ADD,	AS_LTR	},
	{	"*",		&Mult,		OPR_P_MULT,	AS_LTR	},
	{	"/",		&Div,		OPR_P_MULT,	AS_LTR	},
	{	"mod",		&Mod,		OPR_P_MULT,	AS_LTR	},
	{	"choose",	&Choose,	OPR_P_STA,	AS_LTR	},
	{	"+",		&UnAdd,		OPR_P_UNARY,	AS_RTL	},
	{	"-",		&UnSub,		OPR_P_UNARY,	AS_RTL	},
	{	"^",		&Pow,		OPR_P_POW,	AS_RTL	},
};
```
### Math constants

```C
typedef struct MEVE_CONST
{
	const char* str;	/* Pointer to a null-terminated string.	*/
	num_t cVal;		/* The value of the constant.		*/
}MEVE_CONST;
```

Math constants can be used by adding their names and values to the global variable gConstList[].
```C
static const MEVE_CONST gConstList[] =
{
	/*
	 *	symbol	constant number
	*/
	{	"e",	2.718281828459045235360287471352662498L	},
	{	"pi",	3.141592653589793238462643383279502884L	},
	{	"phi",	1.618033988749894848204586834365638118L },
};
```

### Compile

```sh
make
```

## Dependencies

+ The code is written in standard C;
+ No dependencies of other libraries;

## Example
The code below shows how to calculate <img src="https://joyxoff.com/tmp/goldenratio.svg" align="middle" width=250 />
```C
MEVE_CONTEXT ctx;
MTOKEN tkBuff[256];
MeveCode res;

char iStr[] = "(1 + sqrt(5)) / 2";

MeveInit(&ctx, tkBuff, sizeof(tkBuff), iStr);

res = EvalString(&ctx);

if (res == MEVE_ERR_OK)
{
	printf("%s = %Lg\n", iStr, ctx.ans);
}
```
See the [`/src/test.c`](/src/test.c) file for more examples.


## Authors

* **Diego Ferracini Bando** - [ferracini](https://github.com/ferracini)

## License

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> Copyright (c) 2019 Diego Ferracini Bando
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
> 
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.
