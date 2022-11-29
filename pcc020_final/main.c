#include <stdio.h>
#include "fx_s4_3.h"
int main()
{
	float a,b;
	int c;
	fscanf(stdin,"%f %f", &a, &b);
	fprintf(stdout,"1 is float option\n");
	fprintf(stdout,"2 is precision option\n");
	fprintf(stdout,"3 is fair option\n");
	fprintf(stdout,"4 is performance option\n");
	fprintf(stdout,"Calculate with the selected option:\n");
	fscanf(stdin,"%d", &c);
	if( a > 127 || a < -128)
	{
		return 0;
	}
	if (b > 127 || b < -128)
	{
		return 0;
	}
		
	if(c == FX_OP_FLOAT)
	{
	// float
	if(fxAdd_float(a,b)<15.875 || fxAdd_float(a,b)>-16.875)
	{ 
	fprintf(stdout,"fxAdd_float : %.3f\n", fxAdd_float(a,b));
	}
	if(fxSub_float(a,b)<15.875 || fxSub_float(a,b)>-16.875){
	fprintf(stdout,"fxSub_float : %.3f\n", fxSub_float(a,b));
	}
	if(fxMul_float(a,b)<15.875 || fxMul_float(a,b)>-16.875){
	fprintf(stdout,"fxMul_float : %.3f\n", fxMul_float(toFixed(a), toFixed(b)));
	}
	if(fxDiv_float(a,b)<15.875 || fxDiv_float(a,b)>-16.875){
	fprintf(stdout,"fxDiv_float : %.3f\n", fxDiv_float(toFixed(a), toFixed(b)));
	}
	}

	if(c == FX_OP_PRECISION)
	{	
	// precision
	if(fxAdd_precision(a,b)<15.875 || fxAdd_precision(a,b)>-15.875)
	{
	fprintf(stdout,"fxAdd_precision : %.3f\n", fxAdd_precision(a,b));
	}
	if(fxSub_precision(a,b)<15.875 || fxSub_precision(a,b)>-15.875)
	{
	fprintf(stdout,"fxSub_precision : %.3f\n", fxSub_precision(a,b));
	}
	if(fxMul_precision(a,b)<15.875 || fxMul_precision(a,b)>-15.875)
	{
	fprintf(stdout,"fxMul_precision : %.3Lf\n", fxMul_precision(toFixed_pre(a), toFixed_pre(b)));
	}	
	if(fxDiv_precision(a,b)<15.875 || fxDiv_precision(a,b)>-15.875){
	fprintf(stdout,"fxDiv_precision : %.3lf\n", fxDiv_precision(toFixed_pre(a), toFixed_pre(b)));
	}
	}
	
	if(c == FX_OP_FAIR)
	{
	// fair
	if(fxAdd_fair(a,b)<15.875 || fxAdd_fair(a,b)>-16.875)
	{
	fprintf(stdout,"fxAdd_fair : %.3f\n", fxAdd_fair(a,b));
	}
	if(fxSub_fair(a,b)<15.875 || fxSub_fair(a,b)>-16.875)
	{
	fprintf(stdout,"fxSub_fair : %.3f\n", fxSub_fair(a,b));
	}
	if(fxMul_fair(a,b)<15.875 || fxMul_fair(a,b)>-16.875)
	{
	fprintf(stdout,"fxMul_fair : %.3Lf\n",fxMul_fair(FIXED_(a),FIXED_(b)));
	}
	if(fxDiv_fair(a,b)<15.875 || fxDiv_fair(a,b)>-16.875)
	{
	fprintf(stdout,"fxDiv_fair : %.3Lf\n", fxDiv_fair(FIXED_(a),FIXED_(b)));
	}
	}

	if(c == FX_OP_PERFORMANCE)
	{
	// performance
	if(fxADD(a,b)<15.875 || fxADD(a,b)>-16.875)
	{
	fprintf(stdout,"fxAdd_performance : %.3f\n", fxADD(a,b));
	}
	if(fxSUB(a,b)<15.875 || fxSUB(a,b)>-16.875)
	{
	fprintf(stdout,"fxSub_performance : %.3f\n", fxSUB(a,b));
	}
	if(fx_Mul_performance(FIXED_(a),FIXED_(b))<15.875 || fx_Mul_performance(FIXED_(a),(b))>-16.875)
	{
	fprintf(stdout,"fxMul_performance : %.3Lf\n",fx_Mul_performance(FIXED_(a),FIXED_(b)));
	}
	if(fx_Div_performance(FIXED_(a))/fx_Div_performance(FIXED_(b))<15.875 || fx_Div_performance(FIXED_(a))/fx_Div_performance(FIXED_(b))>-16.875)
	{
	fprintf(stdout,"fxDiv_performance : %.3lf\n", fx_Div_performance(FIXED_(a))/fx_Div_performance(FIXED_(b)));
	}
	}
	return 0;
}
