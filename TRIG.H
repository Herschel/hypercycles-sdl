#define COS(X) fx_cos_table[X & 4095]
//(NUMBER_OF_DEGREES-1)]
#define SIN(X) fx_sin_table[X & 4095]

#define NUMBER_OF_DEGREES  4096
#define SHIFT  16
#define SHIFT_MULT  1<<SHIFT;

extern int fx_cos_table[4096];
extern int fx_sin_table[4096];
