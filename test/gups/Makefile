#!/bin/sh
#
# HMCSIM TEST APPLICATIONS 
#
# TEST: GUPS
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: gups
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/gups.o: $(SRC)/gups.c
gups: $(SRC)/gups.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/gups.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean: 
	rm -Rf ./gups ./src/*.o
