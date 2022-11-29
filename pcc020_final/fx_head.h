#define fxADD(a,b) ((a)+(b))
#define fxSUB(a,b) ((a)-(b))
#define PLUS_8 256
#define MINUS_8 1/256
#define PLUS_16 65536
#define MINUS_16 1/65536
#define FIXED_(a) (((double)(a))*65536)
#define fx_Div_performance(a) ((double)(a))
#define fx_Mul_performance(a,b) (((long double)(a*b)/65536)/65536)

// Operation Preference 
#define	FX_OP_FLOAT 1
#define FX_OP_PRECISION 2
#define FX_OP_FAIR 3
#define FX_OP_PERFORMANCE 4

