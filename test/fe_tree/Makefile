#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: FE_TREE
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: fe_tree
$(SRC)/execute_test.o: $(SRC)/execute_test.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/shiftamt.o: $(SRC)/shiftamt.c
	$(CC) $(CFLAGS) -c -o $@ $<
$(SRC)/fe_tree.o: $(SRC)/fe_tree.c
fe_tree: $(SRC)/fe_tree.o $(SRC)/shiftamt.o $(SRC)/execute_test.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/fe_tree.o $(SRC)/shiftamt.o $(SRC)/execute_test.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./fe_tree ./src/*.o
