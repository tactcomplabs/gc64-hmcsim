#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: MUTEX
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: mutex
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/mutex.o: $(SRC)/mutex.c
mutex: $(SRC)/mutex.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/mutex.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./mutex ./src/*.o
