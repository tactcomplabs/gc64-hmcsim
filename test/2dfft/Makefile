#!/bin/sh
#
# HMCSIM TEST APPLICATIONS 
#
# TEST: 2DFFT
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: 2dfft
2dfft:
	$(CC) $(CFLAGS) -o $@ $(SRC)/2dfft.c $(LDFLAGS) $(LDLIBS)
clean: 
	rm -Rf ./2dfft ./src/*.o
