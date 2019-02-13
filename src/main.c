#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generator_head.h"

#define FPADD 11
#define FPMULT 22


void first_message(){
  printf("\n"
	 "\x1b[31m"      //red
	 "***************************************************************************\n"
	 "\x1b[39m"      //return
	 "Generator of Floating Point Adder written in SFL\n\n"
	 "\x1b[35m"	  //magenta
	 "------------------------Floating-Point Operator-------------------------\n"
	 "\x1b[31m"      //red
	 "FPAdd : \n"
	 "\x1b[39m"      //return
	 "\x1b[36m"      //cian
	 "            exp=<n> "
	 "\x1b[39m"      //return
	 ": n bits exponent\n"
	 "\x1b[36m"      //cian
	 "           frac=<m> "
	 "\x1b[39m"      //return
	 ": m bits mantissa\n"
	 "  filename=<string> : SFL is output in the <string> (default:FPAdd_n_m_<n+m>.sfl)\n"
	 "           pipeline : Generate 5steps pipeline version\n"
	 "            wrapper : Wrapper module for delay measurement is created\n"
	 "\n"
	 "\x1b[31m"      //red
	 "FPMult: \n"
	 "\x1b[36m"      //cian
	 "            exp=<n> "
	 "\x1b[39m"      //return
	 ": n bits exponent\n"
	 "\x1b[36m"      //cian
	 "           frac=<m> "
	 "\x1b[39m"      //return
	 ": m bits mantissa\n"
	 "  filename=<string> : SFL is output in the <string> (default:FPMult_n_m_<n+m>.sfl)\n"
	 "           pipeline : Generate 4steps pipeline version\n"
	 "            wrapper : Wrapper module for delay measurement is created\n"
	 "             DSP=no : Using Mutiplier Module insted of Operator.\n"
	 "\n"
	 "\x1b[31m"      //red	 
	 "***************************************************************************\n"	  
	 "\x1b[39m"      //return
	 "\n"
	 );
	 
}


void init_flags(flags_t flag){
  flag->wrapper_flag = 0;
  flag->fpadd_flag = 0;
  flag->fpmulti_flag = 0;
  flag->stdout_flag = 0;
  flag->filename_flag = 0;
  flag->non_DSP_flag = 0;
  flag->pipeline_flag = 0;
  flag->step1_flag = 0;
  flag->step2_flag = 0;
  flag->step3_flag = 0;
  flag->step4_flag = 0;
  flag->step5_flag = 0;
}

int top_arg_check(int ac, char **argv){
  int alm = 0;
  int i;
  
  for (i=0; i<ac; i++){
    if (!strcmp(argv[i], "fpadder")) alm = FPADD;
    if (!strcmp(argv[i], "Fpadder")) alm = FPADD;
    if (!strcmp(argv[i], "FPadder")) alm = FPADD;
    if (!strcmp(argv[i], "FPAdder")) alm = FPADD;
    if (!strcmp(argv[i], "FPADDER")) alm = FPADD;
    if (!strcmp(argv[i], "fpadd")) alm = FPADD;
    if (!strcmp(argv[i], "Fpadd")) alm = FPADD;
    if (!strcmp(argv[i], "FPadd")) alm = FPADD;
    if (!strcmp(argv[i], "FPAdd")) alm = FPADD;
    if (!strcmp(argv[i], "FPADD")) alm = FPADD;

    if (!strcmp(argv[i], "fpmulti")) alm = FPMULT;
    if (!strcmp(argv[i], "Fpmulti")) alm = FPMULT;
    if (!strcmp(argv[i], "FPmulti")) alm = FPMULT;
    if (!strcmp(argv[i], "FPMulti")) alm = FPMULT;
    if (!strcmp(argv[i], "FPMULTI")) alm = FPMULT;
    if (!strcmp(argv[i], "fpmult")) alm = FPMULT;
    if (!strcmp(argv[i], "Fpmult")) alm = FPMULT;
    if (!strcmp(argv[i], "FPmult")) alm = FPMULT;
    if (!strcmp(argv[i], "FPMult")) alm = FPMULT;
    if (!strcmp(argv[i], "FPMULT")) alm = FPMULT;    
  }
  
  return alm;
}

int main(int ac, char **av){
  int alm_flag=0;

  alm_flag = top_arg_check(ac, av);

  if (alm_flag == FPADD){
    Manage_FPAdder_Generator(ac, av);
  }else if (alm_flag == FPMULT){
    Manage_FPMult_Generator(ac, av);
  }else {
    first_message();
    return 0;
  }
  
  return 1;
}
