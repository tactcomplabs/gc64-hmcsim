#!/bin/sh
#
# HMCSIM TOOLS
#
# ---------------------------------------

include ./Makefile.inc

SRC=./src

all: hmctognuplot
hmctognuplot:
	@echo " Building hmctognuplot"; $(CC) $(CFLAGS) -o $@ $(SRC)/hmctognuplot.c $(LDFLAGS) $(LDLIBS)
clean: 
	rm -Rf ./hmctognuplot ./src/*.o
