#ifndef HEAD_DEC
#define HEAD_DEC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../generator_head.h"

#define ERROR 0
#define GET_VAR_NAME(VAR) (#VAR) //変数VARの変数名を文字列として取得



//step_measurement.c
int fpmult_step_measurement(FILE *fp, const efw_t f, flags_t flag, char *module);

//fpmult_main.c
void help_message();
int Wrapper_Generator(FILE *fp, const efw_t f, char *module_name);
int Manage_FPMult_Generator(int argc, char **argv);

FILE *fpmult_arg_check(int argc, char **argv, efw_t f, flags_t flag, char *module_name);
int FPMultiplier_Generator(FILE *fp, const efw_t f, flags_t flag, char *module_name);

int fpmulti_sel_declaration(FILE *fp, int exp, int frac, int width, flags_t flag, char *sel_or_reg);
int fpmulti_signxor_expadd(FILE *fp, int exp, int frac, int width, flags_t flag);
int fpmulti_fracmult(FILE *fp, int exp, int frac, flags_t flag);
int fpmulti_normalize(FILE *fp, int exp, int frac, flags_t flag);
int fpmulti_round(FILE *fp, int frac, int width, flags_t flag);


#endif
