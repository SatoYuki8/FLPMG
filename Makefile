GCC=gcc
BIN=bin/flpmg
OPT=-Wall -lm -O2
SRCDST=src/
ADDDST=$(SRCDST)src_add/
MULDST=$(SRCDST)src_mul/

ADDOBJ=$(ADDDST)fpadd_arg_check.o $(ADDDST)fpadd_generator.o $(ADDDST)fpadd_step_measurement.o
MULOBJ=$(MULDST)fpmult_step_measurement.o $(MULDST)fpmult_arg_check.o $(MULDST)fpmult_generator.o

ADDHEAD=$(ADDDST)add_gen.h
MULHEAD=$(MULDST)mul_gen.h

MAINOBJ=$(SRCDST)main.o $(SRCDST)sub_module.o

###########################################################

all: bin/ comp

comp:	$(MAINOBJ) $(ADDOBJ) $(MULOBJ) 
	$(GCC) -o $(BIN) $^  $(OPT)

$(SRCDST)main.o:	$(SRCDST)main.c  $(SRCDST)generator_head.h
$(SRCDST)main.c:
	$(GCC) -c $@ $(OPT)

$(SRCDST)sub_module.o:	$(SRCDST)sub_module.c  $(SRCDST)generator_head.h
$(SRCDST)sub_module.c:
	$(GCC) -c $@ $(OPT)

bin:
	mkdir bin/

###########################################################

$(ADDDST)fpadd_arg_check.o:	$(ADDDST)fpadd_arg_check.c $(ADDHEAD)
$(ADDDST)fpadd_arg_check.c:
	$(GCC) -c $@ $(OPT)

$(ADDDST)fpadd_generator.o:	$(ADDDST)fpadd_generator.c $(ADDHEAD)
$(ADDDST)fpadd_generator.c:
	$(GCC) -c $@ $(OPT)

#$(ADDDST)main.o:	$(ADDDST)main.c $(ADDHEAD)
#$(ADDDST)main.c:
#	$(GCC) -c $@ $(OPT)

#$(ADDDST)generate_function.o:	$(ADDDST)generate_function.c $(ADDHEAD)
#$(ADDDST)generate_function.c:
#	$(GCC) -c $@ $(OPT)

$(ADDDST)fpadd_step_measurement.o:	$(ADDDST)fpadd_step_measurement.c $(ADDHEAD)
$(ADDDST)fpadd_step_measurement.c:
	$(GCC) -c $@ $(OPT)

###########################################################

$(MULDST)fpmult_step_measurement.o:	$(MULDST)fpmult_step_measurement.c $(MULHEAD)
$(MULDST)fpmult_step_measurement.c:
	$(GCC) -c $@ $(OPT)

$(MULDST)fpmult_arg_check.o: $(MULDST)fpmult_arg_check.c $(MULHEAD)
$(MULDST)fpmult_arg_check.c:
	$(GCC) -c $@ $(OPT)

$(MULDST)fpmult_generator.o:	$(MULDST)fpmult_generator.c $(MULHEAD)
$(MULDST)fpmult_generator.c:
	$(GCC) -c $@ $(OPT)

#$(MULDST)fpmult_main.o: $(MULDST)fpmult_main.c $(MULHEAD)
#$(MULDST)fpmult_main.c:
#	$(GCC) -c $@ $(OPT)

###########################################################

clean:
	rm -rf *~ *.sfl $(SRCDST)*~ $(ADDDST)*~ $(MULDST)*~

distclean:
	rm -rf *~ *.out $(SRCDST)*.o $(SRCDST)*~ $(ADDDST)*.o $(ADDDST)*~ $(MULDST)*.o $(MULDST)*~ *.sfl bin/*

