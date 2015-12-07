#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: SIMPLE
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: simple_cmc
simple_cmc:
	$(CC) $(CFLAGS) -o $@ $(SRC)/simple.c $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./simple_cmc ./src/*.o
