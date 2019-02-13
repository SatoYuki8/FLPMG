#ifndef GEN_H
#define GEN_H


#include <stdio.h>
#include <stdlib.h>
#include "../generator_head.h"


#define GET_VAR_NAME(VAR) (#VAR)//変数VARの変数名を文字列として取得

int DecToBi(int dec, int dig, FILE *fp);
//char ZeroStr(int N, char in[]);
//char OneStr(int N, char in[]);

int Manage_FPAdder_Generator(int argc, char **argv);

FILE *fpadd_arg_check(int argc, char **argv, int ef[], flags_t flag, char *top_module_name);
int FPAdder_Generator(int exp, int frac, int wodth, FILE *fp, flags_t flag, char *module_name);
int wrapper_adder(int wodth, FILE *fp, char *module_name);
int fpadder_step_measurement(int exp, int frac, int width, FILE *fp, flags_t flag, char *top_module_name);


int fpadd_exp_comparison_declaration(FILE *fp, int exp, int width, char *sel, char *sel_or_reg);
int fpadd_preshift_declaration(FILE *fp, int exp, int frac_bit, char *sel, char *sel_or_reg);
int fpadd_mantissa_add_declaration(FILE *fp, int exp, int frac_bit, char *sel, char *sel_or_reg);
int fpadd_leadingzeroshift_declaration(FILE *fp, int exp, int frac, char *sel_or_reg);
int fpadd_round_and_finish_declaration(FILE *fp, int frac, int width, char *sel, char *sel_or_reg);

int fpadd_exp_comparison(FILE *fp, int exp, int frac, int width, flags_t flag);
int fpadd_preshift(FILE *fp, int frac, int width, flags_t flag);
int fpadd_mantissa_add(FILE *fp, int frac_bit, flags_t flag);
int fpadd_leadingzeroshift(FILE *fp, int frac_bit, flags_t flag);
int fpadd_round(FILE *fp, int frac, int width, flags_t flag);


#endif
