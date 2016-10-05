#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: FE_TREE
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: mutex_tree
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/mutex_tree.o: $(SRC)/mutex_tree.c
mutex_tree: $(SRC)/mutex_tree.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/mutex_tree.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./mutex_tree ./src/*.o
