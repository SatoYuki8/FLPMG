#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "generator_head.h"

/* 0b00...0の形の文字列をつくる */
char ZeroStr(int N, char in[]){
  in[0] = '0';
  in[1] = 'b';

  int i;
  for(i=2; i < N+2; i++){
    in[i] = '0';
  }
  
  return 0;
}



/* DecToBi function changes Decimal Num to Binary Num.  */
/* dec -> 2進数に変換したい10進数, dig -> 桁数            */
int DecToBi(int dec, int dig, FILE *fp){
  //char buf[dig+1];
  int *buf;
  int i;
  int num = dec;

  if (dig <= 0) return 0;

  buf = (int*)calloc(dig, sizeof(int));

  for(i=0; i<dig; i++){
    if(i != 0 && num == 0) break;
    if(num % 2 == 0) buf[(dig-1)-i] = 0;
    else buf[(dig-1)-i] = 1;

    num = num / 2;
  }

  fprintf(fp, "0b");
  for (i=0; i<dig; i++){
    fprintf(fp, "%d", buf[i]);
  }

  free(buf);
  return 0;
}

int BarrelShiftDec(int exp, int frac, int width, FILE *fp){

  fprintf(fp,
	  "declare BarrelShift{\n"
	  "input a<%d>;\n"
	  "input amount<%d>;\n"
	  "output f<%d>;\n"
	  "instrin do_in;\n"
	  "instr_arg do_in(a, amount);\n"
	  "}\n\n",
	  width, exp, width
	  );
  
  return 0;
}


int BarrelShift(int exp, int frac, int width, FILE *fp){

  int shamt = (int)log2(width-1) + 1;
  //int shamt = (int)log2(frac+5) + 1;

  if (shamt > exp) shamt = exp;
  
  fprintf(fp,
	  "module BarrelShift{\n"
	  "input a<%d>;\n"
	  "input amount<%d>;\n"
	  "output f<%d>;\n"
	  "instrin do_in;\n",
	  width, exp, width
	  );

  int i;
  for (i=0;i<shamt;i++){
    fprintf(fp, "sel t%d<%d>;\n", i, width);
  }

  for (i=0;i<shamt;i++){
    fprintf(fp, "sel s%d<%d>;\n", i, (int)pow(2,i+1));
  }

  fprintf(fp, "instruct do_in par{\n");

  int ulp = width - (frac + 3);
  char aort;
  for (i=0;i<shamt;i++){
    if (i==0){
      aort = 'a';
      fprintf(fp,
	      "alt{\n"
	      "(amount<%d>): par{\n"
	      "t%d = ",
	      i, i
	      );
      DecToBi(0, (int)pow(2,i), fp);  
      fprintf(fp,
	      " || %c<%d:1>;\n"
	      "s%d = %c<%d> || 0b0;\n"
	      "}\n",
	      aort, width-1,
	      i, aort, ulp
	      );
      fprintf(fp,
	      "else: par{\n"
	      "t%d = %c;\n"
	      "s%d = 0b00;\n"
	      "}\n"
	      "}\n",
	      i, aort,
	      i
	      );
    }else{
      aort = 't';
      
      fprintf(fp,
	      "alt{\n"
	      "(amount<%d>): par{\n"
	      "t%d = ",
	      i, i
	      );
      DecToBi(0, (int)pow(2,i), fp);

      /********************************************************/
      if((ulp+((int)pow(2,i)-1)) >= width){
	fprintf(fp,
		" || %c%d<%d:%d>;\n"
		"s%d = %d#(0b0) || %c%d<%d:%d> || s%d;\n"
		"}\n",
		aort, i-1, width-1, (int)pow(2,i),
		i, ulp+((int)pow(2,i)-1) - (width-1), aort, i-1, width - 1, ulp, i-1
		);
      }else{
	fprintf(fp,
		" || %c%d<%d:%d>;\n"
		"s%d = %c%d<%d:%d> || s%d;\n"
		"}\n",
		aort, i-1, width-1, (int)pow(2,i),
		i, aort, i-1, ulp+((int)pow(2,i) - 1), ulp, i-1
		);
      }
      /********************************************************/
      
      fprintf(fp,
	      "else: par{\n"
	      "t%d = %c%d;\n"
	      "s%d = s%d||",
	      i, aort, i-1,
	      i, i-1
	      );
      DecToBi(0, (int)pow(2,i), fp);
      fprintf(fp,
	      ";\n"
	      "}\n"
	      "}\n"
	      );
    }
  }

  char *zero_full;
  zero_full = (char *)malloc(width*2);
  ZeroStr(width, zero_full);
  if ((exp-1) < i) {
    fprintf(fp,
	    "f = t%d<%d:1> || /|(s%d<%d:0>);\n"
	    "}\n"
	    "}\n",
	    shamt-1, width-1, shamt-1, (int)pow(2,shamt)-3
	    );
  }else{
    fprintf(fp,
	    "alt{\n"
	    "(/|amount<%d:%d>): f = %s;\n"
	    "else: f = t%d<%d:1> || /|(s%d<%d:0>);\n"
	    "}\n"
	    "}\n"
	    "}\n",
	    exp-1, i, zero_full,
	    shamt-1, width-1, shamt-1, (int)pow(2,shamt)-3
	    );
  }
  
  free(zero_full);
  printf("\n");

  printf("Generate %d bits BarrelShiter\n", width);
  
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int LeadingZeroShiftDec(int exp, int frac, int width, FILE *fp){
  fprintf(fp,
	  "declare LeadingZeroShift{\n"
	  "input a<%d>;\n"
	  "output amount<%d>;\n"
	  "output f<%d>;\n"
	  "instrin do_in;\n"
	  "instr_arg do_in(a);\n"
	  "}\n\n",
	  width, exp, width
	  );
  return 0;
}

int LeadingZeroShift(int exp, int frac, int width, FILE *fp){

  int shamt = (int)log2(width-1) + 1;

  fprintf(fp,
	  "circuit LeadingZeroShift{\n"
	  "input a<%d>;\n"
	  "output amount<%d>;\n"
	  "output f<%d>;\n"
	  "instrin do_in;\n",
	  width, exp, width
	  );

  int i;
  for (i=0;i<shamt;i++){
    fprintf(fp, "sel t%d<%d>;\n", i, width);
  }

  for (i=0;i<shamt;i++){
   fprintf(fp, "sel a%d;\n", i);
  }

  fprintf(fp, "instruct do_in par{\n");

  for (i=0;i<shamt;i++){
    fputs("alt{\n", fp);

    if (i == 0){
      fprintf(fp,
	      "(a<%d:%d> == (%d#0b0)): par{\n"
	      "a%d = 0b1; \n"
	      "t%d = a<%d:0>||(%d#0b0);\n"
	      "}\n"
	      "else: par{\n"
	      "a%d = 0b0; \n"
	      "t%d = a;\n"
	      "}\n",
	      width-1, width-(int)pow(2,shamt-i-1), (int)pow(2,shamt-i-1),
	      shamt-i-1,
	      i, width-(int)pow(2,shamt-i-1)-1 ,(int)pow(2,shamt-i-1),	   
	      shamt-i-1,
	      i
	      );
    }else{
      fprintf(fp,
	      "(t%d<%d:%d> == (%d#0b0)): par{ \n"
	      "a%d = 0b1; \n"
	      "t%d = t%d<%d:0>||(%d#0b0); \n"
	      "}\n"
	      "else: par{ \n"
	      "a%d = 0b0; \n"
	      "t%d = t%d; \n"
	      "}\n",
	      i-1 ,width-1, width-(int)pow(2,shamt-i-1), (int)pow(2,shamt-i-1),
	      shamt-i-1,
	      i, i-1, width-(int)pow(2,shamt-i-1)-1 ,(int)pow(2,shamt-i-1),	   
	      shamt-i-1,
	      i, i-1
	      );
    }
  
    fputs("}\n", fp);
  }
    //free(zero);

    int j;
    if (shamt > exp){     //指数部の幅で表せられる値がシフト量より小さい時
    fprintf(fp,
	    "alt{\n"
	    "("
	    );
    for(j=shamt-1; j>exp-1; j--){
      fprintf(fp, "a%d", j);
      if(j != exp) fputs("|", fp);
    }
    fprintf(fp,
	    "): amount = %d#0b1;\n"
	    "else: amount = ",
	    exp
	    );
    for (j=exp-1; j>=0; j--){
      if (j==exp-1) fprintf(fp, "a%d", j);
      else fprintf(fp, "||a%d", j);
    }
    fputs(";\n"
	  "}\n", fp);
  }else{                //指数部で表せられる値がシフト量以上の時

      if((exp-shamt)!= 0){
      fprintf(fp,
	      "amount = (%d#0b0)",
	      exp-shamt);
      }else{
	fprintf(fp,
		"amount = "
		);
      }
      
      //DecToBi(0, exp-shamt, fp);
    for (j=shamt-1; j>=0; j--){
      if ((exp-shamt <= 0) && (j == shamt-1)) fprintf(fp, "a%d", j);
      else fprintf(fp, "||a%d", j);
    }
    fputs(";\n", fp);
  }
  
  fprintf(fp,
	  "f = t%d;\n"
	  "}\n"
	  "}\n",
	  shamt-1
	  );

  printf("Generate %d bits LeadingZeroShifter\n", width);
  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int MantissaAdderDec(int width, FILE *fp){
  
  fprintf(fp,
	  "declare MantissaAdder{\n"
	  "input s1, s2;\n"
	  "input x1<%d>, x2<%d>;\n"
	  "output r1<%d>;\n"
	  "instrin do_in;\n"
	  "instr_arg do_in(s1, s2, x1, x2);\n"
	  "}\n\n",
	  width,  width, width
	  );

  return 0;
}

int MantissaAdder(int width, FILE *fp){

  fprintf(fp,
	  "circuit MantissaAdder{\n"
	  "input s1, s2;\n"
	  "input x1<%d>, x2<%d>;\n"
	  "output r1<%d>;\n"
	  "instrin do_in;\n\n"
	  "instruct do_in r1 = ((%d#s1 @ x1) + s1) + ((%d#s2 @ x2) + s2);\n"
	  "}\n\n",
	  width,  width, width, width, width
	  );

  return 0;
}



int IncreaseFrac(FILE *fp, const int frac, char *name){
  sprintf(name, "IncreaseFrac_%d", frac);
  //Header
  fprintf(fp,
	  "declare %s{\n"
	  "input cin;\n"
	  "input in<%d>;\n"
	  "output out<%d>;\n"
	  "output p;\n"
	  "instrin do_in;\n"
	  "instr_arg do_in(cin, in);\n"
	  "}\n\n",
	  name,
	  frac, frac
	  );

  //Module
  fprintf(fp,
	  "circuit %s{\n"
	  "input cin;\n"
	  "input in<%d>;\n"
	  "output out<%d>;\n"
	  "output p;\n"
	  "instrin do_in;\n"
	  "sel tmp<%d>;\n\n"
	  "instruct do_in par{\n"
	  "tmp = (0b0||in) + cin;\n"
	  "out = tmp<%d:0>;\n"
	  "p = tmp<%d>;\n"
	  "}\n}\n\n",
	  name,
	  frac, frac, frac+1,
	  frac-1, frac
	  );

  printf("Generate %d bits IncreaseFrac\n", frac);
  
  return 1;
}//end IncFrac_Gnerator

/* 筆算アルゴリズムによる乗算    */
/* SECONDSでのシュミレーション用 */
int Long_Division(FILE *fp, const int frac, char *name){
  int mul_frac = frac + 1;

  fprintf(fp,
	  "circuit %s{\n"
	  "input       in1<%d> ;\n"
	  "input       in2<%d> ;\n"
	  "output      out<%d> ;\n"
	  "instrin     do_in;",
	  name,
	  mul_frac, mul_frac, mul_frac*2
	  );

  //sel declaration
  int i;
  for(i=0;i<mul_frac;i++){
    fprintf(fp, "sel tmp%d<%d>;\n", i, mul_frac*2);
  }

  fprintf(fp, "\ninstruct do_in par{\n");
  //long division
  fprintf(fp, "tmp0 = (%3d#(0b0)||in1           ) & (%d # in2<  0>);\n",mul_frac, mul_frac*2);
  for(i=1;i<mul_frac;i++){
    fprintf(fp,
	    "tmp%d = (%3d#(0b0)||in1||%3d#(0b0)) & (%d # in2<%3d>);\n", i, mul_frac-i, i, mul_frac*2, i);
  }

  fprintf(fp, "out = tmp0");
  for(i=1;i<mul_frac;i++){
    fprintf(fp, " + tmp%d", i);
  }
  fprintf(fp, ";\n}\n}\n\n");
  
  return 1;
}//end Long_Division

int Multiplier_Generator(FILE *fp, const int frac, flags_t flag, char *name){
  int mul_frac = frac + 1;

  sprintf(name, "Multi_%d", mul_frac);

  //Header
  fprintf(fp,
	  "declare %s{\n"
	  "input in1<%d>;\n"
	  "input in2<%d> ;\n"
	  "output out<%d> ;\n"
	  "instrin do_in ;\n"
	  "instr_arg do_in(in1,in2) ;\n"
	  "}\n\n",
	  name,
	  mul_frac,
	  mul_frac,
	  mul_frac*2
	  );

  //Multiplier module
  if(flag->non_DSP_flag == 1){
    Long_Division(fp, frac, name);  //筆算アルゴリズムで乗算
    fprintf(stdout, "Not Using Operator\n");
    return 1;
  }
  
  fprintf(fp,
	  "circuit %s{\n"
	  "input in1<%d>;\n"
	  "input in2<%d> ;\n"
	  "output out<%d> ;\n"
	  "instrin do_in ;\n\n"
	  "instruct do_in out = in1 * in2;\n"
	  "}\n\n",
	  name,
	  mul_frac,
	  mul_frac,
	  mul_frac*2
	  );

  fprintf(stdout, "Using Operator\n");
  return 1;
}//end Multiplier_Generator

