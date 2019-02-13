#ifndef ALL_H
#define ALL_H

#define P_IF if(flag->pipeline_flag == 1){
#define P_ELSE }else{
#define P_END }

/* exponation fraction bit_width */
typedef struct exp_frac_width{
  int exp;
  int frac;
  int width;  //exp + frac + sign
} _efw_t;

typedef _efw_t efw_t[1];


/* Flag */
typedef struct flags{
  int wrapper_flag;
  int fpadd_flag;
  int fpmulti_flag;
  int stdout_flag;
  int filename_flag;
  int non_DSP_flag;
  int pipeline_flag;
  int step1_flag;
  int step2_flag;
  int step3_flag;
  int step4_flag;
  int step5_flag;
} _flags_t;

typedef _flags_t flags_t[1];


void init_flags(flags_t flag);
//main.c
int Manage_FPAdder_Generator(int argc, char **argv);
int Manage_FPMult_Generator(int argc, char **argv);

//sub_module.c
char ZeroStr(int N, char in[]);
int DecToBi(int dec, int dig, FILE *fp);
int BarrelShiftDec(int exp, int frac, int width, FILE *fp);
int BarrelShift(int exp, int frac, int width, FILE *fp);
int LeadingZeroShiftDec(int exp, int frac, int width, FILE *fp);
int LeadingZeroShift(int exp, int frac, int width, FILE *fp);
int MantissaAdderDec(int width, FILE *fp);
int MantissaAdder(int width, FILE *fp);
int IncreaseFrac(FILE *fp, const int frac, char *name);
int Long_Division(FILE *fp, const int frac, char *name);
int Multiplier_Generator(FILE *fp, const int frac, flags_t flag, char *name);


#endif
