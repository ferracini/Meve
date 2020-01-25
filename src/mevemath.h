#ifndef MEVEMATH_H
#define MEVEMATH_H
#include "mevenum.h"

/* Returns the value of e raised to the arg[0]-th power.				*/
num_t Exp(
	const num_t* arg);

/* Returns the natural logarithm (base-e logarithm) of arg[0].			*/
num_t Log(
	const num_t* arg);

/* Returns the common logarithm of arg[0].								*/
num_t Log10(
	const num_t* arg);

/* Returns the sine of a radian angle arg[0].							*/
num_t Sin(
	const num_t* arg);

/* Returns the cosine of a radian angle  arg[0].						*/
num_t Cos(
	const num_t* arg);

/* Returns the tangent of a radian angle  arg[0].						*/
num_t Tan(
	const num_t* arg);

/* Returns the principal value of the arc sine of arg[0] in radians.	*/
num_t ArcSin(
	const num_t* arg);

/* Returns the principal value of the arc cosine of arg[0] in radians.	*/
num_t ArcCos(
	const num_t* arg);

/* Returns the principal value of the arc tangent of arg[0] in radians. */
num_t ArcTan(
	const num_t* arg);

/* Returns the hyperbolic sine of arg[0].								*/
num_t Sinh(
	const num_t* arg);

/* Returns the hyperbolic cosine of arg[0].								*/
num_t Cosh(
	const num_t* arg);

/* Returns the hyperbolic tangent of arg[0].							*/
num_t Tanh(
	const num_t* arg);

/* Returns the area hyperbolic sine of arg[0].							*/
num_t ArcSinh(
	const num_t* arg);

/* Returns the area hyperbolic cosine of arg[0].						*/
num_t ArcCosh(
	const num_t* arg);

/* Returns the area hyperbolic tangent of arg[0].						*/
num_t ArcTanh(
	const num_t* arg);

/* Returns the square root of arg[0].									*/
num_t Sqrt(
	const num_t* arg);

/* Returns the factorial of arg[0].										*/
num_t Factorial(
	const num_t* arg);

/* Returns the absolute value of arg[0].								*/
num_t Abs(
	const num_t* arg);

/* Returns the largest of arg[0] and arg[1].							*/
num_t Min(
	const num_t* arg);

/* Returns the smallest of arg[0] and arg[1].							*/
num_t Max(
	const num_t* arg);

/* Returns the floating-point remainder of arg[0]/arg[1].				*/
num_t Mod(
	const num_t* arg);

/* Returns the n[1]-combination of n[0] elements.						*/
num_t Choose(
	const num_t* n);

/* Returns the addition of n[0] and n[1].								*/
num_t Add(
	const num_t* n);

/* Returns the n[0].													*/
num_t UnAdd(
	const num_t* n);

/* Returns the subtraction of n[0] by n[1].								*/
num_t Sub(
	const num_t* n);

/* Returns -n[0]. */
num_t UnSub(
	const num_t* n);

/* Returns the multiplication of n[0] by n[1].							*/
num_t Mult(
	const num_t* n);

/* Returns the division of n[0] by n[1].								*/
num_t Div(
	const num_t* n);

/* Returns n[0] raised to the power of n[1].							*/
num_t Pow(
	const num_t* n);
#endif