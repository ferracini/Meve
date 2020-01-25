#include "mevemath.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

num_t Add(
	const num_t* n)
{
	return n[0] + n[1];
}

num_t UnAdd(
	const num_t* n)
{
	return n[0];
}

num_t Sub(
	const num_t* n)
{
	return n[0] - n[1];
}

num_t UnSub(
	const num_t* n)
{
	return -n[0];
}

num_t Mult(
	const num_t* n)
{
	return n[0] * n[1];
}

num_t Div(
	const num_t* n)
{
	return n[0] / n[1];
}

num_t Pow(
	const num_t* n)
{	
	return powl(n[0], n[1]);
}

num_t Mod(
	const num_t* arg)
{
	return fmodl(arg[0], arg[1]);
}

num_t Choose(
	const num_t* n)
{
	num_t res;
	const num_t lim = n[0] - n[1] > n[1] ? n[1] : n[0] - n[1];

	if (n[1] > n[0] || n[0] < 0 || n[1] < 0)
	{
		return 0;
	}
	else if (n[0] == n[1] || n[1] == 0)
	{
		return 1;
	}
	
	res = n[0];
	for (num_t i = 1; i < lim; i++)
	{
		res *= n[0] - i;
	}

	return res / Factorial(&lim);
}

num_t Exp(
	const num_t* arg)
{
	return expl(arg[0]);
}

num_t Log(
	const num_t* arg)
{
	return logl(arg[0]);
}

num_t Log10(
	const num_t* arg)
{
	return log10l(arg[0]);
}

num_t Sin(
	const num_t* arg)
{
	return sinl(arg[0]);
}

num_t Cos(
	const num_t* arg)
{
	return cosl(arg[0]);
}

num_t Tan(
	const num_t* arg)
{
	return tanl(arg[0]);
}

num_t ArcSin(
	const num_t* arg)
{
	return asinl(arg[0]);
}

num_t ArcCos(
	const num_t* arg)
{
	return acosl(arg[0]);
}

num_t ArcTan(
	const num_t* arg)
{
	return atanl(arg[0]);
}

num_t Sinh(
	const num_t* arg)
{
	return sinhl(arg[0]);
}

num_t Cosh(
	const num_t* arg)
{
	return coshl(arg[0]);
}

num_t Tanh(
	const num_t* arg)
{
	return tanhl(arg[0]);
}

num_t ArcSinh(
	const num_t* arg)
{
	return asinhl(arg[0]);
}

num_t ArcCosh(
	const num_t* arg)
{
	return acoshl(arg[0]);
}

num_t ArcTanh(
	const num_t* arg)
{
	return atanhl(arg[0]);
}

num_t Sqrt(
	const num_t* arg)
{
	return sqrtl(arg[0]);
}

num_t Abs(
	const num_t* arg)
{
	return fabsl(arg[0]);
}

num_t Factorial(
	const num_t* arg)
{
	num_t res;

	if (*arg < 0 || (ceill(*arg) != *arg))
	{		
		return 0;
	}

	if (*arg == 0 || *arg == 1)
	{
		return 1;
	}

	res = 2.0L;
	for (num_t i = 3.0L; i <= *arg; i++)
	{
		res *= i;
	}
	return res;
}

num_t Min(
	const num_t* arg)
{
	return fminl(arg[0], arg[1]);
}

num_t Max(
	const num_t* arg)
{
	return fmaxl(arg[0], arg[1]);
}