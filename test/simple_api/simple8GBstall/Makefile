#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: SIMPLE8GBSTALL
# ---------------------------------------

include ../Makefile.inc

SRC=./src

all: simple8GB
simple8GB:
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)/simple8GB.cpp $(LDFLAGS) $(LDLIBS)
clean:
	rm -Rf ./simple8GB ./src/*.o
