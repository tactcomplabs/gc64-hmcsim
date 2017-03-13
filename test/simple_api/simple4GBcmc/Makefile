#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: SIMPLE4GB
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: simple4GB
simple4GB:
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)/simple4GB.cpp $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./simple4GB ./src/*.o
