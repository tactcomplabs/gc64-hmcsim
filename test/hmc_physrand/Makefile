#!/bin/sh
#
# HMCSIM TEST APPLICATIONS 
#
# TEST: PHYSRAND
# ---------------------------------------

include ../Makefile.inc

SRC=./src

OBJS=$(SRC)/genrand.o $(SRC)/execute_test.o $(SRC)/shiftamt.o

all: physrand
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/genrand.o: $(SRC)/genrand.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
physrand: $(SRC)/genrand.o $(SRC)/execute_test.o $(SRC)/shiftamt.o
	$(CC) $(CFLAGS) -c -o $(SRC)/physrand_main.o $(SRC)/physrand_main.c
	$(CC) $(CFLAGS) -o $@ $(SRC)/physrand_main.o $(OBJS) $(LDFLAGS) $(LDLIBS)
clean: 
	rm -Rf ./physrand ./src/*.o
