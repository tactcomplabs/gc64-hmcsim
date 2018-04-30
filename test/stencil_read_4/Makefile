#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: STENCIL_READ 
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: stencil_read_4
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/stencil_read_4.o: $(SRC)/stencil_read_4.c
	$(CC) $(CFLAGS) -c -o $@ $<
stencil_read_4: $(SRC)/stencil_read_4.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/stencil_read_4.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./stencil_read_4 ./src/*.o
