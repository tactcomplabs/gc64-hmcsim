#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: STENCIL_READ 
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: stencil_read
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/stencil_read.o: $(SRC)/stencil_read.c
	$(CC) $(CFLAGS) -c -o $@ $<
stencil_read: $(SRC)/stencil_read.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/stencil_read.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./stencil_read ./src/*.o
