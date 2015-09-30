#!/bin/sh
#
# HMCSIM TEST APPLICATIONS 
#
# TEST: STREAM
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: stream
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/stream.o: $(SRC)/stream.c
	$(CC) $(CFLAGS) -c -o $@ $<
stream: $(SRC)/stream.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/stream.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean: 
	rm -Rf ./stream ./src/*.o
