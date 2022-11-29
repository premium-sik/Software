#include <stdio.h>
#include "fx_s4_3.h"
int main()
{
	float a,b;
	fscanf(stdin,"%f %f", &a, &b);
	if( a > 9999.999 || a < -9999.999)
	{
		fprintf(stdout,"core dumped\n");
		return 0;
	}
	if (b > 9999.999 || b < -9999.999){
		fprintf(stdout,"core dumped\n");
		return 0;
	}
		
	//float 
	fprintf(stdout,"fxAdd_float : %.3f\n", fxAdd_float(a,b));
	fprintf(stdout,"fxSub_float : %.3f\n", fxSub_float(a,b));
	fprintf(stdout,"fxMul_float : %.3f\n", fxMul_float(toFixed(a), toFixed(b)));
	fprintf(stdout,"fxDiv_float : %.3f\n", fxDiv_float(toFixed(a), toFixed(b)));

	fprintf(stdout,"-----------------------------------------\n");

	//precision
	fprintf(stdout,"fxAdd_precision : %.3f\n", fxAdd_precision(a,b));
	fprintf(stdout,"fxSub_precision : %.3f\n", fxSub_precision(a,b));
	fprintf(stdout,"fxMul_precision : %.3Lf\n", fxMul_precision(toFixed_pre(a), toFixed_pre(b)));
	fprintf(stdout,"fxDiv_precision : %.3lf\n", fxDiv_precision(toFixed_pre(a), toFixed_pre(b)));
	
	fprintf(stdout,"-----------------------------------------\n");
	
	fprintf(stdout,"fxAdd_fair : %.3f\n", fxAdd_fair(a,b));
	fprintf(stdout,"fxSub_fair : %.3f\n", fxSub_fair(a,b));
	fprintf(stdout,"fxMul_fair : %.3Lf\n",fxMul_fair(FIXED_(a),FIXED_(b)));
	fprintf(stdout,"fxDiv_fair : %.3Lf\n", fxDiv_fair(FIXED_(a),FIXED_(b)));

	fprintf(stdout,"-----------------------------------------\n");

	fprintf(stdout,"fxAdd_performance : %.3f\n", fxADD(a,b));
	fprintf(stdout,"fxSub_performance : %.3f\n", fxSUB(a,b));
	fprintf(stdout,"fxMul_performance : %.3Lf\n",fx_Mul_performance(FIXED_(a),FIXED_(b)));
	fprintf(stdout,"fxDiv_performance : %.3lf\n", fx_Div_performance(FIXED_(a))/fx_Div_performance(FIXED_(b)));
	return 0;
}
