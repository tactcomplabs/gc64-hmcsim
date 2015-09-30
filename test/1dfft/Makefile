#!/bin/sh
#
# HMCSIM TEST APPLICATIONS 
#
# TEST: 1DFFT
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: 1dfft
1dfft:
	$(CC) $(CFLAGS) -o $@ $(SRC)/1dfft.c $(LDFLAGS) $(LDLIBS)
clean: 
	rm -Rf ./1dfft ./src/*.o
