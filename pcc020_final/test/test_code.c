// S 4.3
#define FX_2_PLUS_3 (1<<3)
#include "fx_head.h"
typedef char FIXED;

FIXED toFixed(FIXED a)
{
	return a<<3;
}

FIXED Fixed_add(FIXED a,FIXED b)
{
       	return ((a+b));
}

FIXED Fixed_minus(FIXED a, FIXED b)
{
	return ((a-b) >> 3);
}

FIXED Fixed_mul(FIXED a,FIXED b)
{
	return ((a * b) >> 3);
}

FIXED Fixed_div(FIXED a, FIXED b)
{return ((a<<3) / b);}


