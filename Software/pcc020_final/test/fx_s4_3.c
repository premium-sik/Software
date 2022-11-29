// S 4.3
#include "fx_s4_3.h"
#include "fx_head.h"
typedef long FIXED;
typedef long long _FIXED;
#define fxADD(a,b) ((a)+(b))
#define fxSUB(a,b) ((a)-(b))

//float function
FIXED toFixed(double a)
{
	a = a*PLUS_8;
	return a;
}
float fxAdd_float(double a,double b)
{	
	return fxADD(a,b);
}

float fxSub_float(double a, double b)
{
        return fxSUB(a,b);
}

float fxMul_float(FIXED a,FIXED b)
{
	float c = (a*b)*MINUS_8;
        return c*MINUS_8;
}

float fxDiv_float(FIXED a, FIXED b)
{
	float c = a;
	float d = b;
	return c/d;
}

//precision function
_FIXED toFixed_pre(double a)
{
	_FIXED c;	
	c = a*PLUS_16;
	return c;
}
float fxAdd_precision(double a,double b)
{	
	return fxADD(a,b);
}

float fxSub_precision(double a, double b)
{
        return fxSUB(a,b);
}

long double fxMul_precision(_FIXED a,_FIXED b)
{
	long double c = (a*b)*MINUS_16;
        return c*MINUS_16;
}

double fxDiv_precision(_FIXED a, _FIXED b)
{
	double c = a;
	double d = b;
	return c/d;
}

//fair function
float fxAdd_fair(double a,double b)
{	
	return fxADD(a,b);
}

float fxSub_fair(double a, double b)
{
        return fxSUB(a,b);
}

long double fxMul_fair(_FIXED a,_FIXED b)
{
	long double c = (a*b)*MINUS_16;
        return c*MINUS_16;
}

long double fxDiv_fair(_FIXED a, _FIXED b)
{
	double c = a;
	double d = b;
	return c/d;
}


//performance function only calculated by ppc

