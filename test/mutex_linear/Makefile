#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: MUTEX_LINEAR
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: mutex_linear
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/mutex_linear.o: $(SRC)/mutex_linear.c
mutex_linear: $(SRC)/mutex_linear.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/mutex_linear.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./mutex_linear ./src/*.o
