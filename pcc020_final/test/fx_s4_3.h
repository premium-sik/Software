typedef long FIXED;
typedef long long _FIXED;
extern FIXED toFixed(double a);
extern _FIXED toFixed_pre(double a);
#include "fx_head.h"
#include <math.h>
extern float fxAdd_float(double a, double b), fxAdd_precision(double a, double b), fxAdd_fair(double a, double b);
extern float fxSub_float(double a, double b), fxSub_precision(double a, double b), fxSub_fair(double a, double b);
extern float fxMul_float(FIXED a, FIXED b);
extern long double fxMul_precision(_FIXED a, _FIXED b), fxMul_fair(_FIXED a, _FIXED b),fxDiv_fair(_FIXED a, _FIXED b);
extern float fxDiv_float(FIXED a, FIXED b); 
extern double fxDiv_precision(_FIXED a, _FIXED b);






