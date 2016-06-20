#!/bin/sh
#
# HMCSIM TEST APPLICATIONS 
#
# TEST: SST_INIT
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: sst_init
sst_init:
	$(CC) $(CFLAGS) -o $@ $(SRC)/sst_init.c $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./sst_init ./src/*.o
