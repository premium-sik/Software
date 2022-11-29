#include <stdio.h>
#include "fx_s4_3.h"
int main(){
	float i;
	for( i=1; i < 3000000; i++){		
	fprintf(stdout,"fxAdd_float : %.3f\n", fxAdd_float(i,i));
	fprintf(stdout,"fxSub_float : %.3f\n", fxSub_float(i,i));
	fprintf(stdout,"fxMul_float : %.3f\n", fxMul_float(toFixed(i), toFixed(i)));
	fprintf(stdout,"fxDiv_float : %.3f\n", fxDiv_float(toFixed(i), toFixed(i)));
	}

	for(i=1; i < 3000000; i++){
	// precision
	fprintf(stdout,"fxAdd_precision : %.3f\n", fxAdd_precision(i,i));
	fprintf(stdout,"fxSub_precision : %.3f\n", fxSub_precision(i,i));
	fprintf(stdout,"fxMul_precision : %.3Lf\n", fxMul_precision(toFixed_pre(i), toFixed_pre(i)));
	fprintf(stdout,"fxDiv_precision : %.3lf\n", fxDiv_precision(toFixed_pre(i), toFixed_pre(i)));
	}

	for(i=1; i<3000000; i++){
	// fair
	fprintf(stdout,"fxAdd_fair : %.3f\n", fxAdd_fair(i,i));
	fprintf(stdout,"fxSub_fair : %.3f\n", fxSub_fair(i,i));
	fprintf(stdout,"fxMul_fair : %.3Lf\n",fxMul_fair(FIXED_(i),FIXED_(i)));
	fprintf(stdout,"fxDiv_fair : %.3Lf\n", fxDiv_fair(FIXED_(i),FIXED_(i)));
	}

	for(i=1; i<3000000; i++){
	// performance
	fprintf(stdout,"fxAdd_performance : %.3f\n", fxADD(i,i));
	fprintf(stdout,"fxSub_performance : %.3f\n", fxSUB(i,i));
	fprintf(stdout,"fxMul_performance : %.3Lf\n",fx_Mul_performance(FIXED_(i),FIXED_(i)));
	fprintf(stdout,"fxDiv_performance : %.3lf\n", fx_Div_performance(FIXED_(i))/fx_Div_performance(FIXED_(i)));
	}	
}
