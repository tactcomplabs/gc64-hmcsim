#!/bin/sh
#
# HMCSIM TEST APPLICATIONS 
#
# TEST: GUPS
# ---------------------------------------

include ../Makefile.inc

SRC=./src

LCFLAGS := -I$(SRC)

all: spmv
$(SRC)/readmat.o: $(SRC)/readmat.c
	$(CC) $(CFLAGS) $(LCFLAGS) -c -o $@ $<
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) $(LCFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) $(LCLFLAGS) -c -o $@ $<
$(SRC)/spmv.o: $(SRC)/spmv.c
spmv: $(SRC)/spmv.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(SRC)/readmat.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/spmv.o $(SRC)/shiftamt.o\
			$(SRC)/readmat.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean: 
	rm -Rf ./spmv ./src/*.o
