#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: DYN_CMC
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: dyn_cmc
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/dyn_cmc.o: $(SRC)/dyn_cmc.c
	$(CC) $(CFLAGS) -c -o $@ $<
dyn_cmc: $(SRC)/dyn_cmc.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/dyn_cmc.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./dyn_cmc ./src/*.o
