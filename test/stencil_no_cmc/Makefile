#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: STENCIL_READ 
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: stencil_no_cmc
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/stencil_no_cmc.o: $(SRC)/stencil_no_cmc.c
	$(CC) $(CFLAGS) -c -o $@ $<
stencil_no_cmc: $(SRC)/stencil_no_cmc.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/stencil_no_cmc.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./stencil_no_cmc ./src/*.o
