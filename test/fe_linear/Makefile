#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: FE_LINEAR
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: fe_linear
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/fe_linear.o: $(SRC)/fe_linear.c
fe_linear: $(SRC)/fe_linear.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/fe_linear.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./fe_linear ./src/*.o
