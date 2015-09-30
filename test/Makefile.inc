#!/bin/sh


#---------------------------------------------------------
# MACROS
#
#---------------------------------------------------------

#-- HMCSIM_TEST_MACROS
#-- HMCSIM TEST BUILD MACROS
HMCSIM_TEST_MACROS=

#-- CC
#-- C COMPILER
CC=gcc

#-- CXX
#-- C++ COMPILER
CXX=g++

#-- INC
#-- INCLUDE DIRECTORIES
INC=-I../../include/

#-- CFLAGS
#-- C COMPILER FLAGS
CFLAGS=-O0 -Wall -g $(INC) $(HMCSIM_TEST_MACROS)

#-- CXXFLAGS
#-- C++ COMPILER FLAGS
CXXFLAGS=-O0 -Wall -g $(INC) $(HMCSIM_TEST_MACROS)

#-- LDFLAGS
#-- LINKER OPTIONS
LDFLAGS=-L../../

#-- LDLIBS
#-- LIBRARIES FOR LINKING
LDLIBS=-lhmcsim

#-- AR
#-- ARCHIVE BINARY
AR=ar

#-- AR_OPTS
#-- ARCHIVE OPTIONS
AR_OPTS=rcs

#-- DOXYGEN
#-- DOXYGEN BINARY
DOXYGEN=`which doxygen`

#-- EOF 
