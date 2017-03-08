#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: SIMPLE8GB
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: simple8GB
simple4GB:
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)/simple8GB.cpp $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./simple8GB ./src/*.o
