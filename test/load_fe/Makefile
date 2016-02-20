#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: LOAD_FE
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: load_fe
load_fe:
	$(CC) $(CFLAGS) -o $@ $(SRC)/load_fe.c $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./load_fe ./src/*.o
