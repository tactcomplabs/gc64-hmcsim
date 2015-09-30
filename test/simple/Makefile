#!/bin/sh
#
# HMCSIM TEST APPLICATIONS 
#
# TEST: SIMPLE
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: simple
simple:
	$(CC) $(CFLAGS) -o $@ $(SRC)/simple.c $(LDFLAGS) $(LDLIBS)
clean: 
	rm -Rf ./simple ./src/*.o
