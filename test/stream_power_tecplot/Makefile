#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: STREAM_POWER_TECPLOT
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: stream_power
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/stream.o: $(SRC)/stream.c
	$(CC) $(CFLAGS) -c -o $@ $<
stream_power: $(SRC)/stream.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/stream.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./stream_power ./src/*.o
