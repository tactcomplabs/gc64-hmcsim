#!/bin/sh

#-- CC
#-- C COMPILER
CC=gcc

#-- CXX
#-- C++ COMPILER
CXX=g++

#-- INC
#-- INCLUDE DIRECTORIES
INC=-I../../../include/

#-- CFLAGS
#-- C COMPILER FLAGS
CFLAGS=-O2 -Wall -g $(INC) -D_4GB_

#-- CXXFLAGS
#-- C++ COMPILER FLAGS
CXXFLAGS=-O2 -Wall -g $(INC)

#-- LDFLAGS
#-- LINKER OPTIONS
LDFLAGS=-L../../../

#-- LDLIBS
#-- LIBRARIES FOR LINKING
LDLIBS=

#-- DOXYGEN
#-- DOXYGEN BINARY
DOXYGEN=`which doxygen`

#-- EOF
